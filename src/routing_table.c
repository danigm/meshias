#include <stdlib.h>
#include <netlink/route/rtnl.h>
#include <netlink/route/route.h>
#include "routing_table.h"
#include "msh_data.h"

struct msh_route
{
    struct in_addr dst_ip;
    uint8_t prefix_sz;
    uint32_t dest_seq_num;
    uint16_t flags;
    uint8_t hop_count;
    struct in_addr next_hop;
    uint32_t network_iface;
// TODO
//    uint32_t* precursors_list;
    uint32_t lifetime;
    struct list_head list;
};

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
    
}

void routing_table_flush(uint32_t iface)
{
    
}

void routing_table_add(struct routing_table *table, struct msh_route *route)
{
    // If route exists, don't add it
    // TODO: comparar mejor..
    if(routing_table_find(table, route, 0b1111111111))
        return;
    
    struct rtnl_route *nlroute;
    
    rtnl_route_set_oif(nlroute, msh_route_get_net_iface(route));
    rtnl_route_set_table(nlroute, rtnl_route_str2table("main"));
    
    if (rtnl_route_add(data.nl_handle, nlroute, 0) < 0) {
        fprintf(stderr, "rtnl_route_add failed: %s\n", nl_geterror());
    }
    
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
