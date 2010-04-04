#ifndef CONFIGURATION_PARAMETERS_H
#define CONFIGURATION_PARAMETERS_H

#define AODV_UDP_PORT 654

#include <stdint.h>

/**
 * @name Common vars
 * Common vars have been defined as simple functions (@see rfc3561.txt)
 * @{
 */
uint32_t ACTIVE_ROUTE_TIMEOUT(); // Milliseconds

uint32_t ALLOWED_HELLO_LOSS();

uint32_t RREQ_RETRIES();

uint32_t HELLO_INTERVAL(); // Milliseconds

uint32_t LOCAL_ADD_TTL();

uint32_t NET_DIAMETER();

uint32_t RERR_RATELIMIT();

uint32_t RREQ_RATELIMIT();

uint32_t TIMEOUT_BUFFER();

/* The value of TTL when you create a new RREQ packet */
uint32_t TTL_START();

uint32_t TTL_INCREMENT();

uint32_t TTL_THRESHOLD();

uint32_t NODE_TRAVERSAL_TIME(); // Milliseconds

uint32_t NEXT_HOP_WAIT();

uint32_t MAX_REPAIR_TTL();

uint32_t MY_ROUTE_TIMEOUT();

uint32_t NET_TRAVERSAL_TIME();

uint32_t BLACKLIST_TIMEOUT();

uint32_t PATH_DISCOVERY_TIME();

uint32_t DELETE_PERIOD();

uint32_t MIN_REPAIR_TTL();

uint32_t TTL_VALUE();

uint32_t RING_TRAVERSAL_TIME();

uint32_t binary_exponential_backoff_time(int8_t prev_tries);

uint32_t expanding_ring_search_ttl(uint8_t hop_count, int8_t prev_tries);

// See RFC 3561 page 17
uint32_t minimal_lifetime(uint8_t hop_count);

/** @} */

#endif
