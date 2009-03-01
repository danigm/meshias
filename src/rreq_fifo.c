#include "rreq_fifo.h"
#include <stdlib.h>

void __rreq_fifo_alarm_cb(struct alarm_block* alarm, void *data)
{
    
}

struct rreq_fifo* rreq_fifo_alloc()
{
    struct rreq_fifo* queue = (struct rreq_fifo*)
        malloc(sizeof(struct rreq_fifo*));
        
    init_alarm(&queue->alarm, queue, __rreq_fifo_alarm_cb);
    INIT_LIST_HEAD(&(queue->list));
    
    return queue;
}

void rreq_fifo_delete(struct rreq_fifo* queue)
{
    struct rreq_fifo *entry, *tmp;
    
    list_for_each_entry_safe(entry, tmp, &queue->list, list)
    {
        list_del(&entry->list);
        free(entry);
    }
    free(queue);
}

void rreq_fifo_push(struct rreq_fifo* queue, uint32_t rreq_id,
    struct in_addr dst)
{
    struct rreq_fifo* entry = (struct rreq_fifo*)
        malloc(sizeof(struct rreq_fifo*));
    
    entry->rreq_id = rreq_id;
    entry->dst = dst;
    
    list_add(&entry->list, &queue->list);
}

uint8_t rreq_fifo_contains(struct rreq_fifo* queue, uint32_t rreq_id,
    struct in_addr dst)
{
    struct rreq_fifo *entry;
    
    list_for_each_entry(entry, &queue->list, list)
    {
        if(entry->rreq_id == rreq_id && entry->dst.s_addr == dst.s_addr)
            return 1;
    }
    return 0;
}

void rreq_fifo_del(struct rreq_fifo* queue, uint32_t rreq_id,
    struct in_addr dst)
{
    struct rreq_fifo *entry, *tmp;
    
    list_for_each_entry_safe(entry, tmp, &queue->list, list)
    {
        if(entry->rreq_id == rreq_id && entry->dst.s_addr == dst.s_addr)
        {
            list_del(&entry->list);
            return;
        }
    }
}
