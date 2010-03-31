#include "communication_interface.h"
#include "msh_data.h"
#include "statistics.h"
#include "routing_table.h"
#include "meshias-tools.h"

#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

#define stats_size 5000

void get_statistics(char *buf, int bufsize);
void get_routes(char *buf, int bufsize);
void send_msg(int sock, char *buf, int size);

void send_msg(int sock, char *buf, int size)
{
    int bufsize = 1024;
    int sended = 0;
    while (sended < size) {
        send(sock, buf + sended, bufsize, 0);
        sended += bufsize;
    }
}

int comm_interface_init()
{
    int fd;
    struct sockaddr_in local;
    socklen_t length = sizeof(local);
    int option = 1;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR , &option,
                   sizeof option) == -1) {
        perror("Error changing socket options (SO_REUSEADDR):");
        return -1;
    }

    local.sin_family = AF_INET;
    local.sin_port = htons(MESH_PORT);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *) &local, length) == -1) {
        return -1;
    }

    if (listen(fd, 1) == -1) {
        return -1;
    }

    data.comm_fd = fd;

    register_fd(fd, data.fds);
    return 0;
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
    memset(stats_buf, 0, stats_size);

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
        size = strlen(stats_buf);
    } else if (strncmp(command, MSG_SHOW_STATISTICS,
                       strlen(MSG_SHOW_STATISTICS)) == 0) {
        get_statistics (stats_buf, stats_size);
        tosend = stats_buf;
        size = strlen(stats_buf);
    } else if (strncmp(command, MSG_CLEAN_STATISTICS,
                       strlen(MSG_CLEAN_STATISTICS)) == 0) {
        stats_reset();
        get_statistics (stats_buf, stats_size);
        tosend = stats_buf;
        size = strlen(stats_buf);
    }
    else {
        return;
    }

    send_msg(fd, tosend, size);
}

void comm_interface_receive_packets()
{
    char buffer[128];
    int ret;
    int newsock;
    struct sockaddr_in addr;
    socklen_t length = sizeof(addr);

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
              // "no_control_received: %d\n"
              "send_aodv_errors: %d\n"
              "send_aodv_incomplete: %d\n"
              "rreq_incorrect_size: %d\n"
              "rrep_incorrect_size: %d\n"
              "rerr_incorrect_size: %d\n"
              "rerr_dest_cont_zero: %d\n"
              "rrep_ack_incorrect_size: %d\n"
              "aodv_incorrect_type: %d\n"
              "no_ttl_received: %d\n"
              "error_aodv_recv: %d\n"
              "error_nfq_recv: %d\n"
              "error_unix_recv: %d\n"
              "invalid_route: %d\n",
              stats.packets_dropped,
              stats.no_address_received,
              stats.no_payload_received,
              // stats.no_control_received,
              stats.send_aodv_errors,
              stats.send_aodv_incomplete,
              stats.rreq_incorrect_size,
              stats.rrep_incorrect_size,
              stats.rerr_incorrect_size,
              stats.rerr_dest_cont_zero,
              stats.rrep_ack_incorrect_size,
              stats.aodv_incorrect_type,
              stats.no_ttl_received,
              stats.error_aodv_recv,
              stats.error_nfq_recv,
              stats.error_unix_recv,
              stats.invalid_route);
}


int add_route(struct msh_route *route, char *buf, int bufsize)
{
    return snprintf (buf, bufsize, "%d#%d#%d#%d#%d#%d#%d\n",
                     route->dst_ip.s_addr,
                     route->prefix_sz,
                     route->dest_seq_num,
                     route->flags,
                     route->hop_count,
                     route->next_hop.s_addr,
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
    int ret = add_route(msh_r, rd->buf + rd->number, rd->size);
    rd->number += ret;
    return 0;
}

void get_routes(char *buf, int bufsize)
{
    struct route_data rd = {0, bufsize, buf};

    routing_table_foreach(data.routing_table, add_route_cb, &rd);
}
