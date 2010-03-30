#ifndef PACKETS_FIFO_H_
#define PACKETS_FIFO_H_

#include <netinet/in.h>
#include <stdint.h>
#include "alarm/linux_list.h"

struct msh_route;

struct packets_fifo {
    uint32_t id;
    struct in_addr dest;
    struct list_head list;
};

/**
 * Allocates a new packets FIFO queue.
 */
struct packets_fifo* packets_fifo_alloc();

/**
 * Frees annd drop all the packets in the FIFO queue and frees the queue itself.
 */
void packets_fifo_delete(struct packets_fifo* queue);

/**
 * Free a single packets_fifo object, accepting the packet for that id
 * in the nf_queue.
 */
void packet_obj_accept(struct packets_fifo* entry);

/**
 * Free a single packets_fifo object, dropping the packet for that id
 * in the nf_queue.
 */
void packet_obj_drop(struct packets_fifo* entry);

/**
 * Add a new packet id to the queue.
 */
void packets_fifo_push(struct packets_fifo* queue, uint32_t id,
                       struct in_addr dest);

/**
 * If a route discovery has been attempted RREQ_RETRIES times at the maximum
 * TTL without receiving any RREP, all data packets destined for the
 * corresponding destination SHOULD be dropped from the buffer and a
 * Destination Unreachable message SHOULD be delivered to the application.
 * This function is used to drops packets in that case, and is called by
 * @see aodv_find_route().
 */
void packets_fifo_drop_packets(struct packets_fifo* queue, struct in_addr dest);

/**
 * Process a new route. We'll do destination ip longest-prefix matching.
 * For all the packets whose dest. ip matches, the packet is released.
 * Called when a new route is added to accept all the packets related to it.
 */
uint32_t packets_fifo_process_route(struct packets_fifo* queue,
                                    struct msh_route* route);

/**
 * Returns a non-zero number if the queue is empty or 0 otherwise.
 */
uint8_t packets_fifo_is_empty(struct packets_fifo* queue);

#endif
