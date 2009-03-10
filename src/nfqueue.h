#ifndef _NFQUEUE_H_
#define _NFQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>        /* for NF_ACCEPT */

/**
 * Initializes all netfilter related handles and opens the socket
 * NFQUEUE number 0 socket.
 *
 * @return 0 if success or less than zero otherwise
 */
int nfqueue_init();

/**
 * Receive all the packets waiting in the NFQUEUE number 0. We use this
 * to effectively sniff trafic. This function should only be called in the
 * main loop. The packets will be handled to nfq_handle_packet() which in
 * turn will call to @see manage_packet().
 */
void nfqueue_receive_packets();

/**
 * Prints all kind of information attached to a packet: mark,  timestamp,
 * data, indev, outdev. protocol, id, hook. 
 *
 * @returns packet id
 */
static uint32_t nfqueue_packet_print(struct nfq_data *packet);

/**
 * @returns the id of a packet.
 */
static uint32_t nfqueue_packet_get_id(struct nfq_data *packet);

/**
 * @returns the dest ip of a packet.
 */
static struct in_addr nfqueue_packet_get_dest(struct nfq_data *packet);

/**
 * @returns the orig ip of a packet.
 */
static struct in_addr nfqueue_packet_get_orig(struct nfq_data *packet);

/**
 * Called by @see nfq_handle_packet() which in turns gets called by
 * @see nfqueue_receive_packets(). This function inspects each packet
 * and checks if we have a route for the packet and also checks for
 * AODV incoming traffic (UDP port 654).
 */
static int manage_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data);

#endif
