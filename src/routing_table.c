#include <stdlib.h>

#include <netlink/addr.h>
#include <netinet/in.h>
#include <netlink/route/rtnl.h>
#include <netlink/route/route.h>
#include <netlink/addr.h>
#include "routing_table.h"
#include "msh_data.h"

struct routing_table
{
    struct msh_route route_list;
};

void __fill_routing_table(struct nl_object* obj, void *arg)
{
    struct routing_table *table = (struct routing_table *)arg;
    struct rtnl_route* route = (struct rtnl_route*)obj;
    
    char buf[128];
    
    struct nl_addr *addr = rtnl_route_get_dst(route);
    
    if(addr != NULL && rtnl_route_get_table(route) == 254)
    {
        struct msh_route* mshRoute = msh_route_alloc();
        struct in_addr *addr_dst = nl_addr_get_binary_addr(addr);
        
        printf("Route \ttable %d\tdst %s\t\tdst len %d\n",
            rtnl_route_get_table(route),
            nl_addr2str(addr, buf, sizeof(buf)),
            rtnl_route_get_dst_len(route));
        
        msh_route_set_dst_ip(mshRoute, *addr_dst);
        msh_route_set_prefix_sz(mshRoute, rtnl_route_get_dst_len(route));
        msh_route_unset_flag(mshRoute, RTFLAG_VALID_DEST_SEQ_NUM);
        // As it's not a meshias route but an external one, it won't have a
        // lifetime set, but it will be a valid entry. Forever.
        msh_route_set_flag(mshRoute, RTFLAG_VALID_ENTRY);
        routing_table_add(data.routing_table, mshRoute);
    } else
        printf("Route (unkown addr)\n");
    
    
//     struct nl_dump_params dp = {
//         .dp_type = NL_DUMP_FULL,
//         .dp_fd = stdout,
//         .dp_dump_msgtype = 1,
//     };
// 
//     if(rtnl_route_get_table(route) == 254)
//         nl_object_dump(obj, &dp);
}

struct routing_table *routing_table_alloc()
{
    struct routing_table *table =
        (struct routing_table *)calloc(1, sizeof(struct routing_table));
    INIT_LIST_HEAD(&(table->route_list.list));
    
    // Fill the cache
    struct nl_cache *route_cache = rtnl_route_alloc_cache(data.nl_handle);
    printf("Route cache (%d ifaces):\n", nl_cache_nitems(route_cache));
    nl_cache_foreach(route_cache, __fill_routing_table, (void *)table);
    nl_cache_free(route_cache);
    
    return table;
}

void routing_table_delete(struct routing_table *table)
{
    struct msh_route *entry, *tmp;
    
    list_for_each_entry_safe(entry, tmp, &table->route_list.list, list)
    {
        routing_table_del(table, entry);
        free(entry);
    }
}

int routing_table_add(struct routing_table *table, struct msh_route *route)
{
    struct msh_route *found;
    struct rtnl_route *nlroute;
    
    // If route exists then we have nothing to do here: this function only
    // adds new routes, it doesn't update existing entries.
    if((found = routing_table_find(table, route,
        RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING)) != 0)
        return 1;

    nlroute = rtnl_route_alloc();
 
    struct nl_addr *dst = in_addr2nl_addr(&route->dst_ip,
        msh_route_get_prefix_sz(route));
    
    struct nl_addr *nexthop = in_addr2nl_addr(&route->nexthop_ip, 0);
    
    uint8_t dst_len = 32 - msh_route_get_prefix_sz(route);
    
    rtnl_route_set_oif(nlroute, msh_route_get_net_iface(route));
    rtnl_route_set_family(nlroute, AF_INET);
    rtnl_route_set_scope(nlroute, RT_SCOPE_LINK);
    rtnl_route_set_dst(nlroute, dst);
//TODO
//     if(route->flags & RTFLAG_HAS_NEXTHOP)
//         rtnl_route_add_nexthop(nlroute, nexthop);
    if (rtnl_route_add(data.nl_handle, nlroute, 0) < 0)
    {
        fprintf(stderr, "rtnl_route_add failed: %s\n", nl_geterror());
        nl_addr_destroy(dst);
        nl_addr_destroy(nexthop);
        return -1;
    }
    // If we are successful, add the netlink route to the msh_route, set
    // the lifetime of the route and add the route to the list of the routing table
    msh_route_set_rtnl_route(route, nlroute);
    msh_route_set_lifetime(route, ACTIVE_ROUTE_TIMEOUT());
    //TODO: add a callback to the route for getting updates
    
    list_add(&route->list, &table->route_list.list);
    
    nl_addr_destroy(dst);
    nl_addr_destroy(nexthop);

    return 0;
}

int routing_table_del(struct routing_table *table, struct msh_route *route)
{
    struct msh_route *found;
    struct rtnl_route *nlroute;
    
    // If route is not in our list we have nothing to do here
    if((found = routing_table_find(table, route, 0)) != 0)
        return 1;

    nlroute = msh_route_get_rtnl_route(route);
    if (rtnl_route_del(data.nl_handle, nlroute, 0) < 0)
    {
        fprintf(stderr, "rtnl_route_del failed: %s\n", nl_geterror());
        return -1;
    }
    msh_route_set_rtnl_route(route, 0);
    rtnl_route_put(nlroute);
    list_del(&route->list);
    
    return 0;
}

struct msh_route *routing_table_find(struct routing_table *table,
    struct msh_route *route, int attr_flags)
{
    struct msh_route *entry;
    
    puts("routing_table_find: ");
    
    list_for_each_entry(entry, &table->route_list.list, list)
    {
        puts("recorriendo entrada de la tabla de rutas");
        if(msh_route_compare(route, entry, attr_flags) == 0)
            return entry;
    }
    
    return NULL;
}


uint8_t routing_table_use_route(struct routing_table *table,
    struct in_addr dst_ip, struct msh_route **invalid_route)
{
    struct msh_route *route;
    struct msh_route *find_route = msh_route_alloc();
    msh_route_set_dst_ip(find_route, dst_ip);
    
    route = routing_table_find(table, find_route,
        RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING);
    msh_route_destroy(find_route); // Not needed anymore
    
    // If route not found or not active/invalid, return 0
    if(!route || !(msh_route_get_flags(route) & RTFLAG_VALID_ENTRY))
    {
        invalid_route = &route;
        return 0;
    }
    
    // Active route found, reset the lifetime
    msh_route_set_lifetime(route, ACTIVE_ROUTE_TIMEOUT());
    
    /* TODO (RFC Page 12)
     * Since the route between each
     * originator and destination pair is expected to be symmetric, the
     * Active Route Lifetime for the previous hop, along the reverse path
     * back to the IP source, is also updated to be no less than the current
     * time plus ACTIVE_ROUTE_TIMEOUT.
     */
    return 1;
}
