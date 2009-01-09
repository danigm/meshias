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

int main(int argc, char **argv)
{
    int received;
    char buf[4096] __attribute__ ((aligned));

    if(init_data()==ERROR)
    {
    }

    /* Main loop */
    /* Here we could receive signals */
    while(1)
    {
        while(select(DATA(max_fd),DATA(all_fd),NULL,NULL,timer)>0)
        {
            /*If is received a new packet to be queued */
            if(FD_ISSET(DATA(nfqueue_fd),&DATA(all_fd)))
            {
                nfqueue_received();
            }
            /* Here we're looking for others socket which could have
             * been modified
             */
            if(FD_ISSET(DATA(aodv_fd),&DATA(all_fd)))
            {
                aodv_received();
            }
            if(FD_ISSET(DATA(nl_fd),&DATA(all_fd)))
            {
                nl_received();
            }
        }
        /* Here we're checking if the alarm are finished
         *
         *
         */
        check_alarm();
    }

    shutdown_data();
    return 0;
}
