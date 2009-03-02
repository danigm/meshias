#ifndef RREQ_FIFO_H_
#define RREQ_FIFO_H_

#include <stdint.h>
#include <netinet/in.h>
#include "alarm/linux_list.h"
#include "alarm/alarm.h"

struct rreq_fifo
{
    uint32_t rreq_id;
    struct in_addr dst;
    // If the rrep was not send by ourselves, prev_tries will be less than 
    // zero. Otherwise, it stores how many times has this rrep been sent
    // without success.
    int8_t prev_tries;
    
    struct list_head list;
    struct alarm_block alarm;
};


/**
 * Allocates a new RREQ FIFO queue.
 */
struct rreq_fifo* rreq_fifo_alloc();

/**
 * Free all the RREQs in the FIFO queue and frees the queue itself.
 */
void rreq_fifo_delete(struct rreq_fifo* queue);

/**
 * Add a new external RREQ (not send by ourselves) id to the queue.
 */
void rreq_fifo_push(struct rreq_fifo* queue, uint32_t rreq_id,
    struct in_addr dst);

/**
 * Add a new RREQ that was sent by ourselves to the queue.
 * @param try   indicates how many times has this RREQ been sent without
 *              success before.
 */
void rreq_fifo_push_owned(struct rreq_fifo* queue, uint32_t rreq_id,
    struct in_addr dst, int8_t prev_tries);

/**
 * Returns 1 if the RREQ is buffered, otherwise returns 0.
 */
uint8_t rreq_fifo_contains(struct rreq_fifo* queue, uint32_t rreq_id,
    struct in_addr dst);

/**
 * Returns 1 if there is a owned RREQ buffered with the given dest.
 */
uint8_t rreq_fifo_waiting_response_for(struct rreq_fifo* queue,
    struct in_addr dst);

/**
 * Locates the rreq_fifo entry with the given rreq_id and dst if found in the buffer,
 * removing the entry from the buffer if found.
 */
void rreq_fifo_del(struct rreq_fifo* queue, uint32_t rreq_id,
    struct in_addr dst);

#endif
