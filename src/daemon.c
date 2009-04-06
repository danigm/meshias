#include <string.h>
#include <netinet/in.h>

#include "daemon.h"
#include "route_obj.h"
#include "aodv_packet.h"
#include "aodv_logic.h"
#include "statistics.h"

#define BUF_SIZE 1024

int daemon_init()
{
    int option = 1;
    struct sockaddr_in address;

    debug(1, "Daemon: opening aodv socket");
    // Create the udp daemon socket
    if( (data.daemon_fd = socket(AF_INET,SOCK_DGRAM,0)) == -1 )
    {
        perror("Error initializing the aodv UDP socket:");
        return ERR_INIT;
    }

    // This call is what allows broadcast packets to be sent
    if(setsockopt(data.daemon_fd,SOL_SOCKET,SO_BROADCAST,&option,
                sizeof option) == -1)
    {
        perror("Error changing socket options (SO_BROADCAST):");
        return ERR_INIT;
    }

    // This call is what allows ttl to be received
    if(setsockopt(data.daemon_fd,SOL_IP,IP_RECVTTL,&option,
                sizeof option) == -1)
    {
        perror("Error changing socket options (IP_RECVTTL):");
        return ERR_INIT;
    }

    //IPv4
    address.sin_family = AF_INET;
    // Set port
    address.sin_port = htons(AODV_UDP_PORT);
    // Listen from any ip
    address.sin_addr.s_addr = INADDR_ANY;

    // Set the socket to listen
    if( bind(data.daemon_fd, (struct sockaddr *)&address,
        sizeof(address)) == -1 )
    {
        perror("Error binding the aodv UDP socket:");
        return ERR_INIT;
    }

    // Adding daemon_fd to the set
    register_fd(data.daemon_fd,data.fds);

    debug(1, "Daemon initialized sucessfully");
    return 0;
}

void daemon_shutdown()
{
    close(data.daemon_fd);
}

void daemon_receive_packets()
{
    int numbytes;
    char name_buf[BUF_SIZE];
    char iov_buf[BUF_SIZE];
    char control_buf[BUF_SIZE];
    struct msghdr msg;
    struct iovec io;
    struct aodv_pkt* pkt;

    // Set all memory to 0
    memset(&msg, 0, sizeof(msg));
    memset(&io, 0, sizeof(io));
    memset(name_buf, 0, BUF_SIZE);
    memset(iov_buf, 0, BUF_SIZE);
    memset(control_buf, 0, BUF_SIZE);

    // Fill the msg
    msg.msg_name=&name_buf;
    msg.msg_namelen=BUF_SIZE;

    io.iov_base=iov_buf;
    io.iov_len=BUF_SIZE;

    msg.msg_iov=&io;
    msg.msg_iovlen=1;

    msg.msg_control=control_buf;
    msg.msg_controllen=BUF_SIZE;

    //Receive the packet
    numbytes = recvmsg(data.daemon_fd, &msg, 0);
    
    if(numbytes == -1)
    {
        stats.error_aodv_recv++;
        return;
    }

    pkt=aodv_pkt_get(&msg,numbytes);

    if(aodv_pkt_check(pkt)==0)
        return;
    
    // Filter if we are the senders
    if(aodv_pkt_get_address(pkt) == data.ip_addr.s_addr)
        return;

    //HERE STARTS THE AODV LOGIC
    switch(aodv_pkt_get_type(pkt))
    {
        case AODV_RREQ:
            aodv_process_rreq(pkt);
            break;
            
        case AODV_RREP:
            aodv_process_rrep(pkt);
            break;
            
        case AODV_RERR:
            aodv_process_rerr(pkt);
            break;
            
        case AODV_RREP_ACK:
            aodv_process_rrep_ack(pkt);
            break;
        // Impossible, pkt is checked before
        default:
            break;
    }

    // We're done with this packet: now free the mallocs!
    aodv_pkt_destroy(pkt);
}
