/*
 * (C) 2006 by Pablo Neira Ayuso <pablo@netfilter.org>
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

#include "netlink.h"
#include "conntrackd.h"
#include "traffic_stats.h"
#include "ignore.h"
#include "log.h"
#include "debug.h"

#include <errno.h>

int ignore_conntrack(struct nf_conntrack *ct)
{
	/* ignore a certain protocol */
	if (CONFIG(ignore_protocol)[nfct_get_attr_u8(ct, ATTR_ORIG_L4PROTO)])
		return 1;

	/* Accept DNAT'ed traffic: not really coming to the local machine */
	if (nfct_getobjopt(ct, NFCT_GOPT_IS_DNAT)) {
		debug_ct(ct, "DNAT");
		return 0;
	}

	/* Accept SNAT'ed traffic: not really coming to the local machine */
	if (nfct_getobjopt(ct, NFCT_GOPT_IS_SNAT)) {
		debug_ct(ct, "SNAT");
		return 0;
	}

	/* Ignore traffic */
	if (ignore_pool_test(STATE(ignore_pool), ct)) {
		debug_ct(ct, "ignore traffic");
		return 1;
	}

	return 0;
}

static int event_handler(enum nf_conntrack_msg_type type,
			 struct nf_conntrack *ct,
			 void *data)
{
	/*
	 * Ignore this conntrack: it talks about a
	 * connection that is not interesting for us.
	 */
	if (ignore_conntrack(ct))
		return NFCT_CB_STOP;

	switch(type) {
	case NFCT_T_NEW:
		STATE(mode)->event_new(ct);
		break;
	case NFCT_T_UPDATE:
		STATE(mode)->event_upd(ct);
		break;
	case NFCT_T_DESTROY:
		if (STATE(mode)->event_dst(ct))
			update_traffic_stats(ct);
		break;
	default:
		dlog(LOG_WARNING, "unknown msg from ctnetlink\n");
		break;
	}

	return NFCT_CB_CONTINUE;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>

int nl_init_event_handler(void)
{
	STATE(event) = nfct_open(CONNTRACK, NFCT_ALL_CT_GROUPS);
	if (!STATE(event))
		return -1;

	fcntl(nfct_fd(STATE(event)), F_SETFL, O_NONBLOCK);

	/* set up socket buffer size */
	if (CONFIG(netlink_buffer_size))
		nfnl_rcvbufsiz(nfct_nfnlh(STATE(event)),
			       CONFIG(netlink_buffer_size));
	else {
		socklen_t socklen = sizeof(unsigned int);
		unsigned int read_size;

		/* get current buffer size */
		getsockopt(nfct_fd(STATE(event)), SOL_SOCKET,
			   SO_RCVBUF, &read_size, &socklen);

		CONFIG(netlink_buffer_size) = read_size;
	}

	/* ensure that maximum grown size is >= than maximum size */
	if (CONFIG(netlink_buffer_size_max_grown) < CONFIG(netlink_buffer_size))
		CONFIG(netlink_buffer_size_max_grown) =
					CONFIG(netlink_buffer_size);

	/* register callback for events */
	nfct_callback_register(STATE(event), NFCT_T_ALL, event_handler, NULL);

	return 0;
}

static int dump_handler(enum nf_conntrack_msg_type type,
			struct nf_conntrack *ct,
			void *data)
{
	/*
	 * Ignore this conntrack: it talks about a
	 * connection that is not interesting for us.
	 */
	if (ignore_conntrack(ct))
		return NFCT_CB_CONTINUE;

	switch(type) {
	case NFCT_T_UPDATE:
		STATE(mode)->dump(ct);
		break;
	default:
		dlog(LOG_WARNING, "unknown msg from ctnetlink");
		break;
	}
	return NFCT_CB_CONTINUE;
}

int nl_init_dump_handler(void)
{
	/* open dump netlink socket */
	STATE(dump) = nfct_open(CONNTRACK, 0);
	if (!STATE(dump))
		return -1;

	/* register callback for dumped entries */
	nfct_callback_register(STATE(dump), NFCT_T_ALL, dump_handler, NULL);

	if (nl_dump_conntrack_table() == -1)
		return -1;

	return 0;
}

int nl_init_overrun_handler(void)
{
	STATE(overrun) = nfct_open(CONNTRACK, 0);
	if (!STATE(overrun))
		return -1;

	fcntl(nfct_fd(STATE(overrun)), F_SETFL, O_NONBLOCK);

	nfct_callback_register(STATE(overrun), 
			       NFCT_T_ALL, 
			       STATE(mode)->overrun, 
			       NULL);
	return 0;
}

static int warned = 0;

void nl_resize_socket_buffer(struct nfct_handle *h)
{
	unsigned int s = CONFIG(netlink_buffer_size) * 2;

	/* already warned that we have reached the maximum buffer size */
	if (warned)
		return;

	if (s > CONFIG(netlink_buffer_size_max_grown)) {
		dlog(LOG_WARNING,
		     "maximum netlink socket buffer "
		     "size has been reached. We are likely to "
		     "be losing events, this may lead to "
		     "unsynchronized replicas. Please, consider "
		     "increasing netlink socket buffer size via "
		     "SocketBufferSize and "
		     "SocketBufferSizeMaxGrown clauses in "
		     "conntrackd.conf");
		s = CONFIG(netlink_buffer_size_max_grown);
		warned = 1;
	}

	CONFIG(netlink_buffer_size) = nfnl_rcvbufsiz(nfct_nfnlh(h), s);

	/* notify the sysadmin */
	dlog(LOG_NOTICE, "netlink socket buffer size "
			 "has been set to %u bytes",
			 CONFIG(netlink_buffer_size));
}

int nl_dump_conntrack_table(void)
{
	return nfct_query(STATE(dump), NFCT_Q_DUMP, &CONFIG(family));
}

int nl_overrun_request_resync(void)
{
	int family = CONFIG(family);
	return nfct_send(STATE(overrun), NFCT_Q_DUMP, &family);
}

int nl_exist_conntrack(struct nf_conntrack *ct)
{
	int ret;

	ret = nfct_query(STATE(dump), NFCT_Q_GET, ct);
	if (ret == -1)
		return errno == ENOENT ? 0 : -1;

	return 1;
}

/* This function modifies the conntrack passed as argument! */
int nl_create_conntrack(struct nf_conntrack *ct)
{
	uint8_t flags;

	/* XXX: related connections */
	if (nfct_attr_is_set(ct, ATTR_STATUS)) {
		uint32_t status = nfct_get_attr_u32(ct, ATTR_STATUS);
		status &= ~IPS_EXPECTED;
		nfct_set_attr_u32(ct, ATTR_STATUS, status);
	}

	nfct_setobjopt(ct, NFCT_SOPT_SETUP_REPLY);

	/*
	 * TCP flags to overpass window tracking for recovered connections
	 */
	flags = IP_CT_TCP_FLAG_BE_LIBERAL | IP_CT_TCP_FLAG_SACK_PERM;
	nfct_set_attr_u8(ct, ATTR_TCP_FLAGS_ORIG, flags);
	nfct_set_attr_u8(ct, ATTR_TCP_MASK_ORIG, flags);
	nfct_set_attr_u8(ct, ATTR_TCP_FLAGS_REPL, flags);
	nfct_set_attr_u8(ct, ATTR_TCP_MASK_REPL, flags);

	return nfct_query(STATE(dump), NFCT_Q_CREATE_UPDATE, ct);
}

/* This function modifies the conntrack passed as argument! */
int nl_update_conntrack(struct nf_conntrack *ct)
{
	/* unset NAT info, otherwise we hit error */
	nfct_attr_unset(ct, ATTR_SNAT_IPV4);
	nfct_attr_unset(ct, ATTR_DNAT_IPV4);
	nfct_attr_unset(ct, ATTR_SNAT_PORT);
	nfct_attr_unset(ct, ATTR_DNAT_PORT);

	if (nfct_attr_is_set(ct, ATTR_STATUS)) {
		uint32_t status = nfct_get_attr_u32(ct, ATTR_STATUS);
		status &= ~IPS_NAT_MASK;
		nfct_set_attr_u32(ct, ATTR_STATUS, status);
	}

	return nl_create_conntrack(ct);
}

int nl_destroy_conntrack(struct nf_conntrack *ct)
{
	return nfct_query(STATE(dump), NFCT_Q_DESTROY, ct);
}
