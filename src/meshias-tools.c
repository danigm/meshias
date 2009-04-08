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
    help\n\
    quit\n\
    kill\n\
    restart\n\
    showroutes\n\
    showstatistics\n\
    cleanstatistics\n\n"
    


int main(int argc, char **argv)
{
    char command[command_SIZE];
    while(1)
    {
        get_command(command);
        if(is_help_command(command))
        {
            help_command();
            continue;
        }
        else if(is_quit_command(command))
            break;

        send_command(command);
    };

    return 0;
}

int get_command(char *command)
{
    do
    {
        memset(command,0,command_SIZE);

        printf("Write a valid command. If you don't know write help:\n> ");
        scanf("%s",command);

    }while(!check_command(command));
}

int check_command(char *command)
{
    if(strncmp(command,MSG_HELP,strlen(MSG_HELP))==0);
    else if(strncmp(command,MSG_QUIT,strlen(MSG_QUIT))==0);
    else if(strncmp(command,MSG_KILL,strlen(MSG_KILL))==0);
    else if(strncmp(command,MSG_RESTART,
                strlen(MSG_RESTART))==0);
    else if(strncmp(command,MSG_SHOW_ROUTES,
                strlen(MSG_SHOW_ROUTES))==0);
    else if(strncmp(command,MSG_SHOW_STATISTICS,
                strlen(MSG_SHOW_STATISTICS))==0);
    else if(strncmp(command,MSG_CLEAN_STATISTICS,
                strlen(MSG_CLEAN_STATISTICS))==0);
    // Unknown command
    else
    {
        puts("Command no valid.");
        return 0;
    }

    return 1;
}

void (*get_function_command(char* command))(void*)
{
    void (*func)(void*)=NULL;

    if(strncmp(command,MSG_KILL,strlen(MSG_KILL))==0) 
        func=&print_command;
    else if(strncmp(command,MSG_RESTART,
                strlen(MSG_RESTART))==0) 
        func=&print_command;
    else if(strncmp(command,MSG_SHOW_ROUTES,
                strlen(MSG_SHOW_ROUTES))==0) 
        func=&print_command;
    else if(strncmp(command,MSG_SHOW_STATISTICS,
                strlen(MSG_SHOW_STATISTICS))==0) 
        func=&show_statistics_command;
    else if(strncmp(command,MSG_CLEAN_STATISTICS,
                strlen(MSG_CLEAN_STATISTICS))==0) 
        func=&show_statistics_command;

    return func;
}

int is_help_command(char *command)
{
    return strncmp(command,MSG_HELP,strlen(MSG_HELP))==0;
}

void help_command()
{
    printf("%s",HELP_COMMAND);
}

int is_quit_command(char *command)
{
    return strncmp(command,MSG_QUIT,strlen(MSG_QUIT))==0;
}

int send_command(char* command)
{
    struct local_conf conf;
    conf.backlog=1;
    conf.reuseaddr=0;
    sprintf(conf.path,"%s","/tmp/meshias");

    return local_do_request(command,&conf,get_function_command(command));
}

void print_command(void *str)
{
    char* aux=str;
    printf("received: %s\n",aux);
}

void show_statistics_command(void *data)
{
    struct statistics_t* stats=data;

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
