#include <string.h>

#include "daemon.h"
#include "msh_data.h"
#include "common.h"

#define BUF_SIZE 512

int daemon_init()
{
    int broadcast=1;
    struct sockaddr_in address;

    debug(3,"Daemon: opening socket");
    // Create the udp daemon socket
    if((data.daemon_fd=socket(AF_INET,SOCK_DGRAM,0))==-1)
    {
        debug(1,"Error initializing the UDP socket");
        return ERR_INIT;
    }

    //IPv4
    address.sin_family=AF_INET;
    // Set port
    address.sin_port=htons(AODV_UDP_PORT);
    // Listen from any ip
    address.sin_addr.s_addr=INADDR_ANY;


    debug(3,"binding socket");
    // Set the socket to listen
    if(bind(data.daemon_fd,(struct sockaddr *)&address,
                sizeof(address)) == -1)
    {
        close(data.daemon_fd);
        debug(1,"Error binding the UDP socket");
        return ERR_INIT;
    }

    debug(3,"changing socket options");
    // This call is what allows broadcast packets to be sent
    if(setsockopt(data.daemon_fd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) == -1)
    {
        debug(1,"setsockopt (SO_BROADCAST)");
        return ERR_INIT;
    }

    /* Adding daemon_fd to the set */
    FD_SET(data.daemon_fd,&data.all_fd);

    /* max_fd */
    if(data.daemon_fd>=data.max_fd)
      data.max_fd=data.daemon_fd+1;

    debug(3,"Daemon initialized sucessfully");
    return 0;
}

void daemon_shutdown()
{
    close(data.daemon_fd);
}

void daemon_receive_packets()
{
    int numbytes;
    char buffer[BUF_SIZE];
    char string[512];
    struct sockaddr_in their_addr;
    size_t addr_len=sizeof their_addr;
    char saa[INET6_ADDRSTRLEN];
    if((numbytes = recvfrom(data.daemon_fd, buffer, BUF_SIZE-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        debug(1,"FATAL ERROR: recvfrom");
        exit(1);
    }

   // printf("listener: got packet from %s\n",
    //    inet_ntop(their_addr.ss_family,
     //       (((struct sockaddrin*)their_addr)->sin_addr),
      //      saa, sizeof(saa)));
    //debug(3,"Daemon: packet is %d bytes long\n", numbytes);
    debug(3,"Daemon: AODV packet was received");
    buffer[numbytes] = '\0';
    //debug(3,"Daemon: packet contains \"%s\"\n", buffer);
}

int aodv_send_rreq(struct aodv_rreq* to_sent,char ttl)
{
    struct sockaddr_in their_addr; // connector's address information
    int numbytes;

    // Changing ttl
    setsockopt(data.daemon_fd,IPPROTO_IP,IP_MULTICAST_TTL,(char *)&ttl,
            sizeof(char));

    // Set the destination to broadcast
    their_addr.sin_family = AF_INET;     // host byte order
    their_addr.sin_port = htons(AODV_UDP_PORT); // short, network byte order
    inet_pton(AF_INET,"255.255.255.255",&their_addr.sin_addr.s_addr);
    //their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));

    if((numbytes=sendto(data.daemon_fd,(char*) to_sent,sizeof(to_sent),0,
                    (struct sockaddr *)&their_addr,sizeof(their_addr)))==-1)
    {
        return ERR_SEND;
    }
    return numbytes;
}

int aodv_send_rerr(struct aodv_rerr* to_sent)
{
    struct sockaddr_in their_addr; // connector's address information
    int numbytes;

    // Changing ttl
    //setsockopt(data.daemon_fd,IPPROTO_IP,IP_MULTICAST_TTL,(char *)&ttl,
    //        sizeof(char));

    // Set the destination to broadcast
    their_addr.sin_family = AF_INET;     // host byte order
    their_addr.sin_port = htons(AODV_UDP_PORT); // short, network byte order
    inet_pton(AF_INET,"255.255.255.255",&their_addr.sin_addr.s_addr);
    //their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));

    if((numbytes=sendto(data.daemon_fd,(char*) to_sent,sizeof(to_sent),0,
                    (struct sockaddr *)&their_addr,sizeof(their_addr)))==-1)
    {
        return ERR_SEND;
    }
    return numbytes;
}

int aodv_send_rrep(struct aodv_rrep* to_sent)
{
}

int aodv_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl)
{
}
