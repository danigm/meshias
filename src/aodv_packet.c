#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aodv_packet.h"
#include "msh_data.h"

#define DEFAULT_TTL 64

struct aodv_pkt
{
    char* payload;
    size_t payload_len;
    struct sockaddr_in address; // If the packet has been received is the
                                // source else if it has been sent is the dest
    uint8_t ttl;
};

struct aodv_pkt *aodv_pkt_alloc()
{
    struct aodv_pkt* pkt=(struct aodv_pkt*)calloc(1,sizeof(struct aodv_pkt));

    // Set some values by default like broadcast sent
    pkt->ttl=DEFAULT_TTL;
    pkt->address.sin_family=AF_INET;
    pkt->address.sin_port=htons(AODV_UDP_PORT);
    pkt->address.sin_addr.s_addr=INADDR_BROADCAST;
}

struct aodv_pkt *aodv_pkt_get(struct msghdr* msg)
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
        //FIXME payload_len is set to 1024 always
        // we won't always send 1K
        pkt->payload_len=msg->msg_iov->iov_len;
        pkt->payload=(char*)calloc(1,pkt->payload_len);
        memcpy(pkt->payload,msg->msg_iov->iov_base,pkt->payload_len);
    }
    // If control data has been received
    if(msg->msg_controllen>0)
        pkt->ttl=aodv_pkt_receive_ttl(msg);

    //FIXME dont check errors
    return pkt;
}

ssize_t aodv_pkt_send(struct aodv_pkt* pkt)
{
    // Set output ttl in socket option
    setsockopt(data.daemon_fd,SOL_IP,IP_TTL,&(pkt->ttl),sizeof(pkt->ttl));

    // Sending the information
    int numbytes=sendto(data.daemon_fd,pkt->payload,pkt->payload_len,0,
            (struct sockaddr*)&(pkt->address),sizeof(pkt->address));

    //FIXME resolve errors
    return numbytes;
}

void aodv_pkt_destroy(struct aodv_pkt* pkt)
{
    free(pkt->payload);
    free(pkt);
}

uint8_t aodv_pkt_get_ttl(struct aodv_pkt* pkt)
{
    return pkt->ttl;
}

void aodv_pkt_set_ttl(struct aodv_pkt* pkt,uint8_t ttl)
{
    pkt->ttl=ttl;
}

void aodv_pkt_decrease_ttl(struct aodv_pkt *pkt)
{
    (pkt->ttl)--;
}

// NOTE in this function the port is not returned
uint32_t aodv_pkt_get_address(struct aodv_pkt* pkt)
{
    return ntohl(pkt->address.sin_addr.s_addr);
}

void aodv_pkt_set_address(struct aodv_pkt* pkt,uint32_t addr)
{
    pkt->address.sin_addr.s_addr=htonl(addr);
}

char* aodv_pkt_get_payload(struct aodv_pkt *pkt)
{
    return pkt->payload;
}

int aodv_pkt_get_payload_len(struct aodv_pkt *pkt)
{
    return pkt->payload_len;
}

int aodv_pkt_get_type(struct aodv_pkt *pkt)
{
    const int *type = (int*)aodv_pkt_get_payload(pkt);
    return *type;
}

int aodv_pkt_check(struct aodv_pkt* pkt)
{
    struct aodv_rreq* rreq;
    struct aodv_rrep* rrep;
    struct aodv_rerr* rerr;
    struct aodv_rrep_ack* rrep_ack;

    switch(aodv_pkt_get_type(pkt))
    {
        case AODV_RREQ:
            if( sizeof(struct aodv_rreq) == aodv_pkt_get_payload_len(pkt) )
            {
                rreq = (struct aodv_rreq*)aodv_pkt_get_payload(pkt);
            }
            else
            {
                debug(1, "Error: AODV_RREQ packet with incorrect size");
                return 0;
            }
            break;
            
        case AODV_RREP:
            if( sizeof(struct aodv_rrep) == aodv_pkt_get_payload_len(pkt) )
            {
                rrep = (struct aodv_rrep*)aodv_pkt_get_payload(pkt);
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
            if( aodv_pkt_get_payload_len(pkt) >=
                sizeof(uint32_t) + sizeof(struct unrecheable_dest))
            {
                rerr = (struct aodv_rerr*)aodv_pkt_get_payload(pkt);
                if(rerr->dest_count == 0)
                {
                    debug(1, "Error: AODV_RERR packet with DestCont = 0");
                    return 0;
                }
                else
                {
                    // Buffer size = header size + number of desticount * desticount_size
                    if( aodv_pkt_get_payload_len(pkt) == sizeof(uint32_t)+
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
            if( sizeof(struct aodv_rrep_ack) == aodv_pkt_get_payload_len(pkt) )
            {
                rrep_ack = (struct aodv_rrep_ack*)aodv_pkt_get_payload(pkt);
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


size_t aodv_pkt_get_size(struct aodv_pkt* pkt)
{
    return pkt->payload_len;
}

int aodv_pkt_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl)
{
}

void aodv_pkt_build_rrep(struct aodv_pkt* pkt,uint8_t flags, uint8_t prefix_sz,
        uint8_t hop_count,uint32_t dest_ip_addr,uint32_t dest_seq_num,
        uint32_t orig_ip_addr,uint32_t lifetime)
{
    struct aodv_rrep* rrep;

    /* Reserve memory for the structure */
    rrep=(struct aodv_rrep*)calloc(1,sizeof(struct aodv_rrep));
    pkt->payload_len=sizeof(struct aodv_rrep);

    rrep->type = htons(AODV_RREP);
    rrep->flags = htons(flags);
    rrep->prefix_sz = htons(prefix_sz);
    rrep->hop_count = htons(hop_count);
    rrep->dest_ip_addr = htonl(dest_ip_addr);
    rrep->dest_seq_num = htonl(dest_seq_num);
    rrep->orig_ip_addr = htonl(orig_ip_addr);
    rrep->lifetime = htonl(lifetime);

    pkt->payload=(char*)rrep;
}

void aodv_pkt_build_rreq(struct aodv_pkt* pkt,uint8_t flags, uint8_t hop_count,
        uint32_t rreq_id,uint32_t dest_ip_addr,uint32_t dest_seq_num,
        uint32_t orig_ip_addr,uint32_t orig_seq_num)
{
    struct aodv_rreq* rreq;
    
    /* Reserve memory for the structure */
    rreq=(struct aodv_rreq*)calloc(1,sizeof(struct aodv_rreq));
    pkt->payload_len=sizeof(struct aodv_rreq);


    rreq->type = htons(AODV_RREQ);
    rreq->flags = htons(flags);
    rreq->hop_count = htons(hop_count);
    rreq->rreq_id = htonl(rreq_id);
    rreq->dest_ip_addr = htonl(dest_ip_addr);
    rreq->dest_seq_num = htonl(dest_seq_num);
    rreq->orig_ip_addr = htonl(orig_ip_addr);
    rreq->orig_seq_num = htonl(orig_seq_num);

    pkt->payload=(char*)rreq;
}

void aodv_pkt_build_rerr(struct aodv_pkt* pkt,uint8_t flag, uint8_t dest_count,
    struct unrecheable_dest** dests)
{
    struct aodv_rerr* rerr;
    struct unrecheable_dest* aux;
    int i=0;
    pkt->payload_len=sizeof(struct aodv_rerr) +
        (dest_count * sizeof(struct unrecheable_dest));

    /* Reserve memory for the structure */
    rerr=(struct aodv_rerr*)calloc(1,pkt->payload_len);

    rerr->type = htons(AODV_RERR);
    rerr->flag = htons(flag);
    rerr->dest_count = htons(dest_count);

    aux=(struct unrecheable_dest*)rerr+sizeof(struct aodv_rerr);
    for(i=0;i<dest_count;i++)
    {
        aux->ip_addr = htonl(dests[i]->ip_addr);
        aux->seq_num = htonl(dests[i]->seq_num);
        aux++;
    }

    pkt->payload=(char*)rerr;
}

void aodv_pkt_build_rrep_ack(struct aodv_pkt* pkt)
{
    struct aodv_rrep_ack* rrep_ack;

    pkt->payload_len=sizeof(struct aodv_rrep_ack);

    /* Reserve memory for the structure */
    rrep_ack=(struct aodv_rrep_ack*)calloc(1,sizeof(struct aodv_rrep_ack));
    
    rrep_ack->type = htons(AODV_RREP_ACK);

    pkt->payload=(char*)rrep_ack;
}

static uint8_t aodv_pkt_receive_ttl(struct msghdr* msg)
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

