#include "aodv_packet.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

/* TODO
 * Memory is reserved twice for each packet
 */

ssize_t aodv_send_packet(uint32_t dest_addr,
        uint8_t ttl,const char* payload,int payload_len)
{
    char* packet;
    char* msg;
    struct aodv_iphdr ip;
    struct aodv_udphdr udp;
    struct sockaddr_in dest;
    ssize_t packet_size;
    ssize_t bytes;

    packet_size=sizeof(struct iphdr)+sizeof(struct updhdr)+payload_len;
    
    /* Reserve memory for the complete packet */
    packet=(char*)malloc(packet_size);
    memset(packet,0,packet_size);

    /* Fill ip header */
    iphdr=(struct iphdr)packet;
    ip->version = 4;
    ip->ihl = 5;
    /* FIXME */
    ip->id = 0;
    ip->saddr = inet_addr("127.0.0.1");
    ip->daddr = inet_addr("127.0.0.1");
    ip->ttl = ttl;
    ip->protocol = IPPROTO_UDP;
    ip->tot_len = packet_size;
    ip->check = checksum((uint16_t*)ip,ip->ip_ihl << 1);

    /* Fill udp header */
    udp=(struct udphdr)(packet + sizeof(struct iphdr));
    udp->source=htons(AODV_PORT);
    udp->dest=htons(AODV_PORT);
    udp->len=sizeof(struct udphdr)+payload_len;
    udp->check=checksum((uint16_t*)udp,udp->len >> 1);

    /* Copy the message to the buffer to send it */
    msg=packet+sizeof(struct iphdr)+sizeof(struct udphdr);
    memcpy(msg,payload,payload_len);

    /* Fill the destination struct */
    dest.sin_family=AF_INET;
    dest.sin_port=udp->dest;
    dest.sin_addr.s_addr=ip->daddr;

    /* Send the packet */
    if ((bytes=sendto(data.aodv_fd,packet,packet_size,0,
                    (struct sockaddr*)&addr,
                    sizeof(struct sockaddr_in))) == -1) 
    {
        debug(1,"Error: It couldn't send an aodv packet");
    }

    free(packet);

    return bytes;
}

char* aodv_create_rrep(uint8_t flags, uint8_t prefix_sz, uint8_t hop_count,
        uint32_t dest_ip_addr, uint32_t dest_seq_num, uint32_t orig_ip_addr,
        uint32_t lifetime)
{
    struct aodv_rrep* rrep;

    /* Reserve memory for the structure */
    rrep=(struct aodv_rrep*)malloc(sizeof(struct aodv_rrep));
    memset(rrep,0,sizeof(struct aodv_rrep));

    rrep->type = AODV_RREP;
    rrep->flags = flags;
    rrep->prefix_sz = prefix_sz;
    rrep->hop_count = hop_count;
    rrep->dest_ip_addr = dest_ip_addr;
    rrep->dest_seq_num = dest_seq_num;
    rrep->orig_ip_addr = orig_ip_addr;
    rrep->lifetime = lifetime;

    return (char*)rrep;
}

char* aodv_create_rreq(uint8_t flags, uint8_t hop_count, uint32_t rreq_id,
    uint32_t dest_ip_addr, uint32_t dest_seq_num, uint32_t orig_ip_addr,
    uint32_t orig_seq_num)
{
    struct aodv_rreq* rreq;
    
    /* Reserve memory for the structure */
    rreq=(struct aodv_rreq*)malloc(sizeof(struct aodv_rreq));
    memset(rreq,0,sizeof(struct aodv_rreq));


    rreq->type = AODV_RREQ;
    rreq->flags = flags;
    rreq->hop_count = hop_count;
    rreq->rreq_id = rreq_id;
    rreq->dest_ip_addr = dest_ip_addr;
    rreq->dest_seq_num = dest_seq_num;
    rreq->orig_ip_addr = orig_ip_addr;
    rreq->orig_seq_num = orig_seq_num;

    return (char*)rreq;
}

char* aodv_create_rerr(uint8_t flag, uint8_t dest_count,
    struct unrecheable_dest** dests)
{
    struct aodv_rerr* rerr;
    struct unrecheable_dest* aux;
    unsigned size=sizeof(struct aodv_rerr) +
        (dests_num * sizeof(struct unrecheable_dest));
    int i;

    /* Reserve memory for the structure */
    rerr=(struct aodv_rerr*)malloc(size);
    memset(rerr,0,size);

    rerr->type = AODV_RERR;
    rerr->flag = flag;
    rerr->dest_count = dest_count;

    aux=rerr+sizeof(struct aodv_rerr);
    for(i=0;i<dests_count;i++)
    {
        aux->ip_addr = dests[i]->ip_addr;
        aux->seq_num = dests[i]->seq_num;
        aux++;
    }

    return (char*)rerr;
}

char* aodv_create_rrep_ack()
{
    struct aodv_rrep_ack* rrep_ack;

    /* Reserve memory for the structure */
    rrep_ack=(struct aodv_rrep_ack*)malloc(sizeof(struct aodv_rrep_ack));
    memset(rrep_ack,0,sizeof(struct rrep_ack));
    
    rrep_ack->type = AODV_RREP_ACK;

    return rrep_ack;
}

uint16_t checksum(uint16_t *buf,int nwords)
{
    //this function returns the checksum of a buffer
    uint32_t sum;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (uint16_t) (~sum);
}
