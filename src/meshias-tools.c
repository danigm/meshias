#include "local.h"
#include "unix_interface.h"
#include "meshias-tools.h"

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 128


int main(int argc, char **argv)
{
    int fd;
    int end=0;

    while(!end)
    {
        end=process_command();
        receive_response();
    }

    return 0;
}

void receive_response()
{
}

int init_fd()
{
}

int process_command()
{
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    int size;
    int ret=0;
    int to_send=0;
    char* aux;

    do
    {
        memset(buffer,0,BUFFER_SIZE);
        puts("Escribe un comando valido:");
        scanf("%s",buffer);

        aux=strchr(buffer,'\0');

        if(aux!=NULL)
        {
            size=aux-buffer;
            size++;
        }
        else
        {
            size=BUFFER_SIZE;
        }

        printf("size %d\n",size);
        strncpy(command,buffer,size);

        if(strcmp(command,"kill")==0)
        {
            puts("kill");
            to_send=1;
            ret=1;
        }
        else if(strcmp(command,"restart")==0 ||
            strcmp(command,"showstatistics")==0 ||
            strcmp(command,"cleanstatistics")==0 ||
            strcmp(command,"showroutes")==0)
        {
            puts("entra");
            to_send=1;
        }
        else if(strcmp(command,"help"))
        {
            puts("Command avaliable: showroutes, cleanstatistics, showstatistics, kill, restart.");
        }
        else
            puts("Command no valid. Write help.");

        printf("command -> %s\n",command);
    }while(to_send==0);

    struct local_conf conf;
    conf.backlog=1;
    conf.reuseaddr=0;
    //sprintf(conf.path,"%s-%d","/tmp/meshias",getpid());
    sprintf(conf.path,"%s","socket");
    local_do_request(0,&conf,&local_step);

    return ret;
}
