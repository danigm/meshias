#include <netinet/in.h>
#include <netlink/route/link.h>
#include <unistd.h>
#include <sys/types.h>

#include "msh_data.h"
#include "nfqueue.h"

void __msh_data_process_wait_queue_cb(struct alarm_block* alarm, void *qdata)
{
    // Reset the number of rreq sent to zero
    data.num_rreq_sent = 0;
    
    // Set the alarm to call this function again within 1 second
    add_alarm(alarm, 1, 0);
    
    //TODO: Process data.rreq_wait_queue
//     loop
//         // Route not found. Queue the packet and find a route
//         packets_fifo_push(data.packets_queue, id, dest);
//         
//         // Will only try to find a route if we are not already trying to find
//         // one (i.e. when are not waiting response from a RREQ for that route
//         // sent by us)
//         if(!rreq_fifo_waiting_response_for(data.rreq_queue, dest))
//             aodv_find_route(dest, last_kown_dest_seq_num);
}


int msh_data_init(int argc, char **argv)
{
    data.handle = NULL; 
    data.queue = NULL;
    data.netlink_handle = NULL;
    data.nl_handle = NULL; 
    data.max_fd = 0;
    data.num_rreq_sent = 0;
    data.seq_num = 0;
    data.rreq_id = 0;
    
    // Parse args
    if(argc < 2)
    {
        printf("Usage: %s <network interface>\n", argv[0]);
        printf("Example: %s ath0\n", argv[0]);
        exit(1);
    }
    
    // You must be root
    if(getuid() != 0)
    {
        perror("You must be root.\n");
        return ERR_INIT;
    }

    /* Initializing the fd set */
    FD_ZERO(&data.all_fd);

    if(nfqueue_init())
    {
        return ERR_INIT;
    }
    else if(daemon_init())
    {
        return ERR_INIT;
    }
    
    // Allocate a new netlink socket
    data.nl_handle = nl_handle_alloc();
    // Connect to link netlink socket on kernel side
    nl_connect(data.nl_handle, NETLINK_ROUTE);
    
    // Get the interface name
    struct nl_cache *link_cache = rtnl_link_alloc_cache(data.nl_handle);
    struct rtnl_link *link = rtnl_link_get_by_name(link_cache, argv[1]);
    
    if(!link)
    {
        fprintf(stderr, "Error: Couldn't access the interface named %s\n", argv[1]);
        return ERR_INIT;
    }
    
    struct nl_addr *nladdr = rtnl_link_get_addr(link);
    int family = nl_addr_get_family(nladdr);

    /*TODO
    if(nl_addr_get_family(nladdr) != AF_INET)
    {
        fprintf(stderr, "Error: We only support interfaces using IPv4\n");
        return ERR_INIT;
    }
    */

    data.ip_addr.s_addr = nl_addr_get_binary_addr(nladdr);
    data.net_iface = rtnl_link_get_name(link);
    
    rtnl_link_put(link);
    nl_cache_free(link_cache);

    data.routing_table = routing_table_alloc();
    data.rreq_queue = rreq_fifo_alloc();
    data.packets_queue = packets_fifo_alloc();

    init_alarm(&data.rreq_flush_alarm, 0, __msh_data_process_wait_queue_cb);
    //add_alarm(alarm, 1, 0);
    return 1;
}

void msh_data_shutdown()
{
    del_alarm(&data.rreq_flush_alarm);
    routing_table_delete(data.routing_table);
    rreq_fifo_delete(data.rreq_queue);
    packets_fifo_delete(data.packets_queue);

    if(data.queue != NULL)
    {
        printf("unbinding from queue 0\n");
        nfq_destroy_queue(data.queue);
    }

    if(data.handle != NULL)
    {
        printf("closing library handle\n");
        nfq_close(data.handle);
    }
}
