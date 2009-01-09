/*
 * (C) 2006-2008 by Pablo Neira Ayuso <pablo@netfilter.org>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "conntrackd.h"
#include "sync.h"
#include "us-conntrack.h"
#include "queue.h"
#include "debug.h"
#include "network.h"
#include "alarm.h"
#include "log.h"
#include "cache.h"
#include "event.h"

#include <string.h>

#if 0 
#define dp printf
#else
#define dp(...)
#endif

#if 0 
#define dprint printf
#else
#define dprint(...)
#endif

static LIST_HEAD(rs_list);
static LIST_HEAD(tx_list);
static unsigned int rs_list_len;
static unsigned int tx_list_len;
static struct queue *rs_queue;
static struct queue *tx_queue;
static uint32_t exp_seq;
static uint32_t window;
static uint32_t ack_from;
static int ack_from_set = 0;
static struct alarm_block alive_alarm;
static int hello_state = SAY_HELLO;

/* XXX: alive message expiration configurable */
#define ALIVE_INT 1

struct cache_ftfw {
	struct list_head 	rs_list;
	struct list_head	tx_list;
	uint32_t 		seq;
};

static void cache_ftfw_add(struct us_conntrack *u, void *data)
{
	struct cache_ftfw *cn = data;
	/* These nodes are not inserted in the list */
	INIT_LIST_HEAD(&cn->rs_list);
	INIT_LIST_HEAD(&cn->tx_list);
}

static void cache_ftfw_del(struct us_conntrack *u, void *data)
{
	struct cache_ftfw *cn = data;

	/* this node is already out of the list */
	if (list_empty(&cn->rs_list))
	    	return;

	/* no need for list_del_init since the entry is destroyed */
	list_del(&cn->rs_list);
	rs_list_len--;
}

static struct cache_extra cache_ftfw_extra = {
	.size 		= sizeof(struct cache_ftfw),
	.add		= cache_ftfw_add,
	.destroy	= cache_ftfw_del
};

static void tx_queue_add_ctlmsg(uint32_t flags, uint32_t from, uint32_t to)
{
	struct nethdr_ack ack = {
		.flags = flags,
		.from  = from,
		.to    = to,
	};

	switch(hello_state) {
	case SAY_HELLO:
		ack.flags |= NET_F_HELLO;
		break;
	case HELLO_BACK:
		ack.flags |= NET_F_HELLO_BACK;
		hello_state = HELLO_DONE;
		break;
	}

	queue_add(tx_queue, &ack, NETHDR_ACK_SIZ);
	write_evfd(STATE_SYNC(evfd));
}

static void ftfw_run(void);

/* this function is called from the alarm framework */
static void do_alive_alarm(struct alarm_block *a, void *data)
{
	if (ack_from_set && mcast_track_is_seq_set()) {
		/* exp_seq contains the last update received */
		dprint("send ALIVE ACK (from=%u, to=%u)\n",
			ack_from, STATE_SYNC(last_seq_recv));
		tx_queue_add_ctlmsg(NET_F_ACK,
				    ack_from,
				    STATE_SYNC(last_seq_recv));
		ack_from_set = 0;
	} else
		tx_queue_add_ctlmsg(NET_F_ALIVE, 0, 0);

	/* TODO: no need for buffered send, extracted from run_sync() */
	ftfw_run();
	mcast_buffered_pending_netmsg(STATE_SYNC(mcast_client));
}

#undef _SIGNAL_DEBUG
#ifdef _SIGNAL_DEBUG

static int rs_dump(void *data1, const void *data2)
{
	struct nethdr_ack *net = data1;

	dprint("in RS queue -> seq:%u flags:%u\n", net->seq, net->flags);

	return 0;
}

#include <signal.h>

static void my_dump(int foo)
{
	struct cache_ftfw *cn, *tmp;

	list_for_each_entry_safe(cn, tmp, &rs_list, rs_list) {
		struct us_conntrack *u;
		
		u = cache_get_conntrack(STATE_SYNC(internal), cn);
		dprint("in RS list -> seq:%u\n", cn->seq);
	}

	queue_iterate(rs_queue, NULL, rs_dump);
}

#endif

static int ftfw_init(void)
{
	tx_queue = queue_create(CONFIG(resend_queue_size));
	if (tx_queue == NULL) {
		dlog(LOG_ERR, "cannot create tx queue");
		return -1;
	}

	rs_queue = queue_create(CONFIG(resend_queue_size));
	if (rs_queue == NULL) {
		dlog(LOG_ERR, "cannot create rs queue");
		return -1;
	}

	init_alarm(&alive_alarm, NULL, do_alive_alarm);
	add_alarm(&alive_alarm, ALIVE_INT, 0);

	/* set ack window size */
	window = CONFIG(window_size);

#ifdef _SIGNAL_DEBUG
	signal(SIGUSR1, my_dump);
#endif

	return 0;
}

static void ftfw_kill(void)
{
	queue_destroy(rs_queue);
	queue_destroy(tx_queue);
}

static int do_cache_to_tx(void *data1, void *data2)
{
	struct us_conntrack *u = data2;
	struct cache_ftfw *cn = cache_get_extra(STATE_SYNC(internal), u);

	/* add to tx list */
	list_add_tail(&cn->tx_list, &tx_list);
	tx_list_len++;
	write_evfd(STATE_SYNC(evfd));

	return 0;
}

static int ftfw_local(int fd, int type, void *data)
{
	int ret = 1;

	switch(type) {
	case REQUEST_DUMP:
		dlog(LOG_NOTICE, "request resync");
		tx_queue_add_ctlmsg(NET_F_RESYNC, 0, 0);
		break;
	case SEND_BULK:
		dlog(LOG_NOTICE, "sending bulk update");
		cache_iterate(STATE_SYNC(internal), NULL, do_cache_to_tx);
		break;
	default:
		ret = 0;
		break;
	}

	return ret;
}

static int rs_queue_to_tx(void *data1, const void *data2)
{
	struct nethdr_ack *net = data1;
	const struct nethdr_ack *nack = data2;

	if (between(net->seq, nack->from, nack->to)) {
		dp("rs_queue_to_tx sq: %u fl:%u len:%u\n",
			net->seq, net->flags, net->len);
		queue_add(tx_queue, net, net->len);
		write_evfd(STATE_SYNC(evfd));
		queue_del(rs_queue, net);
	}
	return 0;
}

static int rs_queue_empty(void *data1, const void *data2)
{
	struct nethdr *net = data1;
	const struct nethdr_ack *h = data2;

	if (between(net->seq, h->from, h->to)) {
		dp("remove from queue (seq=%u)\n", net->seq);
		queue_del(rs_queue, data1);
	}
	return 0;
}

static void rs_list_to_tx(struct cache *c, unsigned int from, unsigned int to)
{
	struct cache_ftfw *cn, *tmp;

	list_for_each_entry_safe(cn, tmp, &rs_list, rs_list) {
		struct us_conntrack *u;
		
		u = cache_get_conntrack(STATE_SYNC(internal), cn);
		if (between(cn->seq, from, to)) {
			dp("resending nack'ed (oldseq=%u)\n", cn->seq);
			list_del_init(&cn->rs_list);
			rs_list_len--;
			list_add_tail(&cn->tx_list, &tx_list);
			tx_list_len++;
			write_evfd(STATE_SYNC(evfd));
		}
	} 
}

static void rs_list_empty(struct cache *c, unsigned int from, unsigned int to)
{
	struct cache_ftfw *cn, *tmp;

	list_for_each_entry_safe(cn, tmp, &rs_list, rs_list) {
		struct us_conntrack *u;

		u = cache_get_conntrack(STATE_SYNC(internal), cn);
		if (between(cn->seq, from, to)) {
			dp("queue: deleting from queue (seq=%u)\n", cn->seq);
			list_del_init(&cn->rs_list);
			rs_list_len--;
		}
	}
}

static int digest_msg(const struct nethdr *net)
{
	if (IS_DATA(net))
		return MSG_DATA;

	else if (IS_ACK(net)) {
		const struct nethdr_ack *h = (const struct nethdr_ack *) net;

		dprint("ACK(%u): from seq=%u to seq=%u\n",
			h->seq, h->from, h->to);
		rs_list_empty(STATE_SYNC(internal), h->from, h->to);
		queue_iterate(rs_queue, h, rs_queue_empty);
		return MSG_CTL;

	} else if (IS_NACK(net)) {
		const struct nethdr_ack *nack = (const struct nethdr_ack *) net;

		dprint("NACK(%u): from seq=%u to seq=%u\n",
			nack->seq, nack->from, nack->to);
		rs_list_to_tx(STATE_SYNC(internal), nack->from, nack->to);
		queue_iterate(rs_queue, nack, rs_queue_to_tx);
		return MSG_CTL;

	} else if (IS_RESYNC(net)) {
		dp("RESYNC ALL\n");
		cache_iterate(STATE_SYNC(internal), NULL, do_cache_to_tx);
		return MSG_CTL;

	} else if (IS_ALIVE(net))
		return MSG_CTL;

	return MSG_BAD;
}

static int digest_hello(const struct nethdr *net)
{
	int ret = 0;

	if (IS_HELLO(net)) {
		dlog(LOG_NOTICE, "The other node says HELLO");
		hello_state = HELLO_BACK;
		ret = 1;
	} else if (IS_HELLO_BACK(net)) {
		dlog(LOG_NOTICE, "The other node says HELLO BACK");
		hello_state = HELLO_DONE;
	}

	return ret;
}

static int ftfw_recv(const struct nethdr *net)
{
	int ret = MSG_DATA;

	if (digest_hello(net))
		goto bypass;

	switch (mcast_track_seq(net->seq, &exp_seq)) {
	case SEQ_AFTER:
		ret = digest_msg(net);
		if (ret == MSG_BAD) {
			ret = MSG_BAD;
			goto out;
		}

		if (ack_from_set) {
			tx_queue_add_ctlmsg(NET_F_ACK, ack_from, exp_seq-1);
			dprint("OFS send half ACK: from seq=%u to seq=%u\n", 
				ack_from, exp_seq-1);
			ack_from_set = 0;
		}

		tx_queue_add_ctlmsg(NET_F_NACK, exp_seq, net->seq-1);
		dprint("OFS send NACK: from seq=%u to seq=%u\n", 
			exp_seq, net->seq-1);

		/* count this message as part of the new window */
		window = CONFIG(window_size) - 1;
		ack_from = net->seq;
		ack_from_set = 1;
		break;

	case SEQ_BEFORE:
		/* we don't accept delayed packets */
		dlog(LOG_WARNING, "Received seq=%u before expected seq=%u",
				   net->seq, exp_seq);
		ret = MSG_DROP;
		break;

	case SEQ_UNSET:
	case SEQ_IN_SYNC:
bypass:
		ret = digest_msg(net);
		if (ret == MSG_BAD) {
			ret = MSG_BAD;
			goto out;
		}

		if (!ack_from_set) {
			ack_from_set = 1;
			ack_from = net->seq;
		}

		if (--window <= 0) {
			/* received a window, send an acknowledgement */
			dprint("OFS send ACK: from seq=%u to seq=%u\n",
				ack_from, net->seq);

			tx_queue_add_ctlmsg(NET_F_ACK, ack_from, net->seq);
			window = CONFIG(window_size);
			ack_from_set = 0;
		}
	}

out:
	if ((ret == MSG_DATA || ret == MSG_CTL))
		mcast_track_update_seq(net->seq);

	return ret;
}

static void ftfw_send(struct nethdr *net, struct us_conntrack *u)
{
	struct netpld *pld = NETHDR_DATA(net);
	struct cache_ftfw *cn;

	switch(ntohs(pld->query)) {
	case NFCT_Q_CREATE:
	case NFCT_Q_UPDATE:
	case NFCT_Q_DESTROY:
		cn = (struct cache_ftfw *) 
			cache_get_extra(STATE_SYNC(internal), u);

		if (!list_empty(&cn->rs_list)) {
			list_del_init(&cn->rs_list);
			rs_list_len--;
		}

		switch(hello_state) {
		case SAY_HELLO:
			net->flags = ntohs(net->flags) | NET_F_HELLO;
			net->flags = htons(net->flags);
			break;
		case HELLO_BACK:
			net->flags = ntohs(net->flags) | NET_F_HELLO_BACK;
			net->flags = htons(net->flags);
			hello_state = HELLO_DONE;
			break;
		}

		cn->seq = ntohl(net->seq);
		list_add_tail(&cn->rs_list, &rs_list);
		rs_list_len++;
		break;
	}
}

static int tx_queue_xmit(void *data1, const void *data2)
{
	struct nethdr *net = data1;
	size_t len = prepare_send_netmsg(STATE_SYNC(mcast_client), net);

	dp("tx_queue sq: %u fl:%u len:%u\n",
               ntohl(net->seq), ntohs(net->flags), ntohs(net->len));

	mcast_buffered_send_netmsg(STATE_SYNC(mcast_client), net, len);
	HDR_NETWORK2HOST(net);

	if (IS_DATA(net) || IS_ACK(net) || IS_NACK(net)) {
		dprint("tx_queue -> to_rs_queue sq: %u fl:%u len:%u\n",
        	       net->seq, net->flags, net->len);
		queue_add(rs_queue, net, net->len);
	}
	queue_del(tx_queue, net);

	return 0;
}

static int tx_list_xmit(struct list_head *i, struct us_conntrack *u, int type)
{
	int ret;
	struct nethdr *net = BUILD_NETMSG(u->ct, type);
	size_t len = prepare_send_netmsg(STATE_SYNC(mcast_client), net);

	dp("tx_list sq: %u fl:%u len:%u\n",
                ntohl(net->seq), ntohs(net->flags),
                ntohs(net->len));

	list_del_init(i);
	tx_list_len--;

	ret = mcast_buffered_send_netmsg(STATE_SYNC(mcast_client), net, len);
	ftfw_send(net, u);

	return ret;
}

static void ftfw_run(void)
{
	struct cache_ftfw *cn, *tmp;

	/* send messages in the tx_queue */
	queue_iterate(tx_queue, NULL, tx_queue_xmit);

	/* send conntracks in the tx_list */
	list_for_each_entry_safe(cn, tmp, &tx_list, tx_list) {
		struct us_conntrack *u;

		u = cache_get_conntrack(STATE_SYNC(internal), cn);
		if (alarm_pending(&u->alarm))
			tx_list_xmit(&cn->tx_list, u, NFCT_Q_DESTROY);
		else
			tx_list_xmit(&cn->tx_list, u, NFCT_Q_UPDATE);
	}

	/* reset alive alarm */
	add_alarm(&alive_alarm, 1, 0);

	dprint("tx_list_len:%u tx_queue_len:%u "
	       "rs_list_len: %u rs_queue_len:%u\n",
		tx_list_len, queue_len(tx_queue),
		rs_list_len, queue_len(rs_queue));
}

struct sync_mode sync_ftfw = {
	.internal_cache_flags	= LIFETIME,
	.external_cache_flags	= LIFETIME,
	.internal_cache_extra	= &cache_ftfw_extra,
	.init			= ftfw_init,
	.kill			= ftfw_kill,
	.local			= ftfw_local,
	.recv			= ftfw_recv,
	.send			= ftfw_send,
	.run			= ftfw_run,
};
