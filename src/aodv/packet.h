#ifndef PACKET_H
#define PACKET_H

#include <asm/byteorder.h>

#include "../log.h"
#include "../utils.h"

/** Types of aodv packets */
#define AODV_RREQ 1
#define AODV_RREP 2
#define AODV_RERR 3
#define AODV_RREP_ACK 4

/////////////////////////////////
// RREP
/////////////////////////////////
/**
 * Route response message (RREP)
 * @ingroup msg
 */
struct aodv_rrep {
#if defined(__BIG_ENDIAN_BITFIELD)
    /**
     * Type will always be 2 in a RREP
     */
    uint32_t type: 8,

             /**
              * Flags have the format is R A. See above the possible flags
              */
             flags: 2,

             /**
              * Is always zero and ignored
              */
             reserved: 9,

             /**
              * Prefix size.
              * If nonzero, the 5-bit Prefix Size specifies that the indicated next hop
              * may be used for any nodes with the same routing prefix (as defined by
              * the Prefix Size) as the requested destination.
              */
             prefix_sz: 5,

             /**
              * Hop Count.
              * Number of hops from the originator ip Address to the node
              * handling the request.
              */
             hop_count: 8;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
    uint32_t type: 8,
             reserved2: 6,
             flags: 2,
             prefix_sz: 5,
             reserved: 3,
             hop_count: 8;
#else
#error  "Adjust your <asm/byteorder.h> defines"
#endif

    /**
     * Destination IP Address.
     * The IP address of the destination for which a route is supplied.
     */
    uint32_t dest_ip_addr;

    /**
     * Destination Sequence Number.
     * The destination sequence number associated to the route.
     */
    uint32_t dest_seq_num;

    /**
     * Originator IP Address.
     * The IP address of the node which originated the RREQ for which the route
     * is supplied.
     */
    uint32_t orig_ip_addr;

    /**
     * The time in miliseconds for which nodes receiving the RREP consider the
     * route to be valid.
     */
    uint32_t lifetime;
};

/**
 * @name RREP Message flags
 * @{
 */

/**
 * R, Repair flag; used for multicast.
 */
#define RREP_REPAIR_FLAG            2

/**
 * A, Aknowledgement required.
 */
#define RREP_ACK_REQUIRED_FLAG      1

/** @} */

/////////////////////////////////
// RREQ_ACK
/////////////////////////////////
/**
 * Route Reply Acknowledgment (RREP-ACK)
 * @ingroup msg
 */
struct aodv_rrep_ack {
    /**
     * Type will always be 4 in a RREP-ACK
     */
    uint8_t type;

    /**
     * Is always zero and ignored
     */
    uint8_t reserved;
};

/////////////////////////////////
// RREQ
/////////////////////////////////
/**
 * Route request message (RREQ)
 * @ingroup msg
 */
struct aodv_rreq {
#if defined(__BIG_ENDIAN_BITFIELD)
    /**
     * Type will always be 1 in a RREQ
     */
    uint32_t type: 8,

             /**
              * Flags have the format J R G D U. See above the possible flags
              */
             flags: 5,

             /**
              * Is always zero and ignored
              */
             reserved: 11,

             /**
              * Hop Count.
              * Number of hops from the originator ip Address to the node
              * handling the request.
              */
             hop_count: 8;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
    uint32_t type: 8,
             reserved2: 3,
             flags: 5,
             reserved: 8,
             hop_count: 8;
#else
#error  "Adjust your <asm/byteorder.h> defines"
#endif


    /**
     * RREQ ID.
     * A sequence number uniquely identifying the particular RREQ when taken
     * in conjuntion with the originating node's IP address.
     */
    uint32_t rreq_id;

    /**
     * Destination IP Address.
     * The IP address of the destination for which a route is desired.
     */
    uint32_t dest_ip_addr;

    /**
     * Destination Sequence Number.
     * The latest sequence number received in the past by the originator for
     * any route towards the destination.
     */
    uint32_t dest_seq_num;

    /**
     * Originator IP Address.
     * The IP address of the node which originated the Route Request.
     */
    uint32_t orig_ip_addr;

    /**
     * Originator Sequence Number.
     * The current sequence number to be used in the route entry pointing
     * towards the originator of the route request.
     */
    uint32_t orig_seq_num;
};

/**
 * @name RREQ Message flags
 * @{
 */

/**
 * J, Join flag; reserved for multicast.
 */
#define JOIN_FLAG                   16


/**
 * R, Repair flag; reserved for multicast.
 */
#define RREQ_REPAIR_FLAG             8

/**
 * G, Gratuitous RREP flag; indicates whether a gratuitous RREP should be
 * unicast to the node specified in the Destination IP Address field.
 */
#define RREQ_GRATUITOUS_RREP_FLAG    4

/**
 * D, Destination only flag; indicates only the destination may respond to this
 * RREQ.
 */
#define RREQ_DEST_ONLY_FLAG          2

/**
 * U, Unknown sequence number; indicates the destination sequence number is
 * unknown.
 */
#define RREQ_UNKNOWN_SEQ_NUM_FLAG    1
/** @} */

/////////////////////////////////
// RERR
/////////////////////////////////
/**
 * Route Error (RERR)
 * @ingroup msg
 */
struct aodv_rerr {
#if defined(__BIG_ENDIAN_BITFIELD)
    /**
     * Type will always be 3 in a RERR
     */
    uint32_t type: 8,

             /**
              * The only flag is N. See above
              */
             flag: 1,

             /**
              * Is always zero and ignored
              */
             reserved: 15,

             /**
              * The number of unrecheable destinations included in the message; MUST at
              * least be 1.
              */
             dest_count: 8;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
    uint32_t type: 8,
             reserved2: 7,
             flag: 1,
             reserved: 8,
             dest_count: 8;

#else
#error  "Adjust your <asm/byteorder.h> defines"
#endif

};

/**
 * @name RERR Message flag
 * @{
 */

/**
 * N, No delete flag; set when a node has performed a local repair of a link,
 * and upstream nodes should not delete the route.
 */
#define RERR_NO_DELETE_FLAG            1

/** @} */

/**
 * This structure is used with aodv_rerr to send a complete error
 * message
 */
struct unrecheable_dest {
    /**
    * Destination IP Address.
    * The IP address of the destination for which a route is supplied.
    */
    uint32_t ip_addr;

    /**
     * Destination Sequence Number.
     * The destination sequence number associated to the route.
     */
    uint32_t seq_num;
};

struct aodv_pkt;

struct aodv_pkt *aodv_pkt_alloc();

struct aodv_pkt *aodv_pkt_get(struct msghdr* msgh, int received);

ssize_t aodv_pkt_send(struct aodv_pkt* pkt);

void aodv_pkt_destroy(struct aodv_pkt* pkt);

uint8_t aodv_pkt_get_ttl(struct aodv_pkt* pkt);

void aodv_pkt_set_ttl(struct aodv_pkt* pkt, uint8_t ttl);

void aodv_pkt_decrease_ttl(struct aodv_pkt *pkt);

// NOTE in this function the port is not returned
// only address in big endian is returned
uint32_t aodv_pkt_get_address(struct aodv_pkt* pkt);

// addr must be in big endian
void aodv_pkt_set_address(struct aodv_pkt* pkt, uint32_t addr);

char* aodv_pkt_get_payload(struct aodv_pkt *pkt);

int aodv_pkt_get_payload_len(struct aodv_pkt *pkt);

int aodv_pkt_get_type(struct aodv_pkt *pkt);

int aodv_pkt_check(struct aodv_pkt* pkt);

size_t aodv_pkt_get_size(struct aodv_pkt* pkt);

uint8_t aodv_pkt_receive_ttl(struct msghdr* msg);

void aodv_pkt_build_rrep(struct aodv_pkt* pkt, uint8_t flags, uint8_t prefix_sz,
                         uint8_t hop_count, uint32_t dest_ip_addr, uint32_t dest_seq_num,
                         uint32_t orig_ip_addr, uint32_t lifetime);

void aodv_pkt_prepare_rrep(struct aodv_rrep* rrep);

void aodv_pkt_build_rrep_ack(struct aodv_pkt* pkt);

int aodv_pkt_send_rrep_ack(struct aodv_rrep_ack* to_sent, int ttl);

void aodv_pkt_build_rreq(struct aodv_pkt* pkt, uint8_t flags, uint8_t hop_count,
                         uint32_t rreq_id, uint32_t dest_ip_addr, uint32_t dest_seq_num,
                         uint32_t orig_ip_addr, uint32_t orig_seq_num);

void aodv_pkt_prepare_rreq(struct aodv_rreq* rreq);

void aodv_pkt_build_rerr(struct aodv_pkt* pkt, uint8_t flag, uint8_t dest_count,
                         struct unrecheable_dest** dests);

#endif
