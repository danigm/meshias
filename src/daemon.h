#ifndef AODV_DAEMON_H
#define AODV_DAEMON_H

#include "msh_data.h"
#include "common.h"

int daemon_init();
void daemon_shutdown();

int aodv_send_rreq(struct aodv_rreq* to_sent,char ttl);
int aodv_send_rerr(struct aodv_rerr* to_sent);
int aodv_send_rrep(struct aodv_rrep* to_sent);
int aodv_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl);

#endif
