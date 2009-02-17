#ifndef MSH_DATA_H
#define MSH_DATA_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include "libnetfilter_queue/libnetfilter_queue.h"
#include "alarm/alarm.h"

#include "common.h"

struct msh_data_t
{

    // needed for libnetfilter_queue
    struct nfq_handle *handle;
    struct nfq_q_handle *queue;
    struct nfnl_handle *netlink_handle;
    
    // Needed for libnl
    struct nl_handle *nl_handle;
    
    // Here we buffer the packets we don't have a route for
    //struct packets_queue* wait_queue;
    
    // Here we buffer the RREQ in order to avoid loops
    // struct rreq_fifo* rreq_buffer;
    
    // Route table. Plays a major role in AODV
    // struct route_cache* route_table;

    // Sockets
    int nfqueue_fd;
    // This is the socket 654 UDP. It's listening to new packet of the
    // aodv protocol. It also used to send packet of aodv protocol.
    int daemon_fd;
    fd_set all_fd;
    int max_fd;

    // Alarm block
    //BUG: This makes no sense. There should be an alarm block per timeout,
    // with diffenrent function+data to be called
    // struct alarm_block alarm;
};

extern struct msh_data_t data;

int msh_data_init(int argc, char **argv);
void msh_data_shutdown();

#endif
