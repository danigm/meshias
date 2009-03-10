#ifndef AODV_DAEMON_H
#define AODV_DAEMON_H

#include "msh_data.h"
#include "aodv_packet.h"
#include "common.h"

#include <sys/socket.h>

/* Initialize the aodv's stuff like socket */
int daemon_init();

/* Free reserved memory */
void daemon_shutdown();

/* Function to receive packets and make an answer */
void daemon_receive_packets();

#endif
