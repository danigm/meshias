#include <string.h>
#include <netinet/in.h>

#include "daemon.h"
#include "aodv_packet.h"

#define BUF_SIZE 1024

int daemon_init()
{
    int option = 1;
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

    debug(3, "changing socket options");
    // This call is what allows broadcast packets to be sent
    if(setsockopt(data.daemon_fd,SOL_SOCKET,SO_BROADCAST,&option,
                sizeof option) == -1)
    {
        debug(1, "setsockopt (SO_BROADCAST)");
        return ERR_INIT;
    }

    // This call is what allows ttl to be received
    if(setsockopt(data.daemon_fd,SOL_IP,IP_RECVTTL,&option,
                sizeof option) == -1)
    {
        debug(1, "setsockopt (SO_BROADCAST)");
        return ERR_INIT;
    }


    debug(3, "binding socket");
    // Set the socket to listen
    if( bind(data.daemon_fd, (struct sockaddr *)&address,
        sizeof(address)) == -1 )
    {
        close(data.daemon_fd);
        debug(1, "Error binding the UDP socket");
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
    char name_buf[BUF_SIZE];
    char iov_buf[BUF_SIZE];
    char control_buf[BUF_SIZE];
    struct msghdr msg;
    struct iovec io;
    struct aodv_pkt* pkt;

    // Set all memory to 0
    memset(&msg, 0, sizeof(msg));
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
    if( (numbytes = recvmsg(data.daemon_fd, &msg, 0)) == -1 )
    {
        perror("FATAL ERROR: recvmsg");
        return;
    }

    pkt=aodv_get_pkt(&msg);

    //HERE STARTS THE AODV LOGIC
    switch(aodv_get_type(pkt))
    {
        case AODV_RREQ:
            daemon_process_rreq(pkt);
            break;
            
        case AODV_RREP:
            daemon_process_rrep(pkt);
            break;
            
        case AODV_RERR:
            daemon_process_rerr(pkt);
            break;
            
        case AODV_RREP_ACK:
            daemon_process_rrep_ack(pkt);
            break;
            
        default:
            break;
    }
}


void daemon_process_rreq(struct aodv_pkt* pkt)
{
    // (See page 16 of RFC 3561)
    // First create/update a route to the previous hop without a valid sequence
    // number
    struct in_addr prev_hop = { .s_addr = aodv_get_address(pkt) };
    
    struct msh_route *find_route = msh_route_alloc();
    msh_route_set_dst_ip(find_route, prev_hop);
    struct msh_route *route = routing_table_find(data.routing_table,
        find_route, RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING);
    msh_route_destroy(find_route);
    
    // If found a route for the prev_hop, update it
    if(route)
    {
        // reset the lifetime and mark as valid
        msh_route_set_lifetime(route, ACTIVE_ROUTE_TIMEOUT());
    } else {
        // If route for prev_hop not found, create it
        route = msh_route_alloc();
        msh_route_set_dst_ip(route, prev_hop);
        routing_table_add(data.routing_table, route);
    }
    
    // Second, check if the rreq_queue has this rreq buffered. If so, we must
    // discard it.
    struct aodv_rreq* rreq = (struct aodv_rreq*)aodv_get_payload(pkt);
    struct in_addr dest_addr = { .s_addr = ntohl(rreq->rreq_id) };
    if(rreq_fifo_contains(data.rreq_queue, ntohl(rreq->rreq_id),
        dest_addr))
        return;

    uint8_t hop_count = ntohs(rreq->hop_count) + 1;
    rreq->hop_count = htons(hop_count);
    //TODO: continue here
}

void daemon_process_rrep(struct aodv_pkt* pkt)
{
    
}

void daemon_process_rerr(struct aodv_pkt* pkt)
{
    
}

void daemon_process_rrep_ack(struct aodv_pkt* pkt)
{
    
}

