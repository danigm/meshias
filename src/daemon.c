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

    /*
    if((numbytes = recvfrom(data.daemon_fd,buffer,BUF_SIZE,0,
                    (struct sockaddr*)&source_addr,
                    &addr_len)) == -1)
    {
        debug(1, "FATAL ERROR: recvmsg");
        exit(1);
    }
    */
    /* Function inet_ntoa is obsolete, actually inet_ntop must be used 
     * but I don't know how
     */
    /*
    printf("Daemon: got packet from %s:%d\n",
            (char *)inet_ntoa(source_addr.sin_addr),
            //     inet_ntop(source_addr.sin_family, source_addr.sin_addr,
            //        saa, sizeof(saa)),
            (int)ntohs(source_addr.sin_port));

    buffer[numbytes] = '\0';
    
    // Check if the packet is correctly built
    if(!aodv_check_packet(buffer));
    
    switch(aodv_get_type(buffer))
    {
        case AODV_RREQ:
            //aodv_recv_rreq(buffer,source_addr);
            break;
            
        case AODV_RREP:
            break;
            
        case AODV_RERR:
            break;
            
        case AODV_RREP_ACK:
            break;
            
        default:
            printf("Unknown packet type received. Payload:\n%s\n", buffer);
            break;
    }
    */
    memset(&msgh,0,sizeof(msgh));
    msgh.msg_control=(struct iovec*)malloc(10000*sizeof(struct iovec));
    msgh.msg_controllen=10000;
    //Receive the packet
    if((numbytes = recvmsg(data.daemon_fd,&msgh,0)) == -1)
    {
        debug(1, "FATAL ERROR: recvmsg");
        exit(1);
    }

    printf("%d %d %d %d\n",msgh.msg_namelen,msgh.msg_iovlen,msgh.msg_controllen,msgh.msg_flags);
    printf("Daemon: Packet has been received with ttl %d\n",
            aodv_get_ttl(&msgh));
    switch(msgh.msg_flags)
    {
        case MSG_EOR:
            puts("MSG_EOR");
            break;
        case MSG_TRUNC:
            puts("MSG_TRUNC");
            break;
        case MSG_CTRUNC:
            puts("MSG_CTRUNC");
            break;
        case MSG_OOB:
            puts("MSG_OOB");
            break;
        case MSG_ERRQUEUE:
            puts("MSG_ERRQUEUE");
            break;
        case MSG_DONTWAIT:
            puts("MSG_DONTWAIT");
            break;
    }
}

int aodv_get_ttl(struct msghdr* msgh)
{
    struct cmsghdr *cmsg;

    /* Recibir los datos auxiliares en msgh */
    for (cmsg = CMSG_FIRSTHDR(msgh); cmsg != NULL;
            cmsg = CMSG_NXTHDR(msgh,cmsg))
    {
        puts("hi");
        if (cmsg->cmsg_level == SOL_IP
                && cmsg->cmsg_type == IP_TTL)
            return (int)CMSG_DATA(cmsg);
    }
    /* 
     * FIXME TTL no encontrado
     */
    return -1;
}

int aodv_get_type(const char* b)
{
    const int *type = (const int*)b;
    return *type;
}

int aodv_check_packet(const char* b)
{
    struct aodv_rreq* rreq;
    struct aodv_rrep* rrep;
    struct aodv_rerr* rerr;
    struct aodv_rrep_ack* rrep_ack;

    switch(aodv_get_type(b))
    {
        case AODV_RREQ:
            if( sizeof(struct aodv_rreq) == sizeof(b) )
            {
                rreq = (struct aodv_rreq*)b;
            }
            else
            {
                debug(1, "Error: AODV_RREQ packet with incorrect size");
                return 0;
            }
            break;
            
        case AODV_RREP:
            if( sizeof(struct aodv_rrep) == sizeof(b) )
            {
                rrep = (struct aodv_rrep*)b;
            }
            else
            {
                debug(1, "Error: AODV_RREP packet with incorrect size");
                return 0;
            }
            break;
            
        case AODV_RERR:
            /* The size is variable so we have to be more careful
             * Buffer size must be at least header size +
             * one unrecheable_dest
             */
            if( sizeof(b) >=
                sizeof(uint32_t) + sizeof(struct unrecheable_dest))
            {
                rerr = (struct aodv_rerr*)b;
                if(rerr->dest_count == 0)
                {
                    debug(1, "Error: AODV_RERR packet with DestCont = 0");
                    return 0;
                }
                else
                {
                    // Buffer size = header size + number of desticount * desticount_size
                    if( sizeof(b) == sizeof(uint32_t)+
                            sizeof(struct unrecheable_dest) * rerr->dest_count )
                    {
                    }
                    else
                    {
                        debug(1, "Error: AODV_RERR packet with incorrect size");
                        return 0;
                    }
                }
            }
            else
            {
                debug(1, "Error: AODV_RERR packet with incorrect size");
                return 0;
            }
            break;
            
        case AODV_RREP_ACK:
            if( sizeof(struct aodv_rrep_ack) == sizeof(b) )
            {
                rrep_ack = (struct aodv_rrep_ack*)b;
            }
            else
            {
                debug(1, "Error: AODV_RERR_ACK packet with incorrect size");
                return 0;
            }
            break;
            
        default:
            
            debug(1, "Error: Incorrect packet aodv type");
            return 0;
            break;
    }
    return 1;
}

/*
void aodv_recv_rreq(const char *b, const struct sockaddr_in* source)
{
}

void aodv_recv_rrep(const char *b, const struct sockaddr_in* source)
{
}

void aodv_recv_rerr(const char *b, const struct sockaddr_in* source)
{
}

void aodv_recv_rrep_ack(const char *b, const struct sockaddr_in* source)
{
}


int aodv_send_rreq(struct aodv_rreq* to_sent, char ttl)
{
    struct sockaddr_in their_addr; // connector's address information
    int numbytes;

    // Changing ttl
    setsockopt(data.daemon_fd, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl,
        sizeof(char));

    // Set the destination to broadcast
    their_addr.sin_family = AF_INET;     // host byte order
    their_addr.sin_port = htons(AODV_UDP_PORT); // short, network byte order
    inet_pton(AF_INET,"255.255.255.255", &their_addr.sin_addr.s_addr);
    memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));

    if( (numbytes = sendto(data.daemon_fd, (char*)to_sent, sizeof(to_sent), 0,
        (struct sockaddr *)&their_addr, sizeof(their_addr))) == -1 )
    {
        return ERR_SEND;
    }
    return numbytes;
}

int aodv_send_rrep(struct aodv_rrep* to_sent)
{
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
    inet_pton(AF_INET,"255.255.255.255", &their_addr.sin_addr.s_addr);
    //their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));

    if( (numbytes = sendto(data.daemon_fd, (char*) to_sent, sizeof(to_sent), 0,
        (struct sockaddr *)&their_addr, sizeof(their_addr))) == -1 )
    {
        return ERR_SEND;
    }
    return numbytes;
}

int aodv_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl)
{
}
*/
