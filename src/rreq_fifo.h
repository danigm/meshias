#ifndef RREQ_FIFO_H_
#define RREQ_FIFO_H_

#include <stdint.h>

struct rreq_fifo
{
    uint32_t id;
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
 * Add a new RREQ id to the queue.
 */
void rreq_fifo_push(struct rreq_fifo* queue, uint32_t id);

/**
 * Get a RREQ if there's any available, otherwise returns 0.
 */
uint32_t rreq_fifo_pull(struct rreq_fifo* queue);

/**
 * Returns a non-zero number if the queue is empty or 0 otherwise.
 */
uint8_t rreq_fifo_is_empty(struct rreq_fifo* queue);

#endif
