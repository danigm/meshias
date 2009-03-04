#ifndef AODV_DAEMON_H
#define AODV_DAEMON_H

#include "msh_data.h"
#include "aodv_packet.h"
#include "common.h"

#include <sys/socket.h>

/* Initialize the aodv's stuff like socket */
int daemon_init();

/* Free reserved memory */
void daemon_shutdown();

/* Function to receive packets and make an answer */
void daemon_receive_packets();

/* Funtions to process incoming packets */
void daemon_process_rreq(struct aodv_pkt* pkt);
void daemon_process_rrep(struct aodv_pkt* pkt);
void daemon_process_rerr(struct aodv_pkt* pkt);
void daemon_process_rrep_ack(struct aodv_pkt* pkt);

/**
 * This function is used to send a RREP in response to a RREQ. If the RREP
 * is sent (because we have an active route to the destination) it returns
 * a non-zero value. Otherwise, it returns 0.
 */
uint8_t daemon_answer_to_rreq(struct aodv_pkt* pkt_rreq);

#endif
