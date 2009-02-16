#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>                  /* for isprint */
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>        /* for NF_ACCEPT */

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
    while(1)
    {
        // BUG: error: ‘timer’ no se declaró aquí (primer uso en esta función)
        // BUG: error: tipo incompatible para el argumento 2 de ‘select’
        //while( select(data.max_fd, data.all_fd, NULL, NULL, timer) > 0 )
        {
            /* Check for new packets */
            //if( FD_ISSET(data.nfqueue_fd, &data.all_fd) )
            {
                nfqueue_receive_packets();
            }
        }
    }
    
    // close everything safely
    msh_data_shutdown();
    
    return 0;
}
