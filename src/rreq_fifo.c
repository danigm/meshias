#include "rreq_fifo.h"
#include <stdlib.h>

struct rreq_fifo* rreq_fifo_alloc()
{
    struct rreq_fifo* queue = (struct rreq_fifo*)
        malloc(sizeof(struct rreq_fifo*));
    return queue;
}

void rreq_fifo_delete(struct rreq_fifo* queue)
{
    free(queue);
}

void rreq_fifo_push(struct rreq_fifo* queue, uint32_t id)
{
    
}

uint32_t rreq_fifo_pull(struct rreq_fifo* queue)
{
    return 0;
}

uint8_t rreq_fifo_is_empty(struct rreq_fifo* queue)
{
    return 0;
}
