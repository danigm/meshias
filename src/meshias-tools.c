int main()
{
    return 0;
}
/*
#include "local.h"
#include "unix_interface.h"

#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 128

int main(int argc, char **argv)
{
    int fd;
    struct local_conf conf;

    conf->backlog=1;
    conf->reuseaddr=1;
    sprintf(conf.path,"%s-%d","/tmp/meshias",getpid());
    local_client_create();
    return 0;
}

int init_fd()
{
}

char* get_command()
{
    char buffer[BUFFER_SIZE];

    do
    {
        memset(buffer,0,BUFFER_SIZE);
        puts("Escribe un comando AODV valido:");
        scanf("%s",&buffer);

        char command[BUFFER_SIZE];
        char* aux=strchr(buffer,' ');

        if(aux!=NULL)
            strncpy(command,buffer,buffer-aux);
        else
            strncpy(command,buffer,BUFFER_SIZE);

        if(strcmp(command,"kill"))
        {
        }
        else if(strcmp(command,"restart"))
        {
        }
        else if(strcmp(command,"showstatistics"))
        {
        }
        else if(strcmp(command,"cleanstatistics"))
        {
        }
        else if(strcmp(command,"showroutes"))
        {
        }
        else if(strcmp(command,"help"))
        {
            puts("Command avaliable: showroutes, cleanstatistics, showstatistics,kill,restart");
        }
        else
            puts("Command no valid. Writes help.");
    }while(1);
}
*/
