#ifndef _AODV_LOGIC_H_
#define _AODV_LOGIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>

#include "msh_data.h"
#include "route_obj.h"
#include "aodv_packet.h"
#include "common.h"

/**
 * Sends a Route Request AODV message.
 */
void aodv_find_route(struct in_addr dest, struct msh_route *invalid_route,
    uint8_t prev_tries);

/* Funtions to process incoming packets */
void aodv_process_rreq(struct aodv_pkt* pkt);
void aodv_process_rrep(struct aodv_pkt* pkt);
void aodv_process_rerr(struct aodv_pkt* pkt);
void aodv_process_rrep_ack(struct aodv_pkt* pkt);

/**
 * This function is used to send a RREP in response to a RREQ. If the RREP
 * is sent (because we have an active route to the destination) it returns
 * a non-zero value. Otherwise, it returns 0.
 */
uint8_t aodv_answer_to_rreq(struct aodv_pkt* pkt_rreq);


#endif

