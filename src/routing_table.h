#ifndef _ROUTING_TABLE_H_
#define _ROUTING_TABLE_H_

#include <netlink/cache.h>
#include <netlink/addr.h>
#include <netlink/object-api.h> // For NLHDR_COMMON


struct msh_route
{
    NLHDR_COMMON
    
    uint32_t dst_ip;
    uint8_t prefix_sz;
    uint32_t dest_seq_num;
    uint16_t flags;
    uint32_t net_iface;
    uint8_t hop_count;
    uint32_t next_hop;
    uint32_t* precursors_list;
    uint32_t lifetime;
};


#endif
