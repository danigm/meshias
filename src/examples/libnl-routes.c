// libnl headers
#include <stdio.h>
#include <netlink/utils.h>
#include <netlink/route/link.h>
#include <netlink/route/addr.h>
#include <netlink/route/rtnl.h>
// BUG: it breaks if you don't include rtnl.h first !!
#include <netlink/route/route.h>
#include <netlink/netlink.h>
#include <netlink/cache.h>
#include <netlink/addr.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

struct nl_addr* in_addr2nl_addr(struct in_addr *addr, uint8_t prefix_sz)
{
    char buf[256];
    sprintf(buf, "%s/%d", (char *)inet_ntoa(*addr), (int)prefix_sz);
    
    return nl_addr_parse(buf, AF_INET);
}

void print_link(struct nl_object* obj, void *arg)
{
    int *item = (int *)arg;
    struct rtnl_link* link = (struct rtnl_link*)obj;
    printf("Link %d name: %s\n", (*item)++, rtnl_link_get_name(link));
    
    struct nl_dump_params dp = {
        .dp_type = NL_DUMP_FULL,
        .dp_fd = stdout,
        .dp_dump_msgtype = 1,
    };

    nl_object_dump(obj, &dp);
}


void print_route(struct nl_object* obj, void *arg)
{
    int *item = (int *)arg;
    struct rtnl_route* route = (struct rtnl_route*)obj;
    char buf[128];
    
    struct nl_addr *addr = rtnl_route_get_dst(route);
    
    if(addr != NULL && rtnl_route_get_family(route) == 2)
    {
        struct in_addr *inaddr = (struct in_addr*)malloc(sizeof(struct in_addr));
        inaddr = nl_addr_get_binary_addr(addr);
        printf("1: Route %s\n", inet_ntoa(*inaddr));
        
        printf("2: Route %d \ttable %d\tdst %s\t\tdst len %d family %d\n",
            *item,
            rtnl_route_get_table(route),
            nl_addr2str(addr, buf, sizeof(buf)),
            rtnl_route_get_dst_len(route),
            rtnl_route_get_family(route));
        struct nl_addr *my_nl_addr = in_addr2nl_addr(inaddr, rtnl_route_get_dst_len(route));
        
        printf("3: Route dst %s\t\tdst len %d\n",
            nl_addr2str(my_nl_addr, buf, sizeof(buf)),
            nl_addr_get_prefixlen(my_nl_addr));
    } else
        printf("Route %d\n", *item);
    
    (*item)++;
    
//     struct nl_dump_params dp = {
//         .dp_type = NL_DUMP_FULL,
//         .dp_fd = stdout,
//         .dp_dump_msgtype = 1,
//     };
// 
//     if(rtnl_route_get_table(route) == 254)
//     	nl_object_dump(obj, &dp);
// 
//     route_dump_full(route, &params);
}

void print_addr(struct nl_object* obj, void *arg)
{
    int ifindex = *(int*)arg;
    struct rtnl_addr* addr = (struct rtnl_addr*)obj;
    struct nl_dump_params dp = {
        .dp_type = NL_DUMP_FULL,
        .dp_fd = stdout,
        .dp_dump_msgtype = 1,
    };
    
    nl_object_dump(obj, &dp);
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
    nl_cache_mngt_provide(link_cache);
    
    char ath0[] = "ath0";
    char *ifname = (argc > 1) ? argv[1] : ath0;
    int ifindex = rtnl_link_name2i(link_cache, ifname);

     printf("net iface: %d: %s", ifindex, ifname);
    // In a second step, we iterate the link interfaces and print its names.
    printf("Link cache (%d ifaces):\n", nl_cache_nitems(link_cache));
//     int item = 0;
//     nl_cache_foreach(link_cache, print_link, (void *)&item);
//     
//     struct nl_cache *route_cache = rtnl_route_alloc_cache(sock);
//     
//     printf("Route cache (%d routes):\n", nl_cache_nitems(route_cache));
//     item = 0;
//     nl_cache_foreach(route_cache, print_route, (void *)&item);
//     rtnl_route_set_gateway(nlroute, nl_addr_parse("127.0.0.1", AF_INET));
//     if (rtnl_route_del(sock, nlroute, 0) < 0)
//     {
//         fprintf(stderr, "rtnl_route_add failed: %s\n", nl_geterror());
//     }
    
    struct nl_cache* addr_cache = rtnl_addr_alloc_cache(sock);
    
    struct rtnl_addr* filter = rtnl_addr_alloc();
    rtnl_addr_set_ifindex(filter, ifindex);
    rtnl_addr_set_family(filter, AF_INET);
    
    nl_cache_foreach_filter(addr_cache, (struct nl_object *)filter, print_addr,
        &ifindex);
    
//     struct nl_object* obj = nl_cache_search(addr_cache, (struct nl_object*)needle);
//     nl_object_dump(obj, &dp);
//     nl_cache_dump_filter(addr_cache, &dp, (struct nl_object*)needle);
    
    
    
    
    
    struct rtnl_route *nlroute = rtnl_route_alloc();
    char buf[256];
    sprintf(buf, "192.168.0.1/0");
    struct nl_addr *addr1 = nl_addr_parse(buf, AF_INET);
    sprintf(buf, "192.168.0.0/0");
    struct nl_addr *addr2 = nl_addr_parse(buf, AF_INET);
    
    printf("addr1: dst: %s\n", nl_addr2str(addr1, buf, 256));
    rtnl_route_set_oif(nlroute, ifindex);
    rtnl_route_set_family(nlroute, AF_INET);
    rtnl_route_set_scope(nlroute, RT_SCOPE_LINK);
    rtnl_route_set_dst(nlroute, addr1);
//     rtnl_route_set_gateway(nlroute, addr1);
    
    if (rtnl_route_add(sock, nlroute, 0) < 0)
    {
        printf("rtnl_route_add failed: %s\n", nl_geterror());
    }
    
    // Free the mallocs
    nl_cache_free(addr_cache);
    nl_cache_free(link_cache);
//     nl_cache_free(route_cache);
    nl_handle_destroy(sock);
    
    return 0;
}
