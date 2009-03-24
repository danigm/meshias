#ifndef AODV_PACKET_H__
#define AODV_PACKET_H__

#include <sys/socket.h>
#include "common.h"

struct aodv_pkt;

struct aodv_pkt *aodv_pkt_alloc();

struct aodv_pkt *aodv_pkt_get(struct msghdr* msgh);

ssize_t aodv_pkt_send(struct aodv_pkt* pkt);

void aodv_pkt_destroy(struct aodv_pkt* pkt);

uint8_t aodv_pkt_get_ttl(struct aodv_pkt* pkt);

void aodv_pkt_set_ttl(struct aodv_pkt* pkt,uint8_t ttl);

void aodv_pkt_decrease_ttl(struct aodv_pkt *pkt);

// NOTE in this function the port is not returned
// only address in big endian is returned
uint32_t aodv_pkt_get_address(struct aodv_pkt* pkt);

// addr must be in big endian
void aodv_pkt_set_address(struct aodv_pkt* pkt,uint32_t addr);

char* aodv_pkt_get_payload(struct aodv_pkt *pkt);

int aodv_pkt_get_payload_len(struct aodv_pkt *pkt);

int aodv_pkt_get_type(struct aodv_pkt *pkt);

int aodv_pkt_check(struct aodv_pkt* pkt);

size_t aodv_pkt_get_size(struct aodv_pkt* pkt);

int aodv_pkt_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl);

void aodv_pkt_build_rrep(struct aodv_pkt* pkt,uint8_t flags, uint8_t prefix_sz,
        uint8_t hop_count,uint32_t dest_ip_addr,uint32_t dest_seq_num,
        uint32_t orig_ip_addr,uint32_t lifetime);

void aodv_pkt_build_rreq(struct aodv_pkt* pkt,uint8_t flags, uint8_t hop_count,
        uint32_t rreq_id,uint32_t dest_ip_addr,uint32_t dest_seq_num,
        uint32_t orig_ip_addr,uint32_t orig_seq_num);

void aodv_pkt_build_rerr(struct aodv_pkt* pkt,uint8_t flag, uint8_t dest_count,
    struct unrecheable_dest** dests);

void aodv_pktbuild_rrep_ack(struct aodv_pkt* pkt);

static uint8_t aodv_pkt_receive_ttl(struct msghdr* msg);

#endif
