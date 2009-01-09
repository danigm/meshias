#ifndef _AODV_H_
#define _AODV_H_

int init_aodv();
void shutdown_aodv();

void aodv_create_route(u_int32_t route);
void aodv_read_pkg(char* packet);

#endif
