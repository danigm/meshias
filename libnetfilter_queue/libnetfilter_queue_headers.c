#include "libnetfilter_queue_headers.h"
#include "libnetfilter_queue.h"
#include <arpa/inet.h>

struct nfq_iphdr
{
  /*
#if defined(__LITTLE_ENDIAN_BITFIELD)
  u_int8_t  ihl:4,
    version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
*/
 u_int8_t   version:4,
      ihl:4;
 /*
#endif
*/
  u_int8_t  tos;
  u_int16_t  tot_len;
  u_int16_t  id;
  u_int16_t  frag_off;
  u_int8_t  ttl;
  u_int8_t  protocol;
  u_int16_t check;
  u_int32_t  saddr;
  u_int32_t  daddr;
};

struct nfq_tcphdr
{
  u_int16_t  source;
  u_int16_t  dest;
  u_int32_t  seq;
  u_int32_t  ack_seq;
  /*
#if defined(__LITTLE_ENDIAN_BITFIELD)
  u_int16_t res1:4,
    doff:4,
    fin:1,
    syn:1,
    rst:1,
    psh:1,
    ack:1,
    urg:1,
    ece:1,
    cwr:1;
#elif defined(__BIG_ENDIAN_BITFIELD)
*/
  u_int16_t doff:4,
    res1:4,
    cwr:1,
    ece:1,
    urg:1,
    ack:1,
    psh:1,
    rst:1,
    syn:1,
    fin:1;
  /*
#endif
*/
  u_int16_t  window;
  u_int16_t check;
  u_int16_t  urg_ptr;
};

extern struct nfq_iphdr* nfq_get_iphdr(struct nfq_data *nfad)
{
	char *data;

  if(nfq_get_payload(nfad,&data)==-1)
    return NULL;
  return (struct nfq_iphdr*) data;
}

extern u_int8_t nfq_get_ip_ihl(struct nfq_iphdr *hdr)
{
  return hdr->ihl;
}

extern u_int8_t nfq_get_ip_version(struct nfq_iphdr *hdr)
{
  return hdr->version;
}

extern u_int8_t nfq_get_ip_tos(struct nfq_iphdr *hdr)
{
  return hdr->tos;
}

extern u_int16_t nfq_get_ip_tot_len(struct nfq_iphdr *hdr)
{
  return htons(hdr->tot_len);
}

extern u_int16_t nfq_get_ip_id(struct nfq_iphdr *hdr)
{
  return htons(hdr->id);
}

extern u_int16_t nfq_get_ip_fragoff(struct nfq_iphdr *hdr)
{
  return htons(hdr->frag_off);
}

extern u_int8_t nfq_get_ip_ttl(struct nfq_iphdr *hdr)
{
  return hdr->ttl;
}

extern u_int8_t nfq_get_ip_protocol(struct nfq_iphdr *hdr)
{
  return hdr->protocol;
}

extern u_int16_t nfq_get_ip_check(struct nfq_iphdr *hdr)
{
  return htons(hdr->check);
}

extern u_int32_t nfq_get_ip_saddr(struct nfq_iphdr *hdr)
{
  return htonl(hdr->saddr);
}

extern u_int32_t nfq_get_ip_daddr(struct nfq_iphdr *hdr)
{
  return htonl(hdr->daddr);
}

extern struct nfq_tcphdr* nfq_get_tcphdr(struct nfq_data *nfad)
{
	char *data;

  if(nfq_get_payload(nfad,&data)==-1)
    return NULL;
  data=data+sizeof(struct nfq_iphdr);
  return (struct nfq_tcphdr*) data;
}

extern u_int16_t nfq_get_tcp_source(struct nfq_tcphdr *hdr)
{
  return htons(hdr->source);
}

extern u_int16_t nfq_get_tcp_dest(struct nfq_tcphdr *hdr)
{
  return htons(hdr->dest);
}

extern u_int32_t nfq_get_tcp_seq(struct nfq_tcphdr *hdr)
{
  return htonl(hdr->seq);
}

extern u_int32_t nfq_get_tcp_ack_seq(struct nfq_tcphdr *hdr)
{
  return htonl(hdr->ack_seq);
}

extern u_int16_t nfq_get_tcp_flags(struct nfq_tcphdr *hdr)
{
  return htons(hdr->source);
}

/*
extern u_int16_t nfq_get_tcp_doff(struct nfq_tcphdr *hdr)
{
  return htons(hdr->source);
}

extern u_int16_t nfq_get_tcp_res1(struct nfq_tcphdr *hdr)
{
  return htons(hdr->source);
}

extern u_int16_t nfq_get_tcp_cwr(struct nfq_tcphdr *hdr)
{
  return htons(hdr->cwr);
}

extern u_int16_t nfq_get_tcp_ece(struct nfq_tcphdr *hdr)
{
  return htons(hdr->ece);
}

extern u_int16_t nfq_get_tcp_urg(struct nfq_tcphdr *hdr)
{
  return htons(hdr->urg);
}

extern u_int16_t nfq_get_tcp_ack(struct nfq_tcphdr *hdr)
{
  return htons(hdr->ack);
}

extern u_int16_t nfq_get_tcp_psh(struct nfq_tcphdr *hdr)
{
  return htons(hdr->psh);
}

extern u_int16_t nfq_get_tcp_rst(struct nfq_tcphdr *hdr)
{
  return htons(hdr->rst);
}

extern u_int16_t nfq_get_tcp_syn(struct nfq_tcphdr *hdr)
{
  return htons(hdr->syn);
}

extern u_int16_t nfq_get_tcp_fin(struct nfq_tcphdr *hdr)
{
  return htons(hdr->fin);
}

extern u_int16_t nfq_get_tcp_seq(struct nfq_tcphdr *hdr)
{
  return htons(hdr->seq);
}

*/
extern u_int16_t nfq_get_tcp_window(struct nfq_tcphdr *hdr)
{
  return htons(hdr->window);
}

extern u_int16_t nfq_get_tcp_check(struct nfq_tcphdr *hdr)
{
  return htons(hdr->check);
}

extern u_int16_t nfq_get_tcp_urg_ptr(struct nfq_tcphdr *hdr)
{
  return htons(hdr->urg_ptr);
}
