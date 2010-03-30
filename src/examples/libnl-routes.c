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

struct nl_addr* in_addr2nl_addr(struct in_addr *addr, uint8_t prefix_sz) {
    char buf[256];
    sprintf(buf, "%s/%d", (char *)inet_ntoa(*addr), (int)prefix_sz);

    return nl_addr_parse(buf, AF_INET);
}

int main(int argc, char **argv)
{
    struct nl_handle *sock;
    // Allocate a new netlink socket
    sock = nl_handle_alloc();
    // Connect to link netlink socket on kernel side
    nl_connect(sock, NETLINK_ROUTE);

    if (argc < 4) {
        printf("%s <iface> <dst> gateway\n", argv[0]);
        printf("Example: %s ath0 192.168.2.1\n", argv[0]);
        exit(1);
    }

    // The first step is to retrieve a list of all available interfaces within
    // the kernel and put them into a cache.
    struct nl_cache *link_cache = rtnl_link_alloc_cache(sock);
    nl_cache_mngt_provide(link_cache);

    char *ifname = argv[1];
    int ifindex = rtnl_link_name2i(link_cache, ifname);

    struct rtnl_route *nlroute = rtnl_route_alloc();
    struct nl_addr *dst = nl_addr_parse(argv[2], AF_INET);
    rtnl_route_set_oif(nlroute, ifindex);
    rtnl_route_set_dst(nlroute, dst);
    struct nl_addr *gateway = nl_addr_parse(argv[3], AF_INET);
    char buf[200];
    printf("addr: %s\n", nl_addr2str(gateway, buf, sizeof(buf)));
//     rtnl_route_set_family(nlroute, AF_INET);
//     rtnl_route_set_table (nlroute, RT_TABLE_MAIN);
    rtnl_route_set_gateway(nlroute, gateway);
    rtnl_route_set_scope(nlroute, RT_SCOPE_UNIVERSE);
//     nl_addr_put(gateway);

//     struct rtnl_route *nlroute2 = rtnl_route_alloc();
//     rtnl_route_set_oif(nlroute2, ifindex);
//     rtnl_route_set_dst(nlroute2, gateway);
//     if (rtnl_route_add(sock, nlroute2, 0) < 0)
//     {
//         fprintf(stderr, "rtnl_route_add (for gateway) failed: %s\n", nl_geterror());
//     }

//     struct rtnl_nexthop * route_nexthop = rtnl_route_nh_alloc();
//     rtnl_route_nh_set_gateway(route_nexthop, dst);
//     rtnl_route_add_nexthop(nlroute, route_nexthop);
    if (rtnl_route_add(sock, nlroute, 0) < 0) {
        fprintf(stderr, "rtnl_route_add failed: %s\n", nl_geterror());
    }

    // Free the mallocs
//     nl_cache_free(addr_cache);
    nl_cache_free(link_cache);
//     nl_cache_free(route_cache);
    nl_handle_destroy(sock);

    return 0;
}
