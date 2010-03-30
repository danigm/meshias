#include "unix_interface.h"
#include "local.h"
#include "msh_data.h"
#include "statistics.h"

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int unix_interface_init()
{
    struct local_conf local_conf;

    local_conf.backlog = 1;
    local_conf.reuseaddr = 0;
    sprintf(local_conf.path, "%s", "/tmp/meshias");

    local_server_create(&data.local_server, &local_conf);

    register_fd(data.local_server.fd, data.fds);
}

void unix_interface_shutdown()
{
    local_server_destroy(&data.local_server);
}

void unix_interface_process_command(int fd, void* v_command)
{
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
    } else if (strncmp(command, MSG_SHOW_STATISTICS,
                       strlen(MSG_SHOW_STATISTICS)) == 0) {
        tosend = &stats;
        size = sizeof(stats);
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

void unix_interface_receive_packet()
{
    local_server_do_step(&data.local_server,
                         &unix_interface_process_command);
}
