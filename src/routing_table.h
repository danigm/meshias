#ifndef _ROUTING_TABLE_H_
#define _ROUTING_TABLE_H_

#include <netinet/in.h>
#include <stdint.h>
#include "alarm/linux_list.h"
#include "route_obj.h"

struct routing_table;

/**
 * Creates a routing table.
 * TODO: create it for a specific interface and flush all the routes for that interface
 */
struct routing_table *routing_table_alloc();

/**
 * Destroys a routing table.
 * TODO: flush all the routes for the interface related to this table
 */
void routing_table_destroy(struct routing_table *table);

/**
 * Flushes all the routes for the given interface.
 */
void routing_table_flush(uint32_t iface);

/**
 * Adds a route to the routing table structure and to the kernel's routing
 * table. If there's already a route for that destination (found by destination
 * ip longest-prefix matching)  this function doesn't do anything and returns -1.
 * If any error occurs when adding the route to the kernel's routing table, it
 * is not added to our own routing table and it returns -1. On success this
 * function returns 0.
 */
int routing_table_add(struct routing_table *table, struct msh_route *route);

/**
 * Removes a route to the routing table structure and from the kernel's routing
 * table, if possible. It will try first to locate the route in the routing
 * table comparing by addresses. If found, it will be then removed from kernel's
 * routing table and our own internal list of routes, and the rtnl_route will be
 * freed and set to 0. However, you'll be responsible for freeing the msh_route
 * itself,
 */
int routing_table_del(struct routing_table *table, struct msh_route *route);

/**
 * Finds a route in the routing table. It does so by comparing the attributes
 * set in attr_flags of the route with all the routes inside the table. When
 * a match is found, it's returned. If attr_flags is zero, it compares by
 * memory addresses.
 * 
 * @see msh_route_compare()
 */
struct msh_route *routing_table_find(struct routing_table *table,
    struct msh_route *route, int attr_flags);


// void routing_table_foreach(struct routing_table *table,
//     int (*callback_func)(struct msh_route *, void *), void *data);
//     
// void routing_table_foreach_filter(struct routing_table *table,
//     int (*callback_func)(struct msh_route *, void *), void *data);

#endif
