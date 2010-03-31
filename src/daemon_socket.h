#ifndef DAEMON_SOCKET_H
#define DAEMON_SOCKET_H

#include "msh_data.h"

#include "aodv/packet.h"

#include <sys/socket.h>

/* Initialize the aodv's stuff like socket */
int daemon_socket_init();

/* Free reserved memory */
void daemon_socket_shutdown();

/* Function to receive packet and make an answer */
void daemon_socket_receive_packet();

#endif
