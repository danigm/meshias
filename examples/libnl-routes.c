// libnl headers
#include <stdio.h>
#include <netlink/utils.h>
#include <netlink/route/link.h>
#include <netlink/route/rtnl.h>
// BUG: it breaks if you don't include rtnl.h first !!
#include <netlink/route/route.h>
#include <netlink/netlink.h>
#include <netlink/cache.h>
#include <netlink/addr.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>



void print_link(struct nl_object* obj, void *arg)
{
    int *item = (int *)arg;
    struct rtnl_link* link = (struct rtnl_link*)obj;
    printf("Link %d name: %s\n", (*item)++, rtnl_link_get_name(link));
}


void print_route(struct nl_object* obj, void *arg)
{
    int *item = (int *)arg;
    struct rtnl_route* route = (struct rtnl_route*)obj;
    char buf[128];
    
    struct nl_addr *addr = rtnl_route_get_dst(route);
    
    if(addr != NULL)
    {
        if(rtnl_route_get_table(route) == 254)
        {
        printf("Route %d \ttable %d\tdst %s\t\tdst len %d\n", *item,
                rtnl_route_get_table(route),
                nl_addr2str(addr, buf, sizeof(buf)),
                rtnl_route_get_dst_len(route));
        }
    } else
        printf("Route %d\n", *item);
    
    (*item)++;
    struct nl_dump_params params;
    params.dp_type = NL_DUMP_FULL;
//     route_dump_full(route, &params);
}

int main(int argc, char **argv)
{
    struct nl_handle *sock;
    // Allocate a new netlink socket
    sock = nl_handle_alloc();

    // Connect to link netlink socket on kernel side
    nl_connect(sock, NETLINK_ROUTE);
    
    // The first step is to retrieve a list of all available interfaces within
    // the kernel and put them into a cache.
    struct nl_cache *link_cache = rtnl_link_alloc_cache(sock);

    // In a second step, we iterate the link interfaces and print its names.
    printf("Link cache (%d ifaces):\n", nl_cache_nitems(link_cache));
    int item = 0;
    nl_cache_foreach(link_cache, print_link, (void *)&item);
    
    struct nl_cache *route_cache = rtnl_route_alloc_cache(sock);
    printf("Route cache (%d ifaces):\n", nl_cache_nitems(route_cache));
    
    item = 0;
    nl_cache_foreach(route_cache, print_route, (void *)&item);
    
    // Free the mallocs
    nl_cache_free(link_cache);
    nl_cache_free(route_cache);
    nl_handle_destroy(sock);
	
    return 0;
}

