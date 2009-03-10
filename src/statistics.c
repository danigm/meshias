#include "statistics.h"

#include <string.h>
#include <stdio.h>

void init_stats()
{
    memset(&stats,0,sizeof(stats));
}

void reset_stats()
{
    memset(&stats,0,sizeof(stats));
}

void print_all_stats()
{
    printf("packets_dropped: %d\n", stats.packets_dropped);

    printf("no_address_received: %d\n", stats.no_address_received);
    printf("no_payload_received: %d\n", stats.no_payload_received);
    printf("no_control_received: %d\n", stats.no_control_received);

    printf("send_aodv_errors: %d\n", stats.send_aodv_errors);
    printf("send_aodv_incomplete: %d\n", stats.send_aodv_incomplete);

    printf("rreq_incorrect_size: %d\n", stats.rreq_incorrect_size);
    printf("rrep_incorrect_size: %d\n", stats.rrep_incorrect_size);
    printf("rerr_incorrect_size: %d\n", stats.rerr_incorrect_size);
    printf("rerr_dest_cont_zero: %d\n", stats.rerr_dest_cont_zero);
    printf("rrep_ack_incorrect_size: %d\n", stats.rrep_ack_incorrect_size);
    printf("aodv_incorrect_type: %d\n", stats.aodv_incorrect_type);

    printf("ttl_not_found: %d\n", stats.ttl_not_found);

    printf("error_recv: %d\n", stats.error_recv);

    printf("route_not_found: %d\n", stats.route_not_found);
    printf("invalid_route: %d\n", stats.invalid_route);
}
