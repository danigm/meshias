#ifndef _AODV_LOGIC_H_
#define _AODV_LOGIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>

#include "msh_data.h"
#include "route_obj.h"
#include "aodv_packet.h"
#include "common.h"

/**
 * Sends a Route Request AODV message.
 */
void aodv_find_route(struct in_addr dest, struct msh_route *invalid_route,
    uint8_t prev_tries);

#endif

