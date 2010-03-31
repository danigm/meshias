#include <stdlib.h>

#include "rreq_fifo.h"
#include "route_obj.h"
#include "aodv/logic.h"
#include "utils.h"
#include "statistics.h"
#include "msh_data.h"
#include "log.h"
#include "packets_fifo.h"
#include "aodv/configuration_parameters.h"

void __rreq_fifo_entry_delete(struct rreq_fifo* entry)
{
    del_alarm(&entry->alarm);
    list_del(&entry->list);
    free(entry);
}

void __rreq_fifo_alarm_cb(struct alarm_block* alarm, void *qdata)
{
    debug(1, "");
    struct rreq_fifo* entry = (struct rreq_fifo*)qdata;
    struct in_addr dst = { entry->dst.s_addr };

    // If the RREQ was not send  by us (i.e. if prev_tries < 0), we just delete
    // the entry.
    if (entry->prev_tries < 0) {
        __rreq_fifo_entry_delete(entry);
    }
    // If the RREQ was sent by ourselves it means that no RREP
    // has been received and thus we shall send a new RREQ
    else {
        unsigned long  sc, usc;
        // Try one more time
        entry->prev_tries++;

        if (entry->prev_tries > RREQ_RETRIES()) {
            __rreq_fifo_entry_delete(entry);
            // Route to the dest definitely not found: drop packets
            packets_fifo_drop_packets(data.packets_queue, entry->dst);
            stats.packets_dropped++;
            return;
        }

        // routing_table_update_route() will set invalid_route accordingly if
        // it finds a route but it has been marked as invalid. That's the only thing we
        // will use the routing_table_update_route() call for: we can't expect that
        // magically a route can be found, and if it's found it's actually an error
        // because in that case the RREQ should have been removed from the queue.
        struct msh_route *expired_route = 0;

        // An optional parameter set to zero
        struct in_addr orig = { 0 };

        if (routing_table_update_route(data.routing_table, dst, &expired_route, orig)) {
            debug(1, "Error: Found an unexpected route when a waiting RREQ expired\n");
        }

        set_alarm_time(binary_exponential_backoff_time(entry->prev_tries), &sc, &usc);
        add_alarm(&entry->alarm, sc, usc);

        aodv_find_route(dst, expired_route, entry->prev_tries);

        // We might have sent a new rreq (with aodv_find_route()), so we update
        // this entry's seq_num
        entry->rreq_id = data.rreq_id;
    }
}

struct rreq_fifo* rreq_fifo_alloc()
{
    struct rreq_fifo* queue = (struct rreq_fifo*)
                              calloc(1, sizeof(struct rreq_fifo));

    init_alarm(&queue->alarm, queue, __rreq_fifo_alarm_cb);
    INIT_LIST_HEAD(&(queue->list));

    return queue;
}

void rreq_fifo_delete(struct rreq_fifo* queue)
{
    struct rreq_fifo *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &queue->list, list) {
        __rreq_fifo_entry_delete(entry);
    }
    free(queue);
}

void rreq_fifo_push(struct rreq_fifo* queue, uint32_t rreq_id,
                    struct in_addr dst)
{
    struct rreq_fifo* entry = (struct rreq_fifo*)
                              calloc(1, sizeof(struct rreq_fifo));
    unsigned long  sc, usc;

    entry->rreq_id = rreq_id;
    entry->dst = dst;
    entry->prev_tries = -1;

    set_alarm_time(PATH_DISCOVERY_TIME(), &sc, &usc);
    init_alarm(&entry->alarm, entry, __rreq_fifo_alarm_cb);
    add_alarm(&entry->alarm, sc, usc);
    list_add(&entry->list, &queue->list);
}

void rreq_fifo_push_owned(struct rreq_fifo* queue, uint32_t rreq_id,
                          struct in_addr dst, int8_t prev_tries)
{
    struct rreq_fifo* entry = (struct rreq_fifo*)
                              calloc(1, sizeof(struct rreq_fifo));
    unsigned long  sc, usc;

    entry->rreq_id = rreq_id;
    entry->dst = dst;
    entry->prev_tries = prev_tries;

    // Binary exponential backoff
    set_alarm_time(binary_exponential_backoff_time(prev_tries), &sc, &usc);
    init_alarm(&entry->alarm, entry, __rreq_fifo_alarm_cb);
    add_alarm(&entry->alarm, sc, usc);

    printf("prev_tries %d\n", entry->prev_tries);
    printf("list_add %p\n", entry);
    list_add(&entry->list, &queue->list);
}

uint8_t rreq_fifo_contains(struct rreq_fifo* queue, uint32_t rreq_id,
                           struct in_addr dst)
{
    struct rreq_fifo *entry;

    list_for_each_entry(entry, &queue->list, list) {
        if (entry->rreq_id == rreq_id && entry->dst.s_addr == dst.s_addr) {
            return 1;
        }
    }
    return 0;
}

uint8_t rreq_fifo_waiting_response_for(struct rreq_fifo* queue,
                                       struct in_addr dst)
{
    struct rreq_fifo *entry;

    list_for_each_entry(entry, &queue->list, list) {
        // We check that prev_tries non-zero because the packed must be owned"
        // i.e. must have been sent by us
        if (entry->prev_tries >= 0 && entry->dst.s_addr == dst.s_addr) {
            return 1;
        }
    }
    return 0;
}

void rreq_fifo_del(struct rreq_fifo* queue, uint32_t rreq_id,
                   struct in_addr dst)
{
    struct rreq_fifo *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &queue->list, list) {
        if (entry->rreq_id <= rreq_id && entry->dst.s_addr == dst.s_addr) {
            __rreq_fifo_entry_delete(entry);
            return;
        }
    }
}
