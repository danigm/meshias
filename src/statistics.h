#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdint.h>

struct statistics_t {
    /**
     * Packets dropped by nfqueue because we can't find a route.
     */
    uint32_t packets_dropped;
    /**
     * AODV packets received without address.
     */
    uint32_t no_address_received;
    /**
     * AODV packets received without payload.
     */
    uint32_t no_payload_received;
    /**
     * AODV packets received without ttl.
     */
    uint32_t no_ttl_received;
    /**
     * AODV packets sent with errors.
     */
    uint32_t send_aodv_errors;
    /**
     * AODV packets sent incompleted.
     */
    uint32_t send_aodv_incomplete;
    /**
     * AODV packets received with incorrect type.
     */
    uint32_t aodv_incorrect_type;
    /**
     * AODV rreq packets received with incorrect size.
     */
    uint32_t rreq_incorrect_size;
    /**
     * AODV rrep packets received with incorrect size.
     */
    uint32_t rrep_incorrect_size;
    /**
     * AODV rerr packets received with incorrect size.
     */
    uint32_t rerr_incorrect_size;
    /**
     * AODV rrep_ack packets received with incorrect size.
     */
    uint32_t rrep_ack_incorrect_size;
    /**
     * AODV rerr packets received without destination.
     */
    uint32_t rerr_dest_cont_zero;
    /**
     * Errors when daemon socket recv.
     */
    uint32_t error_aodv_recv;
    /**
     * Errors when netfilter_queue socket recv.
     */
    uint32_t error_nfq_recv;
    // FIXME Delete this
    uint32_t error_unix_recv;
    /**
     * ????
     */
    uint32_t invalid_route;
};

/**
 * Global statistics.
 */
extern struct statistics_t stats;

/**
 * Initialize the statistics.
 */
void stats_init();

/**
 * Free the mallocs.
 */
void stats_reset();

#endif
