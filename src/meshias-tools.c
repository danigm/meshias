#include "local.h"
#include "unix_interface.h"
#include "meshias-tools.h"
#include "statistics.h"

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define command_SIZE 128
#define HELP_COMMAND "Usable commands: \n\
    0. help\n\
    1. quit\n\
    2. kill\n\
    3. restart\n\
    4. showroutes\n\
    5. showstatistics\n\
6. cleanstatistics\n\n"

#define N_ELEMENTS(arr) (sizeof (arr) / sizeof ((arr)[0]))


char *COMMANDS[] = {
    "0", MSG_HELP,
    "1", MSG_QUIT,
    "2", MSG_KILL,
    "3", MSG_RESTART,
    "4", MSG_SHOW_ROUTES,
    "5", MSG_SHOW_STATISTICS,
    "6", MSG_CLEAN_STATISTICS,
};


int main(int argc, char **argv)
{
    char command[command_SIZE];

    while (1) {
        get_command(command);

        if (is_help_command(command)) {
            help_command();
            continue;
        } else if (is_quit_command(command)) {
            break;
        }

        send_command(command);
    };

    return 0;
}

int get_command(char *command)
{
    do {
        memset(command, 0, command_SIZE);

        printf("\nWrite a valid command. If you don't know write help:\n> ");
        scanf("%s", command);

    } while (!check_command(command));
}

int check_command(char *command)
{
    int i, found = 0;

    for (i = 0; i < N_ELEMENTS(COMMANDS); i++) {
        if (strncmp(command, COMMANDS[i], strlen(COMMANDS[i])) == 0) {
            found = 1;
        }
    }

    if (!found) {
        puts("Command no valid.");
        return 0;
    }

    return 1;
}

void (*get_function_command(char* command))(void*)
{
    int i;
    void (*func)(void*) = NULL;

    struct cmd {
        char *msg;
        void (*func)(void*);
    };

    struct cmd commands[] = {
        {MSG_KILL, print_command},
        {MSG_RESTART, print_command},
        {MSG_SHOW_ROUTES, show_routes_command},
        {MSG_SHOW_STATISTICS, show_statistics_command},
        {MSG_CLEAN_STATISTICS, show_statistics_command},
    };


    for (i = 0; i < N_ELEMENTS(commands); i++) {
        if (strncmp(command, commands[i].msg, strlen(commands[i].msg)) == 0) {
            func = commands[i].func;
        }
    }

    return func;
}

int is_help_command(char *command)
{
    return strncmp(command, MSG_HELP, strlen(MSG_HELP)) == 0 ||
           strncmp(command, "0", strlen("0")) == 0;
}

void help_command()
{
    printf("%s", HELP_COMMAND);
}

int is_quit_command(char *command)
{
    return strncmp(command, MSG_QUIT, strlen(MSG_QUIT)) == 0 ||
           strncmp(command, "1", strlen("1")) == 0;
}

int send_command(char* command)
{
    int i;
    struct local_conf conf;
    conf.backlog = 1;
    conf.reuseaddr = 0;
    sprintf(conf.path, "%s", "/tmp/meshias");

    for (i = 0; i < N_ELEMENTS(COMMANDS); i += 2) {
        if (strncmp(command, COMMANDS[i], strlen(COMMANDS[i])) == 0) {
            snprintf(command, command_SIZE, "%s", COMMANDS[i+1]);
            break;
        }
    }

    return local_do_request(command, &conf, get_function_command(command));
}

void print_command(void *str)
{
    char* aux = str;
    printf("received: %s\n", aux);
}

void show_statistics_command(void *data)
{
    struct statistics_t* stats = data;

    printf("packets_dropped: %d\n", stats->packets_dropped);

    printf("no_address_received: %d\n", stats->no_address_received);
    printf("no_payload_received: %d\n", stats->no_payload_received);
    printf("no_control_received: %d\n", stats->no_control_received);

    printf("send_aodv_errors: %d\n", stats->send_aodv_errors);
    printf("send_aodv_incomplete: %d\n", stats->send_aodv_incomplete);

    printf("rreq_incorrect_size: %d\n", stats->rreq_incorrect_size);
    printf("rrep_incorrect_size: %d\n", stats->rrep_incorrect_size);
    printf("rerr_incorrect_size: %d\n", stats->rerr_incorrect_size);
    printf("rerr_dest_cont_zero: %d\n", stats->rerr_dest_cont_zero);
    printf("rrep_ack_incorrect_size: %d\n", stats->rrep_ack_incorrect_size);
    printf("aodv_incorrect_type: %d\n", stats->aodv_incorrect_type);

    printf("ttl_not_found: %d\n", stats->ttl_not_found);

    printf("error_aodv_recv: %d\n", stats->error_aodv_recv);
    printf("error_nf_recv: %d\n", stats->error_nf_recv);
    printf("error_unix_recv: %d\n", stats->error_unix_recv);

    printf("route_not_found: %d\n", stats->route_not_found);
    printf("invalid_route: %d\n", stats->invalid_route);
}

void show_routes_command(void *data)
{
    struct route* route = data;

    printf("dst_ip: %s\n", inet_htoa(route->dst_ip));
    printf("prefix_sz: %d\n", route->prefix_sz);
    printf("dest_seq_num: %d\n", route->dest_seq_num);
    printf("flags: %d\n", route->flags);
    printf("hop_count: %d\n", route->hop_count);
    printf("next_hop: %s\n", inet_htoa(route->next_hop));
    printf("net_iface: %d\n", route->net_iface);
}
