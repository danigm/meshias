
#include <netlink/route/link.h>
#include "msh_data.h"
#include "nfqueue.h"

int msh_data_init(int argc, char **argv)
{
    data.handle = NULL; 
    data.queue = NULL;
    data.netlink_handle = NULL;
    data.nl_handle = NULL; 
    data.max_fd = 0;
    
    // Parse args
    if(argc < 2)
    {
        printf("Usage: %s <network interface>\n", argv[0]);
        printf("Example: %s ath0\n", argv[0]);
        exit(1);
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
    data.net_iface = rtnl_link_name2i(link_cache, argv[1]);
    nl_cache_free(link_cache);
    
    //TODO:Max fd
}

void msh_data_shutdown()
{
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
