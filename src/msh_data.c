#include "msh_data.h"
#include "nfqueue.h"

int msh_data_init(int argc, char **argv)
{
    data.handle = NULL; 
    data.queue = NULL;
    data.netlink_handle = NULL;
    data.nl_handle = NULL; 
    
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
    
    /* Initializing alarm */
    //TODO: find how the  heck this works
//     init_alarm(&data.alarms, NULL, do_overrun_alarm);
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
