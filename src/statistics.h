#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdint.h>

struct statistics_t
{
    // aodv_logic.c
    uint32_t packets_dropped;

    // aodv_packet.c
    // recvmsg
    uint32_t no_address_received;
    uint32_t no_payload_received;
    uint32_t no_control_received;

    // sendto
    uint32_t send_aodv_errors;
    uint32_t send_aodv_incomplete;

    // check received packets
    uint32_t rreq_incorrect_size;
    uint32_t rrep_incorrect_size;
    uint32_t rerr_incorrect_size;
    uint32_t rerr_dest_cont_zero;
    uint32_t rrep_ack_incorrect_size;
    uint32_t aodv_incorrect_type;

    // TTL is not received in control msg
    uint32_t ttl_not_found;

    // daemon.c
    uint32_t error_aodv_recv;
    uint32_t error_nf_recv;
    uint32_t error_unix_recv;

    // routing_table.c
    uint32_t route_not_found;
    uint32_t invalid_route;
};

extern struct statistics_t stats;

void stats_init();
void stats_reset();

#endif
