#include "communication_interface.h"
#include "meshias-tools.h"
#include "statistics.h"

#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
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

struct hostent *HE;

int main(int argc, char **argv)
{
    char command[command_SIZE];

   if ((HE=gethostbyname(argv[1]))==NULL){
      printf("gethostbyname() error\n");
      exit(-1);
   }

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

        printf("\nWrite a valid command. If you don't know write help:\n> ");
        scanf("%s",command);

    }while(!check_command(command));
}

int check_command(char *command)
{
    int i, found=0;

    for (i=0; i<N_ELEMENTS (COMMANDS); i++)
    {
        if (strncmp (command, COMMANDS[i], strlen (COMMANDS[i])) == 0)
            found = 1;
    }

    if (!found)
    {
        puts("Command no valid.");
        return 0;
    }

    return 1;
}

int is_help_command(char *command)
{
    return strncmp(command,MSG_HELP,strlen(MSG_HELP))==0 ||
        strncmp(command, "0", strlen("0")) == 0;
}

void help_command()
{
    printf("%s",HELP_COMMAND);
}

int is_quit_command(char *command)
{
    return strncmp(command,MSG_QUIT,strlen(MSG_QUIT))==0 ||
        strncmp(command, "1", strlen("1")) == 0;
}

int send_command(char* command)
{
    int i;
    for (i=0; i<N_ELEMENTS (COMMANDS); i+=2)
    {
        if (strncmp (command, COMMANDS[i], strlen (COMMANDS[i])) == 0)
        {
            snprintf(command, command_SIZE, "%s", COMMANDS[i+1]);
            break;
        }
    }

    int numbytes;
    int bufsize = 1024;
    char buf[bufsize];
    int fd;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr = *((struct in_addr *)HE->h_addr);
    bzero(&(addr.sin_zero),8);

    if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("socket() error\n");
        exit(-1);
    }

    if(connect(fd, (struct sockaddr *)&addr,
      sizeof(struct sockaddr))==-1)
    {
        printf("connect() error\n");
        exit(-1);
    }

    snprintf (buf, bufsize, command);
    if (send(fd, buf, strlen(buf), 0) < 0)
    {
        printf("Error en send() \n");
        exit(-1);
    }

    while ((numbytes=recv(fd, buf, bufsize, 0)) > 0){
        buf[numbytes] = 0;
        printf ("%s", buf);
    }

    close (fd);

    return 0;
}
