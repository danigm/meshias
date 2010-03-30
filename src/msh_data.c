// #include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netlink/cache.h>
#include <netlink/route/link.h>
#include <netlink/route/addr.h>
#include <unistd.h>
#include <sys/types.h>

#include "msh_data.h"
#include "nfqueue.h"

void __msh_data_process_wait_queue_cb(struct alarm_block* alarm, void *qdata)
{
    puts("__msh_data_process_wait_queue_cb called");
    // Reset the number of rreq sent to zero
    data.num_rreq_sent = 0;

    // Set the alarm to call this function again within 1 second
    add_alarm(alarm, 1, 0);

    //TODO: Process data.rreq_wait_queue
    // loop
    // Route not found. Queue the packet and find a route
    // packets_fifo_push(data.packets_queue, id, dest);
    //
    // Will only try to find a route if we are not already trying to find
    // one (i.e. when are not waiting response from a RREQ for that route
    // sent by us)
    // if(!rreq_fifo_waiting_response_for(data.rreq_queue, dest))
    // aodv_find_route(dest, last_kown_dest_seq_num);
}

void __init_addr(struct nl_object* obj, void *arg)
{
    struct rtnl_addr* addr = (struct rtnl_addr*)obj;

    struct nl_dump_params dp = {
        .dp_type = NL_DUMP_FULL,
        .dp_fd = stdout,
        .dp_dump_msgtype = 1,
    };

    struct nl_addr *local = rtnl_addr_get_local(addr);
    memcpy(&data.ip_addr, nl_addr_get_binary_addr(local), sizeof(uint32_t));
    data.ip_addr.s_addr = ntohl(data.ip_addr.s_addr);

    struct nl_addr *broadcast = rtnl_addr_get_broadcast(addr);
    memcpy(&data.broadcast_addr, nl_addr_get_binary_addr(broadcast), sizeof(uint32_t));
    data.broadcast_addr.s_addr = ntohl(data.broadcast_addr.s_addr);

    printf("local %s\n", inet_htoa(data.ip_addr));
    printf("broadcast %s\n", inet_htoa(data.broadcast_addr));
}

int msh_data_init(int argc, char **argv)
{
    memset(&data, 0, sizeof(data));

    // Parse args
    if (argc < 2) {
        printf("Usage: %s <network interface>\n", argv[0]);
        printf("Example: %s ath0\n", argv[0]);
        exit(1);
    }

    // You must be root
    if (getuid() != 0) {
        perror("You must be root.\n");
        return ERR_INIT;
    }

    stats_init();

    if ((data.fds = create_fds()) == NULL) {
        perror("Error: fds");
        return ERR_INIT;
    } else if (nfqueue_init()) {
        return ERR_INIT;
    } else if (daemon_init()) {
        return ERR_INIT;
    } else if (unix_interface_init()) {
        return ERR_INIT;
    }

    // Allocate a new netlink socket
    data.nl_handle = nl_handle_alloc();
    // Connect to link netlink socket on kernel side
    nl_connect(data.nl_handle, NETLINK_ROUTE);

    // Get the interface name
    struct nl_cache *link_cache = rtnl_link_alloc_cache(data.nl_handle);
    struct rtnl_link *link = rtnl_link_get_by_name(link_cache, argv[1]);

    if (!link) {
        fprintf(stderr, "Error: Couldn't access the interface named %s\n", argv[1]);
        return ERR_INIT;
    }

    data.net_iface = rtnl_link_name2i(link_cache, rtnl_link_get_name(link));

//     rtnl_link_put(link);
    nl_cache_free(link_cache);

    // Get the IP address and netmask of the interface
    struct nl_cache* addr_cache = rtnl_addr_alloc_cache(data.nl_handle);
    struct rtnl_addr* filter = rtnl_addr_alloc();
    rtnl_addr_set_ifindex(filter, data.net_iface);
    rtnl_addr_set_family(filter, AF_INET);

    // FIXME PETA
    nl_cache_foreach_filter(addr_cache, (struct nl_object *)filter, __init_addr, 0);
    rtnl_addr_put(filter);

    nl_cache_free(addr_cache);

    data.routing_table = routing_table_alloc();

    data.rreq_queue = rreq_fifo_alloc();
    data.packets_queue = packets_fifo_alloc();

    init_alarm(&data.rreq_flush_alarm, 0, __msh_data_process_wait_queue_cb);
//     add_alarm(&data.rreq_flush_alarm, 1, 0);
    return 1;
}

void msh_data_shutdown()
{
    del_alarm(&data.rreq_flush_alarm);
    routing_table_delete(data.routing_table);
    rreq_fifo_delete(data.rreq_queue);
    packets_fifo_delete(data.packets_queue);

    destroy_fds(data.fds);
    nfqueue_shutdown();
    daemon_shutdown();
    unix_interface_shutdown();
    debug(1, "Freed all memory");
}
