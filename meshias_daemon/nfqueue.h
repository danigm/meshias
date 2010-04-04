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

void nfqueue_shutdown();

/**
 * Receive all the packets waiting in the NFQUEUE number 0. We use this
 * to effectively sniff trafic. This function should only be called in the
 * main loop. The packets will be handled to nfq_handle_packet() which in
 * turn will call to @see manage_packet().
 */
void nfqueue_receive_packet();

/**
 * Prints all kind of information attached to a packet: mark,  timestamp,
 * data, indev, outdev. protocol, id, hook.
 *
 * @returns packet id
 */
uint32_t nfqueue_packet_print(struct nfq_data *packet);

/**
 * @returns the id of a packet.
 */
uint32_t nfqueue_packet_get_id(struct nfq_data *packet);

/**
 * @returns the hook of a packet.
 */
uint32_t nfqueue_packet_get_hook(struct nfq_data *packet);

/**
 * @returns the dest ip of a packet.
 */
struct in_addr nfqueue_packet_get_dest(struct nfq_data *packet);

/**
 * @returns the orig ip of a packet.
 */
struct in_addr nfqueue_packet_get_orig(struct nfq_data *packet);

/**
 * @returns whether a given IP packet is UDP and comes from AODV port.
 */
int nfqueue_packet_is_aodv(struct nfq_data *packet);

#endif
