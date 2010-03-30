#include "communication_interface.h"
#include "msh_data.h"
#include "statistics.h"
#include "routing_table.h"

#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

#define stats_size 5000

void get_statistics(char *buf, int bufsize);
void get_routes(char *buf, int bufsize);

int comm_interface_init()
{
    int fd;
    socklen_t len;
    struct sockaddr_in local;
    int option = 0;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR , &option,
                   sizeof option) == -1) {
        perror("Error changing socket options (SO_REUSEADDR):");
        return -1;
    }

    local.sin_family = AF_INET;
    local.sin_port = htons(12345);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *) &local, sizeof(local)) == -1) {
        return -1;
    }

    if (listen(fd, 1) == -1) {
        return -1;
    }

    data.comm_fd = fd;

    register_fd(fd, data.fds);
}

void comm_interface_shutdown()
{
    close(data.comm_fd);
}

void comm_interface_process_command(int fd, char* v_command)
{
    char stats_buf[stats_size];
    char* command = v_command;
    size_t size = strlen(command);
    void* tosend = command;
    char ack[] = "ACK";

    if (strncmp(command, MSG_KILL, strlen(MSG_KILL)) == 0) {
        data.end = 1;
        tosend = ack;
        size = sizeof(ack);
    } else if (strncmp(command, MSG_RESTART,
                       strlen(MSG_RESTART)) == 0) {
        tosend = ack;
        size = sizeof(ack);
    } else if (strncmp(command, MSG_SHOW_ROUTES,
                       strlen(MSG_SHOW_ROUTES)) == 0) {
        get_routes (stats_buf, stats_size);
        tosend = stats_buf;
        printf ("%d\n", strlen(stats_buf));
        size = strlen(stats_buf);
    } else if (strncmp(command, MSG_SHOW_STATISTICS,
                       strlen(MSG_SHOW_STATISTICS)) == 0) {
        get_statistics (stats_buf, stats_size);
        tosend = stats_buf;
        size = strlen(stats_buf);
    } else if (strncmp(command, MSG_CLEAN_STATISTICS,
                       strlen(MSG_CLEAN_STATISTICS)) == 0) {
        stats_reset();
        tosend = &stats;
        size = sizeof(stats);
    }
    // Unknown command
    else {
        puts("else");
        return;
    }

    send(fd, tosend, size, 0);
}

void comm_interface_receive_packets()
{
    char buffer[128];
    int ret;
    int newsock;
    struct sockaddr_in addr;
    int length = sizeof(addr);

    newsock = accept(data.comm_fd, (struct sockaddr *)&addr, &length);
    ret = recv(newsock, buffer, 128, 0);
    buffer[ret] = 0;
    comm_interface_process_command(newsock, buffer);
    close (newsock);
}

void get_statistics(char *buf, int bufsize)
{
    snprintf (buf, bufsize, "packets_dropped: %d\n"
              "no_address_received: %d\n"
              "no_payload_received: %d\n"
              "no_control_received: %d\n"
              "send_aodv_errors: %d\n"
              "send_aodv_incomplete: %d\n"
              "rreq_incorrect_size: %d\n"
              "rrep_incorrect_size: %d\n"
              "rerr_incorrect_size: %d\n"
              "rerr_dest_cont_zero: %d\n"
              "rrep_ack_incorrect_size: %d\n"
              "aodv_incorrect_type: %d\n"
              "ttl_not_found: %d\n"
              "error_aodv_recv: %d\n"
              "error_nf_recv: %d\n"
              "error_unix_recv: %d\n"
              "route_not_found: %d\n"
              "invalid_route: %d\n",
              stats.packets_dropped,
              stats.no_address_received,
              stats.no_payload_received,
              stats.no_control_received,
              stats.send_aodv_errors,
              stats.send_aodv_incomplete,
              stats.rreq_incorrect_size,
              stats.rrep_incorrect_size,
              stats.rerr_incorrect_size,
              stats.rerr_dest_cont_zero,
              stats.rrep_ack_incorrect_size,
              stats.aodv_incorrect_type,
              stats.ttl_not_found,
              stats.error_aodv_recv,
              stats.error_nf_recv,
              stats.error_unix_recv,
              stats.route_not_found,
              stats.invalid_route);
}


void add_route(struct msh_route *route, char *buf, int bufsize)
{
    puts("asdfas");
    snprintf (buf, bufsize, "dst_ip: %s\n"
              "prefix_sz: %d\n"
              "dest_seq_num: %d\n"
              "flags: %d\n"
              "hop_count: %d\n"
              "next_hop: %s\n"
              "net_iface: %d\n",
              inet_htoa(route->dst_ip),
              route->prefix_sz,
              route->dest_seq_num,
              route->flags,
              route->hop_count,
              inet_htoa(route->next_hop),
              route->net_iface);
}

struct route_data {
    int number;
    int size;
    char *buf;
};

int add_route_cb(struct msh_route *msh_r, void *data)
{
    struct route_data *rd = (struct route_data *)data;
    add_route(msh_r, rd->buf + rd->number, rd->size);
    rd->number++;
}

void get_routes(char *buf, int bufsize)
{
    struct route_data rd = {0, bufsize, buf};

    routing_table_foreach(data.routing_table, add_route_cb, &rd);
}
