#include "local.h"
#include "unix_interface.h"
#include "meshias-tools.h"
#include "statistics.h"

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 128


int main(int argc, char **argv)
{
    while(process_command()!=-1)
    {
    }

    return 0;
}

int process_command()
{
    char command[BUFFER_SIZE];
    size_t size;
    int to_send;
    void (*func)(void*);

    do
    {
        memset(command,0,BUFFER_SIZE);
        to_send=1;

        puts("Escribe un comando valido:");
        scanf("%s",command);

        if(strncmp(command,MSG_KILL,strlen(MSG_KILL))==0) 
        {
            size=strlen(MSG_KILL);
            func=&command_print;
        }
        else if(strncmp(command,MSG_RESTART,
                    strlen(MSG_RESTART))==0) 
        {
            size=strlen(MSG_RESTART);
            func=&command_print;
        }
        else if(strncmp(command,MSG_SHOW_ROUTES,
                    strlen(MSG_SHOW_ROUTES))==0) 
        {
            size=strlen(MSG_SHOW_ROUTES);
            func=&command_print;
        }
        else if(strncmp(command,MSG_SHOW_STATISTICS,
                    strlen(MSG_SHOW_STATISTICS))==0) 
        {
            size=strlen(MSG_SHOW_STATISTICS);
            func=&command_show_statistics;
        }
        else if(strncmp(command,MSG_CLEAN_STATISTICS,
                    strlen(MSG_CLEAN_STATISTICS))==0) 
        {
            size=strlen(MSG_CLEAN_STATISTICS);
            func=&command_show_statistics;
        }
        // Unknown command
        else
        {
            puts("Command no valid. Write help.");
            to_send=0;
        }

        printf("command -> %s size %d\n",command,strlen(command));
    }while(to_send==0);

    struct local_conf conf;
    conf.backlog=1;
    conf.reuseaddr=0;
    sprintf(conf.path,"%s","/tmp/meshias");

    return local_do_request(command,&conf,func);
}

void command_print(void *str)
{
    char* aux=str;
    printf("received: %s\n",aux);
}

void command_show_statistics(void *data)
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
