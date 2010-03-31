#ifndef LOGIC_H
#define LOGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>


#include "../route_obj.h"
#include "../msh_data.h"

#include "packet.h"

/**
 * Sends a Route Request AODV message.
 */
void aodv_find_route(struct in_addr dest, struct msh_route *invalid_route,
                     uint8_t prev_tries);

/* Funtions to process incoming packets */
void aodv_process_packet(struct msghdr *msg, int);
void aodv_process_rreq(struct aodv_pkt *pkt);
void aodv_process_rrep(struct aodv_pkt *pkt);
void aodv_process_rerr(struct aodv_pkt *pkt);
void aodv_process_rrep_ack(struct aodv_pkt* pkt);

/**
 * This function is used to send a RREP in response to a RREQ. If the RREP
 * is sent (because we have an active route to the destination) it returns
 * a non-zero value. Otherwise, it returns 0.
 */
uint8_t aodv_answer_to_rreq(struct aodv_rreq* pkt_rreq, struct in_addr prev_hop,
                            struct msh_route* route_to_orig);


#endif
