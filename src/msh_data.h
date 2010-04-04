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

#include "utils.h"
#include "routing_table.h"
#include "rreq_fifo.h"
#include "packets_fifo.h"
#include "sockets/fds.h"

/**
 * Errors and their corresponding error numbers
 * @ingroup Common
 */
#define ERR_INIT -1
#define ERR_SEND -1

/**
 * Errors and their corresponding error numbers
 * @ingroup Common
 */
#define ERR_INIT -1
#define ERR_SEND -1

/**
 * This struct contains all the global data necesary for program execution.
 */
struct msh_data_t {
    /** Bool to finish the program execution. */
    int end;
    // comm socket
    uint32_t comm_fd;

    /** Struct needed to handle nfq. */
    struct nfq_handle *handle;
    /** Struct needed to handle nfq.
     * It represents the queue where we capture all the packets.  */
    struct nfq_q_handle *queue;
    /** Struct needed to hanle nfq. */
    struct nfnl_handle *netlink_handle;
    /** Netfilter_queue socket */
    uint32_t nfqueue_fd;

    /** Struct needed to handle libnl. */
    struct nl_handle *nl_handle;
    /** This integer represent the interface managed by meshias in libnl. */
    uint32_t net_iface;

    // Here we buffer the packets we don't have a route for
    struct packets_fifo* packets_queue;

    /** Here we buffer the RREQ in order to avoid loops. */
    struct rreq_fifo* rreq_queue;

    /** Route table. Plays a major role in AODV. */
    struct routing_table* routing_table;

    /** This is the socket 654 UDP. It's listening to new packet of the
     * aodv protocol. It also used to send packet of aodv protocol.
     */
    uint32_t daemon_fd;

    /** The set of sockets to manage when data comes to any socket */
    struct fds *fds;

    /** This is the IP of the interface managed by meshias. */
    struct in_addr ip_addr;
    /** This is the broadcast address */
    struct in_addr broadcast_addr;

    /** Our sequence number. Plays  major role in AODV. */
    uint32_t seq_num;

    /** The ID of the last RREQ we have sent. */
    uint32_t rreq_id;

    /** There's a maximum number of RREQs that can be sent per second. */
    uint32_t num_rreq_sent;

    /** This alarm will be used to reset the field num_rreq_sent to zero every
     * second, and to send the RREQs waiting for being sent.*/
    struct alarm_block rreq_flush_alarm;

    //TODO
    //struct rreq_wait_fifo *rreq_wait_queue;
};

/** The global struct. */
extern struct msh_data_t data;

/** Create the initialize the struct with the initial content. */
int msh_data_init(int argc, char **argv);

/** Free the mallocs. */
void msh_data_shutdown();

#endif
