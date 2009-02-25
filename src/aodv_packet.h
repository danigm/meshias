#ifndef AODV_PACKET_H__
#define AODV_PACKET_H__

#include <asm/byteorder.h>

extern ssize_t aodv_send_packet(uint32_t dest_addr,
        uint8_t ttl,const char* payload,int payload_len);

extern loquesea aodv_recv_packet();

char* aodv_create_rrep(uint8_t flags, uint8_t prefix_sz, uint8_t hop_count,
        uint32_t dest_ip_addr, uint32_t dest_seq_num, uint32_t orig_ip_addr,
        uint32_t lifetime);

char* aodv_create_rreq(uint8_t flags, uint8_t hop_count, uint32_t rreq_id,
    uint32_t dest_ip_addr, uint32_t dest_seq_num, uint32_t orig_ip_addr,
    uint32_t orig_seq_num);

char* aodv_create_rerr(uint8_t flag, uint8_t dest_count,
    struct unrecheable_dest** dests);

char* aodv_create_rrep_ack();

#endif
