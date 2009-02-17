#include <string.h>

#include "daemon.h"
#include "msh_data.h"
#include "common.h"

int daemon_init()
{
    int broadcast=1;
    struct sockaddr_in address;
    // Create the udp daemon socket
    if((data.daemon_fd=socket(AF_INET,SOCK_DGRAM,0))==-1)
    {
        perror("Error initializing the UDP socket");
        return ERR_INIT;
    }

    //IPv4
    address.sin_family=AF_INET;
    // Set port
    address.sin_port=AODV_UDP_PORT;
    // Listen from any ip
    address.sin_addr.s_addr=INADDR_ANY;

    // Set the socket to listen
    if(bind(data.daemon_fd,(struct sockaddr *)&address,
                sizeof(address)) == -1)
    {
        close(data.daemon_fd);
        perror("Error binding the UDP socket");
        return ERR_INIT;
    }

    // This call is what allows broadcast packets to be sent
    if(setsockopt(data.daemon_fd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) == -1)
    {
        perror("setsockopt (SO_BROADCAST)");
        return ERR_INIT;
    }

    return 0;
}

void daemon_shutdown()
{
    close(data.daemon_fd);
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
}

int aodv_send_rrep(struct aodv_rrep* to_sent)
{
}

int aodv_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl)
{
}
