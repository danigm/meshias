#ifndef AODV_DAEMON_H
#define AODV_DAEMON_H

#include "msh_data.h"
#include "common.h"

int daemon_init();
void daemon_shutdown();

void daemon_receive_packets();

int aodv_get_type(const char* b);
int aodv_check_packet(const char* b);

void aodv_recv_rreq(const char *b,const struct sockaddr_in* source);
void aodv_recv_rrep(const char *b,const struct sockaddr_in* source);
void aodv_recv_rerr(const char *b,const struct sockaddr_in* source);
void aodv_recv_rrep_ack(const char *b,const struct sockaddr_in* source);

int aodv_send_rreq(struct aodv_rreq* to_sent,char ttl);
int aodv_send_rerr(struct aodv_rerr* to_sent);
int aodv_send_rrep(struct aodv_rrep* to_sent);
int aodv_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl);

#endif
