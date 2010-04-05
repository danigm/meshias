#ifndef UTILS_H
#define UTILS_H

#include <netinet/in.h>
#include <netlink/addr.h>

/**
 * Given an amount of time in miliseconds, it sets the equivalent time in
 * seconds and microseconds in @ref sc and @ref usc vars.
 */
void set_alarm_time(uint32_t miliseconds, unsigned long* sc,
                    unsigned long* usc);

/**
 * Given an amount of time in seconds and microseconds, it returns the
 * equivalent amount of time in miliseconds.
 */
uint32_t get_alarm_time(unsigned long sc, unsigned long usc);

struct nl_addr* in_addr2nl_addr(struct in_addr addr, uint8_t prefix_sz);

char *inet_htoa(struct in_addr addr);

#endif
