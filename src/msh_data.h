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
#include "utils.h"
#include "routing_table.h"
#include "rreq_fifo.h"
#include "packets_fifo.h"
#include "fds.h"
#include "local.h"

struct msh_data_t {
    int end;
    // unix socket
    struct local_server local_server;

    // needed for libnetfilter_queue
    struct nfq_handle *handle;
    struct nfq_q_handle *queue;
    struct nfnl_handle *netlink_handle;

    // Needed for libnl
    struct nl_handle *nl_handle;

    // Here we buffer the packets we don't have a route for
    struct packets_fifo* packets_queue;

    // Here we buffer the RREQ in order to avoid loops
    struct rreq_fifo* rreq_queue;

    // Route table. Plays a major role in AODV
    struct routing_table* routing_table;

    // Sockets
    uint32_t nfqueue_fd;
    // This is the socket 654 UDP. It's listening to new packet of the
    // aodv protocol. It also used to send packet of aodv protocol.
    uint32_t daemon_fd;
    struct fds *fds;

    uint32_t net_iface;
    struct in_addr ip_addr;
    struct in_addr broadcast_addr;

    // Our own sequence number;
    uint32_t seq_num;

    // The ID of the last RREQ we have sent
    uint32_t rreq_id;

    // There's a maximum number of RREQs that can be sent per second
    uint32_t num_rreq_sent;

    // This alarm will be used to reset the field num_rreq_sent to zero every
    // second, and to send the RREQs waiting for being sent.
    struct alarm_block rreq_flush_alarm;
    //TODO
    //struct rreq_wait_fifo *rreq_wait_queue;
};

extern struct msh_data_t data;

int msh_data_init(int argc, char **argv);
void msh_data_shutdown();

#endif
