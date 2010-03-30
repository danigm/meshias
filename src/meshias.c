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
#include "statistics.h"

struct msh_data_t data;
struct statistics_t stats;

int main(int argc, char **argv)
{
    int errno;

    /**
     * next is a var which stores the time left for the next run. We'll use it
     * indirectly with next_run. If there's no next alarm, next_run = NULL.
     */
    struct timeval next;
    struct timeval* next_run = NULL;

    debug(1, "Initializing");

    // Initialize all needed data GET IP
    if ((errno = msh_data_init(argc, argv)) < 0) {
        return errno;
    }

    debug(1, "Initilization done");
    // Main loop
    // TODO: Here we should capture signals sent to the app

    while (!data.end) {
        if (next_run) {
            printf("wile(1) next_run %d %d\n", next_run->tv_sec, next_run->tv_usec);
        } else {
            puts("while1 NULL");
        }

        //This is needed because of yes
        FD_SET(data.nfqueue_fd, &data.fds->readfds);
        FD_SET(data.daemon_fd, &data.fds->readfds);
        FD_SET(data.comm_fd, &data.fds->readfds);

        //TODO: BUG, when no alarm is left, the select never ends!
        // We'll wait for new data in our sockets until a new alarm times out
        while (!data.end && select(data.fds->maxfd + 1,
                                   &data.fds->readfds, NULL, NULL, next_run) > 0) {
            puts("select");

            /* Check for new packets */
            if (FD_ISSET(data.nfqueue_fd, &data.fds->readfds)) {
                printf("A packet was captured by the nfqueue");
                nfqueue_receive_packets();
            }

            if (FD_ISSET(data.daemon_fd, &data.fds->readfds)) {
                debug(1, "An AODV packet was received by the daemon.");
                daemon_receive_packets();
            }

            if( FD_ISSET(data.comm_fd, &data.fds->readfds) )
            {
                debug(1,"A command was received by the comm socket.");
                comm_interface_receive_packets();
            }

            /*
             * The code above has potentially added some new alarms which means
             * we need to recalculate which is the next alarm to be called
             */
            next_run = get_next_alarm_run(&next);

            if (next_run)
                printf("next_run: %d\n", get_alarm_time(next_run->tv_sec,
                                                        next_run->tv_usec));

            //This is needed because of yes
            FD_SET(data.nfqueue_fd, &data.fds->readfds);
            FD_SET(data.daemon_fd, &data.fds->readfds);
            FD_SET(data.comm_fd, &data.fds->readfds);
        }

        next_run = get_next_alarm_run(&next);
        process_alarms(&next);
    }

    debug(1, "Exiting from the mainloop");

    // close everything safely
    msh_data_shutdown();

    return 0;
}
