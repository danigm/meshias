#include "common.h"


uint32_t ACTIVE_ROUTE_TIMEOUT() {
    return 3000;
}

uint32_t ALLOWED_HELLO_LOSS() {
    return 2;
}

uint32_t RREQ_RETRIES() {
    return 2;
}

uint32_t HELLO_INTERVAL() {
    uint32_t mark;
    return 1000;
}

uint32_t LOCAL_ADD_TTL() {
    return 2;
}

uint32_t NET_DIAMETER() {
    return 35;
}

uint32_t RERR_RATELIMIT() {
    return 10;
}

uint32_t RREQ_RATELIMIT() {
    return 10;
}

uint32_t TIMEOUT_BUFFER() {
    return 2;
}

uint32_t TTL_START() {
    return 1;
}

uint32_t TTL_INCREMENT() {
    return 2;
}

uint32_t TTL_THRESHOLD() {
    return 7;
}

uint32_t NODE_TRAVERSAL_TIME() {
    return 40;
}

uint32_t NEXT_HOP_WAIT() {
    return NODE_TRAVERSAL_TIME() + 10;
}

uint32_t MAX_REPAIR_TTL() {
    return 0.3 * NET_DIAMETER();
}

uint32_t MY_ROUTE_TIMEOUT() {
    return 2 * ACTIVE_ROUTE_TIMEOUT();
}

uint32_t NET_TRAVERSAL_TIME() {
    return 2 * NODE_TRAVERSAL_TIME() * NET_DIAMETER();
}

uint32_t BLACKLIST_TIMEOUT() {
    return RREQ_RETRIES() * NET_TRAVERSAL_TIME();
}

uint32_t PATH_DISCOVERY_TIME() {
    return 2 * NET_TRAVERSAL_TIME();
}

uint32_t DELETE_PERIOD() {
    //TODO: see page 32 of RFC 3561 for other implementations
    return ALLOWED_HELLO_LOSS() * HELLO_INTERVAL();
}

uint32_t MIN_REPAIR_TTL() {
    return 0; // see note below
}

uint32_t TTL_VALUE() {
    return 0; // see note below
}

uint32_t RING_TRAVERSAL_TIME() {
    return 2 * NODE_TRAVERSAL_TIME() * (TTL_VALUE() + TIMEOUT_BUFFER());
}


void set_alarm_time(uint32_t miliseconds, unsigned long* sc, unsigned long* usc)
{
    *sc = (unsigned long)(miliseconds / 1000);
    *usc = (miliseconds % 1000) * 1000;
}


uint32_t get_alarm_time(unsigned long sc, unsigned long usc)
{
    return (uint32_t)usc/1000 + sc*1000;
}