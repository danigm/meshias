#include <stdint.h>
#include <sys/time.h>
#include "alarm/linux_list.h"
#include "route_obj.h"
#include "common.h"

void __msh_route_updated(struct msh_route* route, uint32_t change_flag)
{
    if(route->updated_cb)
        (*route->updated_cb)(route, change_flag, route->cb_data);
}

void __msh_route_alarm_cb(struct alarm_block* alarm, void *qdata)
{
    struct msh_route* route = (struct msh_route*)qdata;
    unsigned long sc, usc;
    
    if(route->alarm_action == RTACTION_UNSET_VALID_ENTRY)
    {
        msh_route_unset_flag(route, RTFLAG_VALID_ENTRY);
        route->alarm_action = RTACTION_DESTROY;
        
        set_alarm_time(DELETE_PERIOD(), &sc, &usc);    
        add_alarm(alarm, sc, usc);

    } else if(route->alarm_action == RTACTION_DESTROY)
        msh_route_destroy(route);
}

struct msh_route* msh_route_alloc()
{
    struct msh_route* route = (struct msh_route*)
        calloc(1, sizeof(struct msh_route));
    
    init_alarm(&route->alarm, route, __msh_route_alarm_cb);
    INIT_LIST_HEAD(&(route->precursors_list.list));
    
    return route;
}

void msh_route_destroy(struct msh_route* route)
{
    struct precursor_t  *entry, *tmp;
    
    __msh_route_updated(route, RTACTION_DESTROY);
    del_alarm(&route->alarm);
    
    list_for_each_entry_safe(entry, tmp, &route->precursors_list.list, list)
    {
        list_del(&entry->list);
        free(entry);
    }
    
    free(route);
}

void msh_route_set_dst_ip(struct msh_route *route, struct in_addr dst_ip)
{
    printf("%p\n",route);
    route->dst_ip.s_addr = 0;
    puts("prueba");
    dst_ip.s_addr;
    puts("hola");
}

struct in_addr msh_route_get_dst_ip(struct msh_route *route)
{
    return route->dst_ip;
}

void msh_route_set_nexthop_ip(struct msh_route *route, struct in_addr nexthop_ip)
{
    route->nexthop_ip = nexthop_ip;
    __msh_route_updated(route, RTACTION_CHANGE_NEXTHOP_IP);
}

struct in_addr msh_route_get_nexthop_ip(struct msh_route *route)
{
    return route->nexthop_ip;
}

void msh_route_set_prefix_sz(struct msh_route *route, uint8_t prefix_sz)
{
    route->prefix_sz = prefix_sz;
}

uint8_t msh_route_get_prefix_sz(struct msh_route *route)
{
    return route->prefix_sz;
}

void msh_route_set_flag(struct msh_route *route, uint16_t flag)
{
    if(flag == RTFLAG_VALID_DEST_SEQ_NUM)
        route->flags |= RTFLAG_VALID_DEST_SEQ_NUM;
    else if(flag == RTFLAG_VALID_ENTRY)
        route->flags |= RTFLAG_VALID_ENTRY;
}


void msh_route_unset_flag(struct msh_route *route, uint16_t flag)
{
    if(flag == RTFLAG_VALID_DEST_SEQ_NUM)
        route->flags &= ~RTFLAG_VALID_DEST_SEQ_NUM;
    else if(flag == RTFLAG_VALID_ENTRY)
        route->flags &= ~RTFLAG_VALID_ENTRY;
}

uint16_t msh_route_get_flags(struct msh_route *route)
{
    return route->flags;
}

void msh_route_set_hop_count(struct msh_route *route, uint8_t hop_count)
{
    route->hop_count = hop_count;
}

uint8_t msh_route_get_hop_count(struct msh_route *route)
{
    return route->hop_count;
}

void msh_route_set_next_hop(struct msh_route *route, struct in_addr next_hop)
{
    route->next_hop = next_hop;
}

struct in_addr msh_route_get_next_hop(struct msh_route *route)
{
    return route->next_hop;
}

void msh_route_set_net_iface(struct msh_route *route, uint32_t net_iface)
{
    route->net_iface = net_iface;
}

uint32_t msh_route_get_net_iface(struct msh_route *route)
{
    return route->net_iface;
}

void msh_route__set_dest_seq_num(struct msh_route *route, uint32_t dest_seq_num)
{
    route->dest_seq_num = dest_seq_num;
}

uint32_t msh_route_get_dest_seq_num(struct msh_route *route)
{
    return route->dest_seq_num;
}

void msh_route_set_lifetime(struct msh_route *route, uint32_t lifetime)
{
    unsigned long sc, usc;
    
    msh_route_set_flag(route, RTFLAG_VALID_ENTRY);
    route->alarm_action = RTACTION_UNSET_VALID_ENTRY;
    
    set_alarm_time(lifetime, &sc, &usc);
    add_alarm(&route->alarm, sc, usc);
}

uint32_t msh_route_get_lifetime(struct msh_route *route)
{
    struct timeval now;
    
    gettimeofday(&now, NULL);
    if (!(route->flags & RTFLAG_VALID_ENTRY) ||
        !timercmp(&route->alarm.tv, &now, >))
        return 0;
    
    timersub(&now, &route->alarm.tv, &now);
    
    return get_alarm_time(now.tv_sec, now.tv_usec);
}

void msh_route_set_rtnl_route(struct msh_route *route, struct rtnl_route *nlroute)
{
    route->nlroute = nlroute;
}

struct rtnl_route *msh_route_get_rtnl_route(struct msh_route *route)
{
    return route->nlroute;
}

void msh_route_add_precursor(struct msh_route *route, struct in_addr dst_ip)
{
    // Check the precursor is not already added
    struct precursor_t *entry;
    
    list_for_each_entry(entry, &route->precursors_list.list, list)
    {
        if(entry->dst_ip.s_addr == dst_ip.s_addr)
            return;
    }
    
    // Entry not found; adding it
    entry = (struct precursor_t *)calloc(1, sizeof(struct precursor_t));
    entry->dst_ip.s_addr = dst_ip.s_addr;
    list_add(&entry->list, &route->precursors_list.list);
}

void msh_route_del_precursor(struct msh_route *route, struct in_addr dst_ip)
{
    // Find the entry
    struct precursor_t *entry;
    
    list_for_each_entry(entry, &route->precursors_list.list, list)
    {
        if(entry->dst_ip.s_addr == dst_ip.s_addr)
        {
            // Found!
            list_del(&entry->list);
            free(entry);
            return;
        }
    }
}

void msh_route_foreach_precursor(struct msh_route *route,
    int (*callback_func)(struct msh_route *, struct in_addr *, void *), void *data)
{
    // Find the entry
    struct precursor_t *entry;
    
    list_for_each_entry(entry, &route->precursors_list.list, list)
    {
        (*callback_func)(route, &entry->dst_ip, data);
    }
}

int msh_route_compare(struct msh_route *first, struct msh_route *second,
    int attr_flags)
{
    uint32_t diff = 0;
    
    // See if dest ip of first route is contained by the destination group
    // of the second route.
    if(attr_flags & RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING)
    {
        uint32_t mask = 32 - second->prefix_sz;
        uint32_t ip1mask = first->dst_ip.s_addr & mask;
        uint32_t ip2mask = second->dst_ip.s_addr & mask;
        diff |= (ip1mask == ip2mask);
    }

    if(attr_flags & RTATTR_DST_IP)
        diff |= (first->dst_ip.s_addr != second->dst_ip.s_addr);
    
    if(attr_flags & RTATTR_PREFIX_SZ)
        diff |= (first->prefix_sz != second->prefix_sz);
    
    if(attr_flags & RTATTR_DEST_SEQ_NUM)
        diff |= (first->dest_seq_num != second->dest_seq_num);
    
    if(attr_flags & RTATTR_FLAGS)
        diff |= (first->flags != second->flags);
    
    if(attr_flags & RTATTR_HOP_COUNT)
        diff |= (first->hop_count != second->hop_count);
    
    if(attr_flags & RTATTR_NEXT_HOP)
        diff |= (first->next_hop.s_addr != second->next_hop.s_addr);
    
    if(attr_flags & RTATTR_NET_IFACE)
        diff |= (first->net_iface != second->net_iface);

    if(attr_flags & RTATTR_LIFETIME)
        diff |= timercmp(&first->alarm.tv, &second->alarm.tv, !=);
    
    if(attr_flags & RTATTR_UPDATED_CB)
        diff |= (first->updated_cb != second->updated_cb);
    
    if(attr_flags & RTATTR_CB_DATA)
        diff |= (first->cb_data != second->cb_data);
    
    return diff;
}

