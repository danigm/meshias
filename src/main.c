#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>                  /* for isprint */
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>        /* for NF_ACCEPT */
#include <sys/select.h>

#include "libnetfilter_queue/libnetfilter_queue.h"
#include "libnetfilter_queue/libnetfilter_queue_headers.h"

#include "msh_data.h"

struct msh_data_t data;

int main(int argc, char **argv)
{
    int errno;
    
    // Initialize all needed data GET IP
    if((errno = msh_data_init(argc, argv)) < 0)
    {
        return errno;
    }
    
    // Main loop
    // TODO: Here we should capture signals sent to the app
    
    /**
     * next is a var which stores the time left for the next run. We'll use it
     * indirectly with next_run. If there's no next alarm, next_run = NULL.
     */
    struct timeval next;
    struct timeval* next_run = NULL;
    
    while(1)
    {
        // We'll wait for new data in our sockets until a new alarm times out
        while( select(data.max_fd, &data.all_fd, NULL, NULL, next_run) > 0 )
        {
            /* Check for new packets */
            if( FD_ISSET(data.nfqueue_fd, &data.all_fd) )
            {
                nfqueue_receive_packets();
            }
            
            // The code above has potentially added some new alarms which means
            // we need to recalculate which is the next alarm to be called
            next_run = get_next_alarm_run(next_run);
        }
        process_alarms(next_run);
    }
    
    // close everything safely
    msh_data_shutdown();
    
    return 0;
}
