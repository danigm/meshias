#ifndef AODV_PACKET_H__
#define AODV_PACKET_H__

#include <asm/byteorder.h>

extern char* aodv_create_packet(u_int32_t dest_addr,u_int16_t dest_port,
        u_int8_t ttl,const char* payload,int payload_len);
#endif
