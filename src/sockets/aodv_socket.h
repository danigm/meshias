#ifndef DAEMON_SOCKET_H
#define DAEMON_SOCKET_H

#include "msh_data.h"

#include "aodv/packet.h"

#include <sys/socket.h>

/**
 * Initialize the aodv socket, which receives the AODV protocol packets.
 */
int aodv_socket_init();

/**
 * Free reserved memory for the daemon socket.
 */
void aodv_socket_shutdown();

/**
 * Receives an aodv packet and make an answer
 *
 * @note in order for this function to be non-blocking, this function should only be called when you
 * are sure that there's a packet waiting. This function is only called by meshias.c main() when
 * select() states that the daemon socket is not empty.
 */
void aodv_socket_receive_packet();

#endif
