#include <libnetfilter_queue/libnetfilter_queue.h>
#include <libnetfilter_queue/libnetfilter_queue_headers.h>

#include "msh_data.h"

int msh_init_data()
{
    data->wait_packets = NULL;
    data->route_cache = NULL;
    data->handle = NULL;
    data->queue = NULL;
    data->netlink_handle = NULL;

    /* Initializing the fd set */
    FD_ZERO(&all_fd);

    if(nfqueue_init());
    else if(aodv_init());
    else if(nl_init());

    /* Initializing alarm */
    init_alarm(data->alarms, NULL, do_overrun_alarm);
}

void msh_shutdown_data(struct data_t* data)
{
    shutdown_packet_queue(data->wait_packets);
    shtudown_cache(data->route_cache);

    if(data->queue != NULL)
    {
        printf("unbinding from queue 0\n");
        nfq_destroy_queue(data->queue);
    }

    if(data->handle != NULL)
    {
    #ifdef INSANE
        /* normally, applications SHOULD NOT issue this command, since
        * it detaches other programs/sockets from AF_INET, too ! */
        printf("unbinding from AF_INET\n");
        nfq_unbind_pf(data->handle, AF_INET);
    #endif
        printf("closing library handle\n");
        nfq_close(data->handle);
    }
}
