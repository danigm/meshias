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

    local_conf.backlog=1;
    local_conf.reuseaddr=1;
    sprintf(local_conf.path,"%s-%d","/tmp/meshias",getpid());

    local_server_create(&data.local_server,&local_conf);

    register_fd(data.local_server.fd,data.fds);
}

void unix_interface_shutdown()
{
    local_server_destroy(&data.local_server);
}

void unix_interface_receive_packets()
{
}

void unix_interface_run_command(enum commands_t command)
{
    switch(command)
    {
        case KILL:
            break;
        case RESTART:
            break;
        case SHOW_ROUTES:
            break;
        case SHOW_STATISTICS:
            break;
        case CLEAN_STATISTICS:
            reset_stats();
            break;
    }
}
