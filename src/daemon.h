#ifndef AODV_DAEMON_H
#define AODV_DAEMON_H

#include "msh_data.h"
#include "common.h"

/* Initialize the aodv's stuff like socket */
int daemon_init();
/* Free reserved memory */
void daemon_shutdown();

/* Function to receive packets and make an answer */
void daemon_receive_packets();

/* Get type of a aodv packet */
int aodv_get_type(const char* b);
/* Verify the aodv packet is correctly built */
int aodv_check_packet(const char* b);

/* Funtions to make an answer from receive packets */
void aodv_recv_rreq(const char *b,const struct sockaddr_in* source);
void aodv_recv_rrep(const char *b,const struct sockaddr_in* source);
void aodv_recv_rerr(const char *b,const struct sockaddr_in* source);
void aodv_recv_rrep_ack(const char *b,const struct sockaddr_in* source);

/* Send aodv rreq packet */
int aodv_send_rreq(struct aodv_rreq* to_sent,char ttl);
int aodv_sent_new_rreq(uint8_t flags,uint8_t hop_count,uint32_t rreq_id,
        uint32_t dest_ip_addr,uint32_t seq_number,uint32_t orig_ip_addr,
        uint32_t orig_seq_number);

/* Send aodv rerr packet */
int aodv_send_rerr(struct aodv_rerr* to_sent);
int aodv_send_new_rerr(uint8_t flag,uint8_t dest_count,
        struct unrecheable_dest* dests);

/* Send aodv rrep packet */
int aodv_send_rrep(struct aodv_rrep* to_sent);
int aodv_send_new_rrep(uint8_t flags,uint8_t prefix_sz,uint8_t hop_count,
        uint32_t dest_ip_addr,uint32_t dest_seq_number,uint32_t orig_ip_addr,
        uint32_t lifetime);
int aodv_send_new_rerr(uint8_t flag,uint8_t dest_count,
        struct unrecheable_dest* dests);

/* Send aodv rrep_ack packet */
int aodv_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl);

#endif
