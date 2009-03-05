#include <stdio.h> 
#include <netinet/in.h>
#include <netlink/addr.h>
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


struct nl_addr* in_addr2nl_addr(struct in_addr *addr, uint8_t prefix_sz)
{
    char buf[256];
    sprintf(buf, "%s/%d", (char *)inet_ntoa(addr), (int)32 - prefix_sz);
    
    return nl_addr_parse(buf, AF_INET);
}
