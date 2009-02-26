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

struct routing_table *routing_table_alloc()
{
    struct routing_table *table =
        (struct routing_table *)malloc(sizeof(struct routing_table *));
    INIT_LIST_HEAD(&(table->route_list.list));
    
    return table;
}

void routing_table_destroy(struct routing_table *table)
{
    struct msh_route *entry, *tmp;
    
    list_for_each_entry_safe(entry, tmp, &table->route_list.list, list)
    {
        routing_table_del(table, entry);
        free(entry);
    }
    routing_table_flush(0);
}

void routing_table_flush(uint32_t iface)
{
    
}

void routing_table_add(struct routing_table *table, struct msh_route *route)
{
    struct msh_route *found;
    struct rtnl_route *nlroute;
    
    // If route exists, don't add it in some cases, just modify it.
    // TODO: compare entry better
    if((found = routing_table_find(table, route, ~0)) != 0)
    {
        // TODO:
        // update logic: if current entry is invalid or num hops is less than
        // current, etc, we should change current entry.
        return;
    }
    
    struct nl_addr *dst = in_addr2nl_addr(&route->dst_ip,
        msh_route_get_prefix_sz(route));
    
    struct nl_addr *gateway = in_addr2nl_addr(&route->gateway_ip, 0);
    
    uint8_t dst_len = 32 - msh_route_get_prefix_sz(route);
        
    rtnl_route_set_oif(nlroute, msh_route_get_net_iface(route));
    rtnl_route_set_family(nlroute, AF_INET);
    rtnl_route_set_scope(nlroute, RT_SCOPE_LINK);
    rtnl_route_set_dst(nlroute, dst);
    rtnl_route_set_gateway(nlroute, gateway);
    
    if (rtnl_route_add(data.nl_handle, nlroute, 0) < 0) {
        fprintf(stderr, "rtnl_route_add failed: %s\n", nl_geterror());
    }
    nl_addr_destroy(dst);
    nl_addr_destroy(gateway);
    
}

void routing_table_del(struct routing_table *table, struct msh_route *route)
{
    
}

struct msh_route *routing_table_find(struct routing_table *table,
    struct msh_route *route, int attr_flags)
{
    struct msh_route *entry;
    
    list_for_each_entry(entry, &table->route_list.list, list)
    {
        if(msh_route_compare(route, entry, attr_flags) == 0)
            return entry;
    }
    
    return 0;
}

void routing_table_foreach(struct routing_table *table,
    int (*callback_func)(struct msh_route *, void *), void *data)
{
    
}
    
void routing_table_foreach_filter(struct routing_table *table,
    int (*callback_func)(struct msh_route *, void *), void *data)
{
    
}
