#include "aodv_packet.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

/* TODO
 * Memory is reserved twice for each packet
 */

ssize_t aodv_send_packet(uint32_t dest_addr,uint8_t ttl,
        const char* payload,size_t payload_len)
{
    /* Fill the destination struct */
    dest.sin_family=AF_INET;
    dest.sin_port=htons(AODV_PORT);
    dest.sin_addr.s_addr=htonl(dest_addr);

    /* Send the packet */
    if ((bytes=sendto(data.aodv_fd,packet,packet_size,0,
                    (struct sockaddr*)&addr,
                    sizeof(struct sockaddr_in))) == -1) 
    {
        debug(1,"Error: It couldn't send an aodv packet");
    }


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

int aodv_get_type(const char* b)
{
    const int *type = (const int*)b;
    return *type;
}

size_t aodv_get_size_pkt(const char* b)
{
    const struct aodv_rerr* rerr;
    switch(aodv_get_type_pkt(b))
    {
        case AODV_RREP:
            sizeof(struct aodv_rrep);
            break;
        case AODV_RREQ:
            return sizeof(struct aodv_rreq);
        case AODV_RERR:
            rerr=(struct aodv_rerr*)b;
            return sizeof(struct aodv_rerr) +
                rerr->dest_count * sizeof(struct unrecheable_dest);
        case AODV_RREP_ACK:
            return sizeof(struct aodv_rrep_ack);
        default:
            //FIXME statistic
            return 0;
    }
}
