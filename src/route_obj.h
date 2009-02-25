#ifndef _ROUTE_OBJ_H_
#define _ROUTE_OBJ_H_

#include <netinet/in.h>

struct msh_route;

/**
 * These are the possible ways a route can be flagged
 * @see msh_route_set_flag
 * @see msh_route_unset_flag
 * @see msh_route_get_flags
 */
#define RTFLAG_VALID_DEST_SEQ_NUM           0x0001
#define RTFLAG_VALID_ENTRY                  0x0002
// #define RTFLAG_REPAIRABLE                   0x0004
// #define RTFLAG_BEING_REPAIRED               0x0008

/**
 * These can be used as the flags in a comparation
 * @see msh_route_compare
 */
#define RTATTR_DST_IP                       0x0001
#define RTATTR_PREFIX_SZ                    0x0002
#define RTATTR_DEST_SEQ_NUM                 0x0004
#define RTATTR_FLAGS                        0x0008
#define RTATTR_HOP_COUNT                    0x0010
#define RTATTR_NEXT_HOP                     0x0020
#define RTATTR_NET_IFACE                    0x0040
#define RTATTR_LIFETIME                     0x0080
#define RTATTR_UPDATED_CB                   0x0100
#define RTATTR_CB_DATA                      0x0200

#define RTACTION_SET_VALID_DEST_SEQ_NUM     0x0001
#define RTACTION_UNSET_VALID_DEST_SEQ_NUM   0x0002
#define RTACTION_SET_VALID_ENTRY            0x0004
#define RTACTION_UNSET_VALID_ENTRY          0x0008
#define RTACTION_DESTROY                    0x0010

struct msh_route* msh_route_alloc();
void msh_route_destroy(struct msh_route* route);

void msh_route_set_dst_ip(struct msh_route *route, struct in_addr dst_ip);
struct in_addr msh_route_get_dst_ip(struct msh_route *route);

void msh_route_set_prefix_sz(struct msh_route *route, uint8_t prefix_sz);
uint8_t msh_route_get_prefix_sz(struct msh_route *route);

void msh_route_set_flag(struct msh_route *route, uint16_t flag);
void msh_route_unset_flag(struct msh_route *route, uint16_t flag);
uint16_t msh_route_get_flags(struct msh_route *route);

void msh_route_set_hop_count(struct msh_route *route, uint8_t hop_count);
uint8_t msh_route_get_hop_count(struct msh_route *route);

void msh_route_set_next_hop(struct msh_route *route, struct in_addr next_hop);
struct in_addr msh_route_get_next_hop(struct msh_route *route);

void msh_route__set_net_iface(struct msh_route *route, uint32_t net_iface);
uint32_t msh_route_get_net_iface(struct msh_route *route);

void msh_route_set_lifetime(struct msh_route *route, uint32_t lifetime);
uint32_t msh_route_get_lifetime(struct msh_route *route);

/**
 * Compare two msh_route's only by the attributes indicated by attr_flags. Returns
 * 0 if a match is found, or not-zero otherwise.
 */
int msh_route_compare(struct msh_route *first, struct msh_route *second,
    int attr_flags);

#endif
