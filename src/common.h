#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>


/**
 * Errors and their corresponding error numbers
 * @ingroup Common
 */
#define ERR_INIT -1

/**
 * @name Common vars
 * Common vars have been defined as simple functions (@see rfc3561.txt)
 * @{
 */
int ACTIVE_ROUTE_TIMEOUT(); // Milliseconds

int ALLOWED_HELLO_LOSS();

int RREQ_RETRIES();

int HELLO_INTERVAL(); // Milliseconds

int LOCAL_ADD_TTL();

int NET_DIAMETER();

int RERR_RATELIMIT();

int RREQ_RATELIMIT();

int TIMEOUT_BUFFER();

int TTL_START();

int TTL_INCREMENT();

int TTL_THRESHOLD();

int NODE_TRAVERSAL_TIME(); // Milliseconds

int NEXT_HOP_WAIT();

int MAX_REPAIR_TTL();

int MY_ROUTE_TIMEOUT();

int NET_TRAVERSAL_TIME();

int BLACKLIST_TIMEOUT();

int PATH_DISCOVERY_TIME();

int DELETE_PERIOD();

int MIN_REPAIR_TTL();

int TTL_VALUE();

int RING_TRAVERSAL_TIME();

/** @} */

/**
 * Route request message (RREQ)
 * @ingroup msg
 */
struct aodv_rreq
{
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
    prefiz_sz:5,

    /**
     * Hop Count.
     * Number of hops from the originator ip Address to the node
     * handling the request.
     */
    hop_count:8;
    
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
    
    /**
     * Array containing as many unrecheable destinations as specified in
     * dest_count.
     */
    struct unrechable_dest
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
    } *destinations;
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