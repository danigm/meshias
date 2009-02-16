#ifndef LIBNETFILTER_QUEUE_HEADERS__H__
#define LIBNETFILTER_QUEUE_HEADERS__H__

#include "libnetfilter_queue.h"

struct nfq_tcphdr;
struct nfq_iphdr;
struct nfq_data;

extern struct nfq_iphdr* nfq_get_iphdr(struct nfq_data *nfad);

extern u_int8_t nfq_get_ip_ihl(struct nfq_iphdr *hdr);
extern u_int8_t nfq_get_ip_version(struct nfq_iphdr *hdr);
extern u_int8_t nfq_get_ip_tos(struct nfq_iphdr *hdr);
extern u_int16_t nfq_get_ip_tot_len(struct nfq_iphdr *hdr);
extern u_int16_t nfq_get_ip_id(struct nfq_iphdr *hdr);
extern u_int16_t nfq_get_ip_fragoff(struct nfq_iphdr *hdr);
extern u_int8_t nfq_get_ip_ttl(struct nfq_iphdr *hdr);
extern u_int8_t nfq_get_ip_protocol(struct nfq_iphdr *hdr);
extern u_int16_t nfq_get_ip_check(struct nfq_iphdr *hdr);
extern u_int32_t nfq_get_ip_saddr(struct nfq_iphdr *hdr);
extern u_int32_t nfq_get_ip_daddr(struct nfq_iphdr *hdr);


extern struct nfq_tcphdr* nfq_get_tcphdr(struct nfq_data *nfad);

extern u_int16_t nfq_get_tcp_source(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_dest(struct nfq_tcphdr *hdr);
extern u_int32_t nfq_get_tcp_seq(struct nfq_tcphdr *hdr);
extern u_int32_t nfq_get_tcp_ack_seq(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_flags(struct nfq_tcphdr *hdr);
/*
extern u_int16_t nfq_get_tcp_doff(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_res1(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_cwr(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_ece(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_urg(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_ack(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_psh(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_rst(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_syn(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_fin(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_seq(struct nfq_tcphdr *hdr);
*/
extern u_int16_t nfq_get_tcp_window(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_check(struct nfq_tcphdr *hdr);
extern u_int16_t nfq_get_tcp_urg_ptr(struct nfq_tcphdr *hdr);

#endif
