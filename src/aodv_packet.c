#include "aodv_packet.h"
#include <arpa/inet.h>
#include <in.h>
#include <udp.h>
#include <ip.h>

extern char* aodv_create_packet(u_int32_t dest_addr,u_int16_t dest_port,
        u_int8_t ttl,const char* data,int data_len)
{
    char* packet;
    char* msg;
    struct aodv_iphdr ip;
    struct aodv_udphdr udp;
    struct sockaddr_in dest;
    unsigned packet_size;

    packet_size=sizeof(struct iphdr)+sizeof(struct updhdr)+data_len;
    
    /* Reserve memory for the complete packet */
    packet=(char*)malloc(packet_size);
    memset(packet,0,packet_size);

    /* Fill ip header */
    iphdr=(struct aodv_iphdr)packet;
    ip->version = 4;
    ip->ihl = 5;
    //TODO
    ip->id = htonl(random());
    ip->saddr = inet_addr("1.2.3.4");
    ip->daddr = inet_addr("1.2.3.4");
    ip->ttl = ttl;
    ip->protocol = IPPROTO_UDP;
    ip->tot_len = packet_size;
    ip->check = 0;

    /* Fill udp header */
    udp=(struct udphdr)(packet + sizeof(struct iphdr));
    udp->source=htons(AODV_PORT);
    udp->dest=htons(AODV_PORT);
    udp->len=packet_size;
    udp->check=0;

    /* Copy the message to the buffer to send it */
    msg=packet+sizeof(struct iphdr)+sizeof(struct udphdr);
    strncpy(msg,data,data_len);

    /* Fill the destination struct */
    dest.sin_family=AF_INET;
    dest.sin_port=udp->source;
    dest.sin_addr.s_addr=ip->saddr;

    /* Send the packet */
    if ((sendto(data.aodv_fd,packet,packet_size,0,(struct sockaddr*)&addr,
                    sizeof(struct sockaddr_in))) == -1) 
    {
        debug(1,"Error: It couldn't send an aodv packet");
        exit(1);
    }

    return 0;
}

