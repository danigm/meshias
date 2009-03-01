#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <string.h>

#include "aodv_packet.h"

#define DEFAULT_TTL 64

struct aodv_pkt
{
    char* payload;
    size_t payload_len;
    struct sockaddr_in address; // If the packet has been received is the
                                // source else if it has been sent is the dest
    uint8_t ttl;
};

struct aodv_pkt *aodv_create_pkt()
{
    struct aodv_pkt* pkt=(struct aodv_pkt*)calloc(1,sizeof(struct aodv_pkt));

    // Set some values by default like broadcast sent
    pkt->ttl=DEFAULT_TTL;
    pkt->address.sin_family=AF_INET;
    pkt->address.sin_port=htons(AODV_UDP_PORT);
    pkt->address.sin_addr.s_addr=INADDR_BROADCAST;
}

struct aodv_pkt *aodv_get_pkt(struct msghdr* msg)
{
    struct aodv_pkt* pkt=(struct aodv_pkt*)calloc(1,sizeof(struct aodv_pkt));
    
    // If address has been received
    if(msg->msg_namelen>0)
    {
        pkt->address=*(struct sockaddr_in*)msg->msg_name;
    }
    // If data has been received
    if(msg->msg_iovlen>0)
    {
        pkt->payload_len=msg->msg_iov->iov_len;
        strncpy(pkt->payload,msg->msg_iov->iov_base,pkt->payload_len);
    }
    // If control data has been received
    if(msg->msg_controllen>0)
        pkt->ttl=aodv_receive_ttl(msg);

    return pkt;
}

uint8_t aodv_get_ttl(struct aodv_pkt* pkt)
{
    return pkt->ttl;
}

void aodv_set_ttl(struct aodv_pkt* pkt,uint8_t ttl)
{
    pkt->ttl=ttl;
}

void aodv_decrease_ttl(struct aodv_pkt *pkt)
{
    (pkt->ttl)--;
}

// NOTE in this function the port is not returned
uint32_t aodv_get_address(struct aodv_pkt* pkt)
{
    return ntohl(pkt->address.sin_addr.s_addr);
}

void aodv_set_address(struct aodv_pkt* pkt,uint32_t addr)
{
    pkt->address.sin_addr.s_addr=htonl(addr);
}

char* aodv_get_payload(struct aodv_pkt *pkt)
{
    return pkt->payload;
}

int aodv_get_payload_len(struct aodv_pkt *pkt)
{
    return pkt->payload_len;
}

int aodv_get_type(struct aodv_pkt *pkt)
{
    const int *type = (int*)aodv_get_payload(pkt);
    return *type;
}

int aodv_check_packet(struct aodv_pkt* pkt)
{
    struct aodv_rreq* rreq;
    struct aodv_rrep* rrep;
    struct aodv_rerr* rerr;
    struct aodv_rrep_ack* rrep_ack;

    switch(aodv_get_type(pkt))
    {
        case AODV_RREQ:
            if( sizeof(struct aodv_rreq) == aodv_get_payload_len(pkt) )
            {
                rreq = (struct aodv_rreq*)aodv_get_payload(pkt);
            }
            else
            {
                debug(1, "Error: AODV_RREQ packet with incorrect size");
                return 0;
            }
            break;
            
        case AODV_RREP:
            if( sizeof(struct aodv_rrep) == aodv_get_payload_len(pkt) )
            {
                rrep = (struct aodv_rrep*)aodv_get_payload(pkt);
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
            if( aodv_get_payload_len(pkt) >=
                sizeof(uint32_t) + sizeof(struct unrecheable_dest))
            {
                rerr = (struct aodv_rerr*)aodv_get_payload(pkt);
                if(rerr->dest_count == 0)
                {
                    debug(1, "Error: AODV_RERR packet with DestCont = 0");
                    return 0;
                }
                else
                {
                    // Buffer size = header size + number of desticount * desticount_size
                    if( aodv_get_payload_len(pkt) == sizeof(uint32_t)+
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
            if( sizeof(struct aodv_rrep_ack) == aodv_get_payload_len(pkt) )
            {
                rrep_ack = (struct aodv_rrep_ack*)aodv_get_payload(pkt);
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


size_t aodv_get_size(struct aodv_pkt* pkt)
{
    return pkt->payload_len;
}
int aodv_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl)
{
}

ssize_t aodv_send_packet(struct aodv_pkt* pkt)
{
    /*
    if ((bytes=sendto(data.aodv_fd,aodv_get_payload(pkt),
                    aodv_get_payload_len(pkt),0,
                    (struct sockaddr*)aodv_get_address(pkt),
                    sizeof(struct sockaddr_in))) == -1) 
    {
        debug(1,"Error: It couldn't send an aodv packet");
    }


    return bytes;
    */
    return 0;
}

void aodv_build_rrep(struct aodv_pkt* pkt,uint8_t flags, uint8_t prefix_sz,
        uint8_t hop_count,uint32_t dest_ip_addr,uint32_t dest_seq_num,
        uint32_t orig_ip_addr,uint32_t lifetime)
{
    struct aodv_rrep* rrep;

    /* Reserve memory for the structure */
    rrep=(struct aodv_rrep*)calloc(1,sizeof(struct aodv_rrep));
    pkt->payload_len=sizeof(struct aodv_rrep);

    rrep->type = AODV_RREP;
    rrep->flags = flags;
    rrep->prefix_sz = prefix_sz;
    rrep->hop_count = hop_count;
    rrep->dest_ip_addr = dest_ip_addr;
    rrep->dest_seq_num = dest_seq_num;
    rrep->orig_ip_addr = orig_ip_addr;
    rrep->lifetime = lifetime;

    pkt->payload=(char*)rrep;
}

void aodv_build_rreq(struct aodv_pkt* pkt,uint8_t flags, uint8_t hop_count,
        uint32_t rreq_id,uint32_t dest_ip_addr,uint32_t dest_seq_num,
        uint32_t orig_ip_addr,uint32_t orig_seq_num)
{
    struct aodv_rreq* rreq;
    
    /* Reserve memory for the structure */
    rreq=(struct aodv_rreq*)calloc(1,sizeof(struct aodv_rreq));
    pkt->payload_len=sizeof(struct aodv_rreq);


    rreq->type = AODV_RREQ;
    rreq->flags = flags;
    rreq->hop_count = hop_count;
    rreq->rreq_id = rreq_id;
    rreq->dest_ip_addr = dest_ip_addr;
    rreq->dest_seq_num = dest_seq_num;
    rreq->orig_ip_addr = orig_ip_addr;
    rreq->orig_seq_num = orig_seq_num;

    pkt->payload=(char*)rreq;
}

void aodv_build_rerr(struct aodv_pkt* pkt,uint8_t flag, uint8_t dest_count,
    struct unrecheable_dest** dests)
{
    struct aodv_rerr* rerr;
    struct unrecheable_dest* aux;
    int i=0;
    pkt->payload_len=sizeof(struct aodv_rerr) +
        (dest_count * sizeof(struct unrecheable_dest));

    /* Reserve memory for the structure */
    rerr=(struct aodv_rerr*)calloc(1,pkt->payload_len);

    rerr->type = AODV_RERR;
    rerr->flag = flag;
    rerr->dest_count = dest_count;

    aux=(struct unrecheable_dest*)rerr+sizeof(struct aodv_rerr);
    for(i=0;i<dest_count;i++)
    {
        aux->ip_addr = dests[i]->ip_addr;
        aux->seq_num = dests[i]->seq_num;
        aux++;
    }

    pkt->payload=(char*)rerr;
}

void aodv_build_rrep_ack(struct aodv_pkt* pkt)
{
    struct aodv_rrep_ack* rrep_ack;

    pkt->payload_len=sizeof(struct aodv_rrep_ack);

    /* Reserve memory for the structure */
    rrep_ack=(struct aodv_rrep_ack*)calloc(1,sizeof(struct aodv_rrep_ack));
    
    rrep_ack->type = AODV_RREP_ACK;

    pkt->payload=(char*)rrep_ack;
}

static uint8_t aodv_receive_ttl(struct msghdr* msg)
{
    struct cmsghdr *cmsg;
    for (cmsg = CMSG_FIRSTHDR(msg); cmsg != NULL;
            cmsg = CMSG_NXTHDR(msg,cmsg))
    {
        if (cmsg->cmsg_level == SOL_IP
                && cmsg->cmsg_type == IP_TTL)
            return *(uint8_t*)CMSG_DATA(cmsg);
    }
     // FIXME TTL no encontrado
    return -1;
}

