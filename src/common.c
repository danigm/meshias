#include <sys/time.h>
#include <time.h>
#include "common.h"

uint32_t ACTIVE_ROUTE_TIMEOUT()
{
    return 3000;
}

uint32_t ALLOWED_HELLO_LOSS()
{
    return 2;
}

uint32_t RREQ_RETRIES()
{
    return 2;
}

uint32_t HELLO_INTERVAL()
{
    uint32_t mark;
    return 1000;
}

uint32_t LOCAL_ADD_TTL()
{
    return 2;
}

uint32_t NET_DIAMETER()
{
    return 35;
}

uint32_t RERR_RATELIMIT()
{
    return 10;
}

uint32_t RREQ_RATELIMIT()
{
    return 10;
}

uint32_t TIMEOUT_BUFFER()
{
    return 2;
}

uint32_t TTL_START()
{
    return 3;
}

uint32_t TTL_INCREMENT()
{
    return 2;
}

uint32_t TTL_THRESHOLD()
{
    return 7;
}

uint32_t NODE_TRAVERSAL_TIME()
{
    return 40;
}

uint32_t NEXT_HOP_WAIT()
{
    return NODE_TRAVERSAL_TIME() + 10;
}

uint32_t MAX_REPAIR_TTL()
{
    return 0.3 * NET_DIAMETER();
}

uint32_t MY_ROUTE_TIMEOUT()
{
    return 2 * ACTIVE_ROUTE_TIMEOUT();
}

uint32_t NET_TRAVERSAL_TIME()
{
    return 2 * NODE_TRAVERSAL_TIME() * NET_DIAMETER();
}

uint32_t BLACKLIST_TIMEOUT()
{
    return RREQ_RETRIES() * NET_TRAVERSAL_TIME();
}

uint32_t PATH_DISCOVERY_TIME()
{
    return 2 * NET_TRAVERSAL_TIME();
}

uint32_t DELETE_PERIOD()
{
    //TODO: see page 32 of RFC 3561 for other implementations
    return ALLOWED_HELLO_LOSS() * HELLO_INTERVAL();
}

uint32_t MIN_REPAIR_TTL()
{
    return 0; // see note below
}

uint32_t TTL_VALUE()
{
    return 0; // see note below
}

uint32_t RING_TRAVERSAL_TIME()
{
    return 2 * NODE_TRAVERSAL_TIME() * (TTL_VALUE() + TIMEOUT_BUFFER());
}

uint32_t binary_exponential_backoff_time(int8_t prev_tries)
{
    return (1 << prev_tries) * NET_TRAVERSAL_TIME();
}

uint32_t expanding_ring_search_ttl(uint8_t hop_count, int8_t prev_tries)
{
    if (prev_tries == 0)
        return hop_count + TTL_START();

    uint32_t ret = hop_count + TTL_START() + prev_tries * TTL_INCREMENT();

    return (ret > TTL_THRESHOLD()) ? NET_DIAMETER() : ret;
}

uint32_t minimal_lifetime(uint8_t hop_count)
{
    struct timeval now;

    gettimeofday(&now, NULL);
    uint32_t current_time = get_alarm_time(now.tv_sec, now.tv_usec);

    return (current_time + 2 * NET_TRAVERSAL_TIME() -
            2 * hop_count * NODE_TRAVERSAL_TIME());
}
