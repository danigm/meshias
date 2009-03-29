#include <sys/time.h>

#include "rreq_fifo.h"
#include "packets_fifo.h"
#include "aodv_logic.h"
#include "daemon.h"
#include "route_obj.h"
#include "aodv_packet.h"
#include "statistics.h"

void aodv_find_route(struct in_addr dest, struct msh_route *invalid_route,
    uint8_t prev_tries)
{
    //First, check that prev_tries hasn't exceeded its limit.
    if(prev_tries > RREQ_RETRIES())
    {
        // Route to the dest definitely not found: drop packets
        packets_fifo_drop_packets(data.packets_queue, dest);
        stats.packets_dropped++;
        return;
    }
    
    // TODO: Second, Do not send more than RREQ_RATELIMIT() RREQs per second.
    // Buffer the RREQs when they can't be sent
    
    // we must increment our sequence number before generating a RREQ (see RFC
    // 3561 Page 11)
    data.seq_num++;
    
    // We are going to send a new RREQ, so we increment the global rreq id
    // counter
    data.rreq_id++;
        
    // Third, buffer the rreq we're about to add. Any previous rreq
    // originating from ourselves should have been already added to the queue
    // already so we won't add it there; it's not needed. So we only add the
    // rreq to the RREQ FIFO if this is the first try i.e. prev_tries = 0
    if(prev_tries == 0)
    {
        rreq_fifo_push_owned(data.rreq_queue, data.rreq_id, dest, prev_tries);
    }
    
    struct aodv_pkt* pkt = aodv_pkt_alloc();
    uint8_t hop_count = (invalid_route) ?
        msh_route_get_hop_count(invalid_route) : 0;
    uint32_t dest_seq_num = (invalid_route) ?
        msh_route_get_dest_seq_num(invalid_route) : 0;
    
    // We always want bidirectional communications
    uint8_t flags = RREQ_GRATUITOUS_RREP_FLAG;
    flags |= (!invalid_route) ? RREQ_UNKNOWN_SEQ_NUM_FLAG : 0;
    
    aodv_pkt_set_ttl(pkt, expanding_ring_search_ttl(hop_count, prev_tries));

    
    // After gathering all the needed information, we can build our rreq
    aodv_pkt_build_rreq(pkt, flags, hop_count, data.rreq_id, dest.s_addr,
            dest_seq_num, data.ip_addr.s_addr, data.seq_num);
    
    // Finally we send the packet
    printf("sending broadcast RREQ: ");
    aodv_pkt_send(pkt);
    
    aodv_pkt_destroy(pkt);
}

void aodv_process_rreq(struct aodv_pkt* pkt)
{
    // (See page 16 of RFC 3561)
    // First create/update a route to the previous hop without a valid sequence
    // number
    //TODO: is this the prev hop or the orig??
    struct in_addr addr;
    addr.s_addr = aodv_pkt_get_address(pkt);
    
    struct msh_route *route = routing_table_find_by_ip(data.routing_table, addr);
    
    // If found a route for the prev_hop, update it
    if(route)
    {
        // reset the lifetime and mark as valid
        msh_route_set_lifetime(route, ACTIVE_ROUTE_TIMEOUT());
    }
    else
    {
        // If route for prev_hop not found, create it
        route = msh_route_alloc();
        msh_route_set_dst_ip(route, addr);
        routing_table_add(data.routing_table, route);
    }
    
    // Second, check if the rreq_queue has this rreq buffered. If so, we must
    // discard it.
    struct aodv_rreq* rreq = (struct aodv_rreq*)aodv_pkt_get_payload(pkt);
    struct in_addr dest_addr;
    dest_addr.s_addr = rreq->dest_ip_addr;
    if(rreq_fifo_contains(data.rreq_queue, rreq->rreq_id,
        dest_addr))
        return;
    
    // As this RREQ is new, we should add it to the queue
    rreq_fifo_push(data.rreq_queue, rreq->rreq_id, dest_addr);
        

    uint8_t hop_count = rreq->hop_count;
    rreq->hop_count = hop_count + 1;
    
    // searches for a reverse route to the Originator IP Address (see
    // section 6.2), using longest-prefix matching.  If need be, the route
    // is created, or updated using the Originator Sequence Number from the
    // RREQ in its routing table.
    struct in_addr addr_orig = { rreq->orig_ip_addr };
    route = routing_table_find_by_ip(data.routing_table, addr_orig);
    
    uint32_t seq_num_new = rreq->orig_seq_num;
    uint32_t min_lifetime = minimal_lifetime(hop_count);
    // If found a route for the orig ip, update it
    if(route)
    {
        // reset the lifetime and mark as valid
        uint32_t seq_num_old = msh_route_get_dest_seq_num(route);
        uint32_t lifetime_old = msh_route_get_lifetime(route);
        
        msh_route_set_flag(route, RTFLAG_VALID_DEST_SEQ_NUM);
        msh_route_set_next_hop(route, addr);
        msh_route_set_hop_count(route, hop_count);
        
        //signed comparation as explained in RFC 3561 page 11
        if((int32_t)seq_num_old < (int32_t)seq_num_new)
            msh_route_set_dest_seq_num(route, seq_num_new);
        if(min_lifetime > lifetime_old)
            msh_route_set_lifetime(route, min_lifetime);
        
    }
    else
    {
        // If route for orig ip not found, create it
        route = msh_route_alloc();
        msh_route_set_dst_ip(route, addr_orig);
        msh_route_set_dest_seq_num(route, seq_num_new);
        msh_route_set_next_hop(route, addr);
        msh_route_set_hop_count(route, hop_count);
        msh_route_set_lifetime(route, min_lifetime);
        msh_route_set_flag(route, RTFLAG_VALID_DEST_SEQ_NUM);
        routing_table_add(data.routing_table, route);
    }
    
//    If a node does not generate a RREP (following the processing rules in
//    section 6.6), and if the incoming IP header has TTL larger than 1,
//    the node updates and broadcasts the RREQ to address 255.255.255.255
//    on each of its configured interfaces (see section 6.14).  To update
//    the RREQ, the TTL or hop limit field in the outgoing IP header is
//    decreased by one, and the Hop Count field in the RREQ message is
//    incremented by one, to account for the new hop through the
//    intermediate node.  Lastly, the Destination Sequence number for the
//    requested destination is set to the maximum of the corresponding
//    value received in the RREQ message, and the destination sequence
//    value currently maintained by the node for the requested destination.
//    However, the forwarding node MUST NOT modify its maintained value for
//    the destination sequence number, even if the value received in the
//    incoming RREQ is larger than the value currently maintained by the
//    forwarding node.
    if(!aodv_answer_to_rreq(rreq) && aodv_pkt_get_ttl(pkt) > 1)
    {
        aodv_pkt_set_address(pkt, data.broadcast_addr.s_addr);
        aodv_pkt_decrease_ttl(pkt);
        rreq->hop_count++;
        
        route = routing_table_find_by_ip(data.routing_table, dest_addr);
        
        // If we didn't send a rrep it's because we either don't have a route
        // for the destination or it's marked as invalid. If it's marked as
        // invalid, we might need to update the rreq's dest seq num before
        // broadcasting it.
        if(route && !(msh_route_get_flags(route) & RTFLAG_VALID_ENTRY) &&
                (msh_route_get_flags(route) & RTFLAG_VALID_DEST_SEQ_NUM)) 
        {
            uint32_t dest_seq_num_old = msh_route_get_dest_seq_num(route);
            uint32_t dest_seq_num_new = ntohl(rreq->orig_seq_num);
            if(dest_seq_num_old < dest_seq_num_new)
                msh_route_set_dest_seq_num(route, dest_seq_num_new);
                
        }
        aodv_pkt_send(pkt);
    }
}

uint8_t aodv_answer_to_rreq(struct aodv_rreq* rreq)
{
//    Check if we're going to answer the RREQ A node generates a RREP if either:
    struct in_addr dest_addr = { rreq->dest_ip_addr };
    // We the destination of the RREQ
    if(dest_addr.s_addr == data.ip_addr.s_addr)
    {
//    6.6.1. Route Reply Generation by the Destination
// 
//    If the generating node is the destination itself, it MUST increment
//    its own sequence number by one if the sequence number in the RREQ
//    packet is equal to that incremented value.  Otherwise, the
//    destination does not change its sequence number before generating the
//    RREP message.  The destination node places its (perhaps newly
//    incremented) sequence number into the Destination Sequence Number
//    field of the RREP, and enters the value zero in the Hop Count field
//    of the RREP.
//
//    The destination node copies the value MY_ROUTE_TIMEOUT (see section
//    10) into the Lifetime field of the RREP.  Each node MAY reconfigure
//    its value for MY_ROUTE_TIMEOUT, within mild constraints (see section
//    10).
        struct aodv_pkt* pkt = aodv_pkt_alloc();
        if(rreq->dest_seq_num == data.seq_num)
            data.seq_num++;
        
        aodv_pkt_set_address(pkt, rreq->orig_ip_addr);
        aodv_pkt_build_rrep(pkt, 0, 0, 0, data.ip_addr.s_addr, data.seq_num,
            rreq->orig_ip_addr, MY_ROUTE_TIMEOUT());
        aodv_pkt_send(pkt);
        aodv_pkt_destroy(pkt);
    }

//    (ii)      it has an active route to the destination, the destination
//              sequence number in the node's existing route table entry
//              for the destination is valid and greater than or equal to
//              the Destination Sequence Number of the RREQ (comparison
//              using signed 32-bit arithmetic), and the "destination only"
//              ('D') flag is NOT set.
    struct msh_route *route_to_dest = 0;
    if((route_to_dest = routing_table_find_by_ip(data.routing_table, dest_addr)) != 0 &&
        rreq->dest_seq_num >= data.seq_num &&
        !(rreq->flags & RREQ_DEST_ONLY_FLAG))
    {
// 6.6.2. Route Reply Generation by an Intermediate Node
// 
//    If the node generating the RREP is not the destination node, but
//    instead is an intermediate hop along the path from the originator to
//    the destination, it copies its known sequence number for the
//    destination into the Destination Sequence Number field in the RREP
//    message.
// 
//    The intermediate node updates the forward route entry by placing the
//    last hop node (from which it received the RREQ, as indicated by the
//    source IP address field in the IP header) into the precursor list for
//    the forward route entry -- i.e., the entry for the Destination IP
//    Address.  The intermediate node also updates its route table entry
//    for the node originating the RREQ by placing the next hop towards the
//    destination in the precursor list for the reverse route entry --
//    i.e., the entry for the Originator IP Address field of the RREQ
//    message data.
// 
//    The intermediate node places its distance in hops from the
//    destination (indicated by the hop count in the routing table) Count
//    field in the RREP.  The Lifetime field of the RREP is calculated by
//    subtracting the current time from the expiration time in its route
//    table entry.
        struct aodv_pkt* pkt = aodv_pkt_alloc();
        uint32_t known_dest_seq_num = msh_route_get_dest_seq_num(route_to_dest);
        uint32_t dest_hop_count = msh_route_get_hop_count(route_to_dest);
        
        struct timeval now;
        gettimeofday(&now, NULL);
        uint32_t lifetime = msh_route_get_lifetime(route_to_dest) -
            get_alarm_time(now.tv_sec, now.tv_usec);
        
        aodv_pkt_set_address(pkt, dest_addr.s_addr);
        aodv_pkt_build_rrep(pkt, 0, 0, dest_hop_count, dest_addr.s_addr,
            known_dest_seq_num, rreq->orig_ip_addr, lifetime);
        
        aodv_pkt_destroy(pkt);
        
        if(rreq->flags & RREQ_GRATUITOUS_RREP_FLAG)
        {
//         6.6.3. Generating Gratuitous RREPs
// 
//    After a node receives a RREQ and responds with a RREP, it discards
//    the RREQ.  If the RREQ has the 'G' flag set, and the intermediate
//    node returns a RREP to the originating node, it MUST also unicast a
//    gratuitous RREP to the destination node.
//
//    Hop Count                        The Hop Count as indicated in the
//                                     node's route table entry for the
//                                     originator
// 
//    Destination IP Address           The IP address of the node that
//                                     originated the RREQ
// 
//    Destination Sequence Number      The Originator Sequence Number from
//                                     the RREQ
// 
//    Originator IP Address            The IP address of the Destination
//                                     node in the RREQ
//
//    Lifetime                         The remaining lifetime of the route
//                                     towards the originator of the RREQ,
//                                     as known by the intermediate node.
// 
//    The gratuitous RREP is then sent to the next hop along the path to
//    the destination node, just as if the destination node had already
//    issued a RREQ for the originating node and this RREP was produced in
//    response to that (fictitious) RREQ.  The RREP that is sent to the
//    originator of the RREQ is the same whether or not the 'G' bit is set.
            struct aodv_pkt* pkt = aodv_pkt_alloc();
            struct msh_route *route_to_orig =
                routing_table_find_by_ip(data.routing_table, dest_addr);
            uint32_t orig_hop_count = msh_route_get_hop_count(route_to_orig);
            uint32_t orig_lifetime = msh_route_get_lifetime(route_to_orig);
            aodv_pkt_set_address(pkt, dest_addr.s_addr);
            aodv_pkt_build_rrep(pkt, 0, 0, orig_hop_count, rreq->orig_ip_addr,
            rreq->orig_seq_num, dest_addr.s_addr, orig_lifetime);
            
            aodv_pkt_destroy(pkt);
        }
    }
    
    return 0;
}

void aodv_process_rrep(struct aodv_pkt* pkt)
{
    
}

void aodv_process_rerr(struct aodv_pkt* pkt)
{
    
}

void aodv_process_rrep_ack(struct aodv_pkt* pkt)
{
    
}
