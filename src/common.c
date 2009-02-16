#include "common.h"


int ACTIVE_ROUTE_TIMEOUT() {
    return 3000;
}

int ALLOWED_HELLO_LOSS() {
    return 2;
}

int RREQ_RETRIES() {
    return 2;
}

int HELLO_INTERVAL() {
    uint32_t mark;
    return 1000;
}

int LOCAL_ADD_TTL() {
    return 2;
}

int NET_DIAMETER() {
    return 35;
}

int RERR_RATELIMIT() {
    return 10;
}

int RREQ_RATELIMIT() {
    return 10;
}

int TIMEOUT_BUFFER() {
    return 2;
}

int TTL_START() {
    return 1;
}

int TTL_INCREMENT() {
    return 2;
}

int TTL_THRESHOLD() {
    return 7;
}

int NODE_TRAVERSAL_TIME() {
    return 40;
}

int NEXT_HOP_WAIT() {
    return NODE_TRAVERSAL_TIME() + 10;
}

int MAX_REPAIR_TTL() {
    return 0.3 * NET_DIAMETER();
}

int MY_ROUTE_TIMEOUT() {
    return 2 * ACTIVE_ROUTE_TIMEOUT();
}

int NET_TRAVERSAL_TIME() {
    return 2 * NODE_TRAVERSAL_TIME() * NET_DIAMETER();
}

int BLACKLIST_TIMEOUT() {
    return RREQ_RETRIES() * NET_TRAVERSAL_TIME();
}

int PATH_DISCOVERY_TIME() {
    return 2 * NET_TRAVERSAL_TIME();
}

int DELETE_PERIOD() {
    return 0; // see note below
}

int MIN_REPAIR_TTL() {
    return 0; // see note below
}

int TTL_VALUE() {
    return 0; // see note below
}

int RING_TRAVERSAL_TIME() {
    return 2 * NODE_TRAVERSAL_TIME() * (TTL_VALUE() + TIMEOUT_BUFFER());
}
