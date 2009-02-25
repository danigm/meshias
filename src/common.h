#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <asm/byteorder.h>

#define AODV_UDP_PORT 654

#include "log.h"

/* Types of aodv packets */
#define AODV_RREQ 1
#define AODV_RREP 2
#define AODV_RERR 3
#define AODV_RREP_ACK 4

/**
 * Errors and their corresponding error numbers
 * @ingroup Common
 */
#define ERR_INIT -1
#define ERR_SEND -1

/**
 * @name Common vars
 * Common vars have been defined as simple functions (@see rfc3561.txt)
 * @{
 */
uint32_t ACTIVE_ROUTE_TIMEOUT(); // Milliseconds

uint32_t ALLOWED_HELLO_LOSS();

uint32_t RREQ_RETRIES();

uint32_t HELLO_INTERVAL(); // Milliseconds

uint32_t LOCAL_ADD_TTL();

uint32_t NET_DIAMETER();

uint32_t RERR_RATELIMIT();

uint32_t RREQ_RATELIMIT();

uint32_t TIMEOUT_BUFFER();

/* The value of TTL when you create a new RREQ packet */
uint32_t TTL_START();

uint32_t TTL_INCREMENT();

uint32_t TTL_THRESHOLD();

uint32_t NODE_TRAVERSAL_TIME(); // Milliseconds

uint32_t NEXT_HOP_WAIT();

uint32_t MAX_REPAIR_TTL();

uint32_t MY_ROUTE_TIMEOUT();

uint32_t NET_TRAVERSAL_TIME();

uint32_t BLACKLIST_TIMEOUT();

uint32_t PATH_DISCOVERY_TIME();

uint32_t DELETE_PERIOD();

uint32_t MIN_REPAIR_TTL();

uint32_t TTL_VALUE();

uint32_t RING_TRAVERSAL_TIME();

/**
 * Given an amount of time in miliseconds, it sets the equivalent time in 
 * seconds and microseconds in @ref sc and @ref usc vars.
 */
void set_alarm_time(uint32_t miliseconds, unsigned long* sc,
    unsigned long* usc);

/**
 * Given an amount of time in seconds and miliseconds, it returns the
 * equivalent amount of time in miliseconds.
 */
uint32_t get_alarm_time(unsigned long sc, unsigned long usc);

/** @} */

/**
 * Route request message (RREQ)
 * @ingroup msg
 */
struct aodv_rreq
{
#if defined(__BIG_ENDIAN_BITFIELD)
    /**
     * Type will always be 1 in a RREQ
     */
    uint32_t type:8,
    
    /**
     * Flags have the format J R G D U. See above the possible flags
     */
    flags:5,
    
    /**
     * Is always zero and ignored
     */
    reserved:11,
    
    /**
     * Hop Count.
     * Number of hops from the originator ip Address to the node
     * handling the request.
     */
    hop_count:8;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
    uint32_t type:8,
        reserved2:3,
        flags:5,
        reserved:8,
        hop_count:8;
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


/**
 * Route response message (RREP)
 * @ingroup msg
 */
struct aodv_rrep
{
#if defined(__BIG_ENDIAN_BITFIELD)
    /**
     * Type will always be 2 in a RREP
     */
    uint32_t type:8,

    /**
     * Flags have the format is R A. See above the possible flags
     */
    flags:2,
    
    /**
     * Is always zero and ignored
     */    
    reserved:9,
    
    /**
     * Prefix size.
     * If nonzero, the 5-bit Prefix Size specifies that the indicated next hop
     * may be used for any nodes with the same routing prefix (as defined by
     * the Prefix Size) as the requested destination.
     */
    prefix_sz:5,

    /**
     * Hop Count.
     * Number of hops from the originator ip Address to the node
     * handling the request.
     */
    hop_count:8;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
    uint32_t type:8,
        reserved2:6,
        flags:2,
        prefix_sz:5,
        reserved:3,
        hop_count:8;
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



/**
 * Route Error (RERR)
 * @ingroup msg
 */
struct aodv_rerr
{
#if defined(__BIG_ENDIAN_BITFIELD)
    /**
     * Type will always be 3 in a RERR
     */
    uint32_t type:8,

    /**
     * The only flag is N. See above
     */
    flag:1,
    
    /**
     * Is always zero and ignored
     */    
    reserved:15,
    
    /**
     * The number of unrecheable destinations included in the message; MUST at
     * least be 1.
     */
    dest_count:8;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
    uint32_t type:8,
    reserved2:7,
    flag:1,
    reserved:8,
    dest_count:8;
        
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
struct unrecheable_dest
{
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

/**
 * Route Reply Acknowledgment (RREP-ACK)
 * @ingroup msg
 */
struct aodv_rrep_ack
{
    /**
     * Type will always be 4 in a RREP-ACK
     */
    uint8_t type;
    
    /**
     * Is always zero and ignored
     */
    uint8_t reserved;
};

#endif
