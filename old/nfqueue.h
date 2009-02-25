#ifndef _NFQUEUE_H_
#define _NFQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <libnetfilter_queue/libnetfilter_queue.h>


void nfqueue_received();

/* returns packet id */
static u_int32_t print_pkt (struct nfq_data *packet);

u_int32_t nfqueue_get_id(struct nfq_data *packet);

u_int32_t nfqueue_get_dest(struct nfq_data *packet);

static int manager_pkt(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data);
