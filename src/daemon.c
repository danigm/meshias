#include <string.h>
#include <netinet/in.h>

#include "daemon.h"

#define BUF_SIZE 2024

int daemon_init()
{
    int broadcast = 1;
    struct sockaddr_in address;

    debug(3, "Daemon: opening aodv socket");
    // Create the udp daemon socket
    if( (data.daemon_fd = socket(AF_INET,SOCK_DGRAM,0)) == -1 )
    {
        debug(1, "Error initializing the UDP socket");
        return ERR_INIT;
    }

    //IPv4
    address.sin_family = AF_INET;
    // Set port
    address.sin_port = htons(AODV_UDP_PORT);
    // Listen from any ip
    address.sin_addr.s_addr = INADDR_ANY;


    debug(3, "binding socket");
    // Set the socket to listen
    if( bind(data.daemon_fd, (struct sockaddr *)&address,
        sizeof(address)) == -1 )
    {
        close(data.daemon_fd);
        debug(1, "Error binding the UDP socket");
        return ERR_INIT;
    }

    debug(3, "changing socket options");
    // This call is what allows broadcast packets to be sent
    if(setsockopt(data.daemon_fd,SOL_SOCKET,SO_BROADCAST,&broadcast,
                sizeof broadcast) == -1)
    {
        debug(1, "setsockopt (SO_BROADCAST)");
        return ERR_INIT;
    }

    /* Adding daemon_fd to the set */
    FD_SET(data.daemon_fd, &data.all_fd);

    /* max_fd */
    if(data.daemon_fd >= data.max_fd)
        data.max_fd = data.daemon_fd + 1;

    debug(3, "Daemon initialized sucessfully");
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
    struct sockaddr_in source_addr;
    socklen_t addr_len = sizeof source_addr;
    char saa[INET6_ADDRSTRLEN];
    struct msghdr msgh;

    //Receive the packet
    if((numbytes = recvmsg(data.daemon_fd,&msgh,0)) == -1)
    {
        debug(1, "FATAL ERROR: recvmsg");
        exit(1);
    }

}
