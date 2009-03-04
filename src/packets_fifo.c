#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>        /* for NF_ACCEPT */

#include "packets_fifo.h"
#include "route_obj.h"
#include "msh_data.h"

struct packets_fifo* packets_fifo_alloc()
{
    struct packets_fifo* queue = (struct packets_fifo*)
        calloc(1, sizeof(struct packets_fifo));
    INIT_LIST_HEAD(&(queue->list));
    
    return queue;
}

void packets_fifo_delete(struct packets_fifo* queue)
{
    struct packets_fifo *entry, *tmp;
    
    list_for_each_entry_safe(entry, tmp, &queue->list, list)
    {
        packet_obj_drop(entry);
    }
    free(queue);
}

void packet_obj_accept(struct packets_fifo* packet_obj)
{
    nfq_set_verdict(data.queue, packet_obj->id, NF_ACCEPT, 0, NULL);
    free(packet_obj);
}

void packet_obj_drop(struct packets_fifo* packet_obj)
{
    nfq_set_verdict(data.queue, packet_obj->id, NF_DROP, 0, NULL);
    free(packet_obj);
}

void packets_fifo_push(struct packets_fifo* queue, uint32_t id,
    struct in_addr dest)
{
    struct packets_fifo* packet_obj = (struct packets_fifo*)
        calloc(1, sizeof(struct packets_fifo*));
    
    packet_obj->id = id;
    packet_obj->dest.s_addr = dest.s_addr;
    
    list_add(&packet_obj->list, &queue->list);
}

void packets_fifo_drop_packets(struct packets_fifo* queue, struct in_addr dest)
{
    struct packets_fifo *entry, *tmp;
    
    list_for_each_entry_safe(entry, tmp, &queue->list, list)
    {
        if(entry->dest.s_addr == dest.s_addr)
            packet_obj_drop(entry);
    }
}

uint32_t packets_fifo_process_route(struct packets_fifo* queue,
    struct msh_route* route)
{
    struct packets_fifo *entry, *tmp;
    struct msh_route *second = msh_route_alloc();
    
    list_for_each_entry_safe(entry, tmp, &queue->list, list)
    {
        msh_route_set_dst_ip(second, entry->dest);
        
        // Free the packets matched by this new route
        if(msh_route_compare(route, second,
            RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING))
            packet_obj_accept(entry);
    }
    free(route);
    msh_route_destroy(second);
}

uint8_t packets_fifo_is_empty(struct packets_fifo* queue)
{
    return list_empty(&queue->list);
}
