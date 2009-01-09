#ifndef MSH_DATA_H
#define MSH_DATA_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "packet_queue.h"
#include "routes_cache.h"
#include "alarm.h"

struct msh_data_t
{
    struct packet_queue* wait_packets;
    struct nfq_handle *handle;
    struct nfq_q_handle *queue;
    struct nfnl_handle *netlink_handle;

    /* Sockets */
    int nfqueue_fd;
    fd_set all_fd;
    int max_fd;

    /* Alarm block */
    alarm_block alarms;
};

extern struct msh_data_t data;

#define DATA(x) data.x

int msh_init_data();
void msh_shutdown_data();

#endif
