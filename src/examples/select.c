#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>


#define PORT 9990
#define NSOCKET 2
#define SIZE 1024

int main(int argc,char *argvp[])
{
    int sockets[NSOCKET];
    fd_set set;
    struct sockaddr_in addr;
    int i=0;
    int max=0;
    struct timeval tv;
    char buffer[SIZE];

    FD_ZERO(&set);

    for(i=0;i<NSOCKET;i++)
    {
        sockets[i]=socket(AF_INET,SOCK_DGRAM,0);
        if(sockets[i] == -1 )
        {
            perror("socket");
            goto end;
        }
    }

    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;

    for(i=0;i<NSOCKET;i++)
    {
        addr.sin_port=htons(PORT+i);
        if(bind ( sockets[i],(struct sockaddr*)&addr,sizeof(addr))==-1)
        {
            perror("binding");
            goto end;
        }
        FD_SET(sockets[i],&set);
        if(sockets[i]>max)
            max=sockets[i];
    }

    while(1)
    {
        select(max+1,&set,NULL,NULL,NULL);
        for(i=0;i<NSOCKET;i++)
        {
            if(FD_ISSET(sockets[i],&set))
            {
                printf("Recibido socket %d\n",i);
                memset(buffer,0,SIZE);
                if(recv(sockets[i],buffer,SIZE,0)<0)
                {
                    perror("recv");
                    goto end;
                }
                printf("%s\n",buffer);
            }
        }

        tv.tv_sec=5;
        if(select(max+1,&set,NULL,NULL,&tv)==0)
        {
            puts("Time Out");
        }
        else
        {
            for(i=0;i<NSOCKET;i++)
            {
                if(FD_ISSET(sockets[i],&set))
                {
                    printf("Recibido socket %d\n",i);
                    memset(buffer,0,SIZE);
                    if(recv(sockets[i],buffer,SIZE,0)<0)
                    {
                        perror("recv");
                        goto end;
                    }
                    printf("%s\n",buffer);
                }
            }
        }
    }

end:

    for(i=0;i<NSOCKET;i++)
        close(sockets[i]);
}
