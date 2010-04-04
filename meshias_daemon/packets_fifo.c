#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>        /* for NF_ACCEPT */

#include "packets_fifo.h"
#include "msh_route.h"
#include "msh_data.h"
#include "log.h"
#include "statistics.h"

struct packets_fifo* packets_fifo_alloc() {
    struct packets_fifo* queue = (struct packets_fifo*)
                                 calloc(1, sizeof(struct packets_fifo));
    INIT_LIST_HEAD(&(queue->list));

    return queue;
}

void packets_fifo_delete(struct packets_fifo* queue)
{
    struct packets_fifo *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &queue->list, list) {
        packet_obj_drop(entry);
    }
    free(queue);
}

void packet_obj_accept(struct packets_fifo* packet_obj)
{
    printf("packet_obj_accept ID %d\n", packet_obj->id);
    nfq_set_verdict(data.queue, packet_obj->id, NF_ACCEPT, 0, NULL);
    list_del(&packet_obj->list);
    free(packet_obj);
}

void packet_obj_drop(struct packets_fifo* packet_obj)
{
    nfq_set_verdict(data.queue, packet_obj->id, NF_DROP, 0, NULL);
    list_del(&packet_obj->list);
    free(packet_obj);

    stats.packets_dropped++;
}

void packets_fifo_push(struct packets_fifo* queue, uint32_t id,
                       struct in_addr dest)
{
    struct packets_fifo* packet_obj = (struct packets_fifo*)
                                      calloc(1, sizeof(struct packets_fifo));

    packet_obj->id = id;
    packet_obj->dest.s_addr = dest.s_addr;
    debug(2, "adding packet with id %d and dest %s", id, inet_htoa(dest));

    list_add(&packet_obj->list, &queue->list);
}

void packets_fifo_drop_packets(struct packets_fifo* queue, struct in_addr dest)
{
    struct packets_fifo *entry, *tmp;

    debug(2, "");
    list_for_each_entry_safe(entry, tmp, &queue->list, list) {
        if (entry->dest.s_addr == dest.s_addr) {
            debug(1, "DROP packet for destination %s", inet_htoa(dest));
            packet_obj_drop(entry);
        }
    }
}

void packets_fifo_process_route(struct packets_fifo* queue,
                                    struct msh_route* route)
{
    debug(2, "dest %s", inet_htoa(route->dst_ip));
    struct packets_fifo *entry, *tmp;
    struct msh_route *first = msh_route_alloc();

    list_for_each_entry_safe(entry, tmp, &queue->list, list) {
        debug(2, "packets_fifo_process_route: accept?");
        msh_route_set_dst_ip(first, entry->dest);

        // Free the packets matched by this new route
        if (msh_route_compare(first, route,
                              RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING) == 0) {
            debug(1, "packets_fifo_process_route: ACCEPT!");
            packet_obj_accept(entry);
        }
    }
    msh_route_destroy(first);
}

uint8_t packets_fifo_is_empty(struct packets_fifo* queue)
{
    return list_empty(&queue->list);
}
