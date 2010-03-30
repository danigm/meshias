#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netlink/addr.h>
#include <netinet/in.h>
#include <netlink/route/rtnl.h>
#include <netlink/route/route.h>
#include <netlink/route/nexthop.h>
#include "routing_table.h"
#include "msh_data.h"
#include "utils.h"
#include "statistics.h"

struct routing_table {
    struct msh_route route_list;

    //Used for searching internally
    struct msh_route *find_route;
};

void __routing_table_route_updated_cb(struct msh_route* route, uint32_t change_flag,
                                      void *qdata)
{
    if (change_flag & RTACTION_DESTROY) {
        printf("routing_table_destroyed %p\n", route);
        list_del(&route->list);

        struct rtnl_route *nlroute = msh_route_get_rtnl_route(route);
        msh_route_set_rtnl_route(route, 0);

        if (!nlroute) {
            return;
        }

        if (rtnl_route_del(data.nl_handle, nlroute, 0) < 0) {
            fprintf(stderr, "rtnl_route_del failed: %s\n", nl_geterror());
        }

        rtnl_route_put(nlroute);
    } else if (change_flag & RTACTION_CHANGE_NEXTHOP_IP) {
        struct rtnl_route *nlroute = msh_route_get_rtnl_route(route);

        if (!nlroute) {
            return;
        }

        uint8_t dst_len = msh_route_get_prefix_sz(route);
        struct in_addr next_hop_addr = msh_route_get_next_hop(route);
        struct nl_addr *nexthop = in_addr2nl_addr(next_hop_addr, dst_len);

        if (route->flags & RTFLAG_HAS_NEXTHOP) {
            rtnl_route_set_gateway(nlroute, nexthop);
        }
    }
}

struct routing_table *routing_table_alloc() {
    struct routing_table *table =
        (struct routing_table *)calloc(1, sizeof(struct routing_table));
    INIT_LIST_HEAD(&(table->route_list.list));
    table->find_route = msh_route_alloc();

    return table;
}

void routing_table_delete(struct routing_table *table)
{
    struct msh_route *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &table->route_list.list, list) {
        routing_table_del(table, entry);
        free(entry);
    }
    msh_route_destroy(table->find_route);
}

int routing_table_add(struct routing_table *table, struct msh_route *route)
{
    struct msh_route *found;

    // If route exists then we have nothing to do here: this function only
    // adds new routes, it doesn't update existing entries.
    if ((found = routing_table_find(table, route,
                                    RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING)) != 0) {
        return 1;
    }

    struct rtnl_route *nlroute = rtnl_route_alloc();

    uint8_t dst_len = msh_route_get_prefix_sz(route);

    struct nl_addr *dst = in_addr2nl_addr(route->dst_ip, dst_len);

    char buf[256];

    rtnl_route_set_oif(nlroute, data.net_iface);

    rtnl_route_set_dst(nlroute, dst);

    rtnl_route_set_scope(nlroute, RT_SCOPE_UNIVERSE);

    if (route->flags & RTFLAG_HAS_NEXTHOP) {
        struct in_addr next_hop_addr = msh_route_get_next_hop(route);
        struct nl_addr *nexthop = in_addr2nl_addr(next_hop_addr, 0);

        // TODO: call to rtnl_route_set_ttl() ... or maybe not?

        char buf2[200];
        printf("nexthop %s\n", nl_addr2str(nexthop, buf2, 200));
        printf("dest %s\n", nl_addr2str(dst, buf2, 200));

        // If the next hop is the destination, there's no need to set the gateway
        if (route->dst_ip.s_addr != next_hop_addr.s_addr) {
            rtnl_route_set_gateway(nlroute, nexthop);
        }
    }

    int errno = 0;

    // Errno = 17 means that route already exists so it's not problematic and
    // we can ignore it
    if ((errno = rtnl_route_add(data.nl_handle, nlroute, 0)) < 0 && errno != -17) {
        fprintf(stderr, "rtnl_route_add failed: errno=%d %s\n", errno, nl_geterror());
        return -1;
    }

    // If we are successful, add the netlink route to the msh_route, set
    // the lifetime of the route and add the route to the list of the routing table
    msh_route_set_rtnl_route(route, nlroute);
    msh_route_set_lifetime(route, ACTIVE_ROUTE_TIMEOUT());
    // add a callback to the route for getting updates
    // route __routing_table_route_updated_cb;
    msh_route_set_updated_callback(route, __routing_table_route_updated_cb, 0);

    list_add(&route->list, &table->route_list.list);

    // TODO: remove this in routing_table_del instead (because it's being used by route_nexthop)
//     nl_addr_destroy(nexthop);

    printf("routing_table_added %p\n", route);
    printf("packets_fifo_process_route %s\n", inet_htoa(route->dst_ip));
    packets_fifo_process_route(data.packets_queue, route);
    return 0;
}

int routing_table_del(struct routing_table *table, struct msh_route *route)
{
    struct msh_route *found;
    struct rtnl_route *nlroute;

    // If route is not in our list we have nothing to do here
    if ((found = routing_table_find(table, route, 0)) != 0) {
        return 1;
    }

    // If flag RTFLAG_UNMANAGED is set it means this is an route not managed
    // by ourselves and thus external and readonly.
    if (!(found->flags & RTFLAG_UNMANAGED)) {
        nlroute = msh_route_get_rtnl_route(route);

        if (rtnl_route_del(data.nl_handle, nlroute, 0) < 0) {
            fprintf(stderr, "rtnl_route_del failed: %s\n", nl_geterror());
            return -1;
        }

        msh_route_set_rtnl_route(route, 0);
    }

    rtnl_route_put(nlroute);
    list_del(&route->list);
    msh_route_destroy(route);

    return 0;
}

struct msh_route *routing_table_find(struct routing_table *table,
                                     struct msh_route *route, int attr_flags) {
    struct msh_route *entry;

    list_for_each_entry(entry, &table->route_list.list, list) {
        if (msh_route_compare(route, entry, attr_flags) == 0) {
            return entry;
        }
    }

    return 0;
}

struct msh_route *routing_table_find_by_ip(struct routing_table *table,
        struct in_addr addr) {
    struct msh_route *route;

    msh_route_set_dst_ip(table->find_route, addr);
    route = routing_table_find(table, table->find_route,
                               RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING);

    return route;

}

uint8_t routing_table_use_route(struct routing_table *table,
                                struct in_addr dst_ip, struct msh_route **invalid_route, struct in_addr orig_ip)
{
    struct msh_route *dst_route, *next_hop_route, *orig_route, *prev_hop_route;

    //if it's a packet directed to us, there's  no need to find a route to us
    if (dst_ip.s_addr != data.ip_addr.s_addr) {
        dst_route = routing_table_find_by_ip(table, dst_ip);

        // If route not found or not active/invalid, return 0
        if (!dst_route) {
            stats.route_not_found++;
            return 0;
        }

        if (!(msh_route_get_flags(dst_route) & RTFLAG_VALID_ENTRY)) {
            stats.invalid_route++;
            invalid_route = &dst_route;
            return 0;
        }

//    From Page 12 of RFC 3561:
//    Each time a route is used to forward a data packet, its Active Route
//    Lifetime field of the source, destination and the next hop on the
//    path to the destination is updated to be no less than the current
//    time plus ACTIVE_ROUTE_TIMEOUT.  Since the route between each
//    originator and destination pair is expected to be symmetric, the
//    Active Route Lifetime for the previous hop, along the reverse path
//    back to the IP source, is also updated to be no less than the current
//    time plus ACTIVE_ROUTE_TIMEOUT.

        // Active route found, reset the lifetime
        msh_route_set_lifetime(dst_route, ACTIVE_ROUTE_TIMEOUT());

        // Update also the lifetime of the next_hop
        struct in_addr next_hop =  msh_route_get_next_hop(dst_route);

        if (next_hop.s_addr != dst_ip.s_addr) {
            if ((next_hop_route = routing_table_find_by_ip(table, next_hop)) != 0) {
                msh_route_set_lifetime(next_hop_route, ACTIVE_ROUTE_TIMEOUT());
            } else
                // not finding a route to the next_hop is a bug because if we have a
                // route to the destination, we should have already found a route to
                // the next_hop too.
            {
                puts("BUG: no route to next_hop found!");
            }
        }
    }

    // orig_ip is set to zero means we don't want to update that part of the route
    if (orig_ip.s_addr == 0) {
        return 1;
    }

    // If found a route for the orig ip, update it
    if ((orig_route = routing_table_find_by_ip(data.routing_table,
                      orig_ip)) != 0) {
        // reset the lifetime and mark as valid
        msh_route_set_lifetime(orig_route, ACTIVE_ROUTE_TIMEOUT());
        // Update also the lifetime of the next_hop
        struct in_addr prev_hop =  msh_route_get_next_hop(orig_route);

        if (prev_hop.s_addr != orig_ip.s_addr) {
            if ((prev_hop_route = routing_table_find_by_ip(table, prev_hop)) != 0) {
                msh_route_set_lifetime(prev_hop_route, ACTIVE_ROUTE_TIMEOUT());
            } else {
                puts("BUG: no route to prev_hop found!");
            }
        }
    } else {
        puts("BUG: no route to orig found!");
    }

    return 1;
}

void routing_table_foreach(struct routing_table *table,
                           int (*callback_func)(struct msh_route *, void *), void *data)
{
    struct msh_route *entry;

    list_for_each_entry(entry, &table->route_list.list, list) {
        callback_func(entry, data);
    }
}
