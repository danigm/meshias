#include <stdio.h> 
#include <netinet/in.h>
#include <netlink/addr.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "common.h"

void set_alarm_time(uint32_t miliseconds, unsigned long* sc, unsigned long* usc)
{
    *sc = (unsigned long)(miliseconds / 1000);
    *usc = (miliseconds % 1000) * 1000;
}


uint32_t get_alarm_time(unsigned long sc, unsigned long usc)
{
    return (uint32_t)usc/1000 + sc*1000;
}


struct nl_addr* in_addr2nl_addr(struct in_addr addr, uint8_t prefix_sz)
{
    char buf[256];
    if(prefix_sz > 0)
      sprintf(buf, "%s/%d", inet_htoa(addr), prefix_sz);
    else
      sprintf(buf, "%s", inet_htoa(addr));
    
    return nl_addr_parse(buf, AF_INET);
}

char *inet_htoa(struct in_addr addr)
{
    struct in_addr addr_net = { htonl(addr.s_addr) };
    return inet_ntoa(addr_net);
}
