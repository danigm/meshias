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
    //First, check that preev_tries hasn't exceeded its limit.
    if(prev_tries > RREQ_RETRIES())
    {
        // Route to the dest definitely not found: drop packets
        packets_fifo_drop_packets(data.packets_queue, dest);
        stats.packets_dropped++;
        return;
    }
    
    
    // TODO: Second, Do not send more than RREQ_RATELIMIT() RREQs per second.
    // Buffer the RREQs when they can't be sent
    
    // Third, buffer the rreq we're about to add. Any previous rreq
    // originating from ourselves should have been removed from the queue
    // already so we won't check that here; it's assumed. Also, we must
    // increment our sequence number before generating a RREQ (see RFC 3561
    // Page 11)
    data.seq_num++;
    rreq_fifo_push(data.rreq_queue, data.seq_num, data.ip_addr);
    
    struct aodv_pkt* pkt = aodv_pkt_alloc();
    uint8_t hop_count = (invalid_route) ?
        msh_route_get_hop_count(invalid_route) : 0;
    uint32_t dest_seq_num = (invalid_route) ?
        msh_route_get_dest_seq_num(invalid_route) : 0;
    
    // We always want bidirectional communications
    uint8_t flags = RREQ_GRATUITOUS_RREP_FLAG;
    flags |= (!invalid_route) ? RREQ_UNKNOWN_SEQ_NUM_FLAG : 0;
    
    aodv_pkt_set_ttl(pkt, expanding_ring_search_ttl(hop_count, prev_tries));

    // We are going to send a new RREQ, so we increment the global rreq id
    // counter
    data.rreq_id++;
    
    // After gathering all the needed information, we can build our rreq
    aodv_pkt_build_rreq(pkt, flags, hop_count, data.rreq_id, dest.s_addr,
            dest_seq_num, data.ip_addr.s_addr, data.seq_num);
    
    // Finally we send the packet
    aodv_pkt_send(pkt);
    
    aodv_pkt_destroy(pkt);
}

void aodv_process_rreq(struct aodv_pkt* pkt)
{
    // (See page 16 of RFC 3561)
    // First create/update a route to the previous hop without a valid sequence
    // number
    struct in_addr addr;
    addr.s_addr = aodv_pkt_get_address(pkt);
    
    struct msh_route *find_route = msh_route_alloc();
    msh_route_set_dst_ip(find_route, addr);
    struct msh_route *route = routing_table_find(data.routing_table,
        find_route, RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING);
    
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

    uint8_t hop_count = rreq->hop_count;
    rreq->hop_count = hop_count + 1;
    
    // searches for a reverse route to the Originator IP Address (see
    // section 6.2), using longest-prefix matching.  If need be, the route
    // is created, or updated using the Originator Sequence Number from the
    // RREQ in its routing table.
    struct in_addr addr_orig;
    addr_orig.s_addr = rreq->orig_ip_addr;
    msh_route_set_dst_ip(find_route, addr_orig);
    route = routing_table_find(data.routing_table,
        find_route, RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING);
    
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
        
        //TODO: check if this seq_num check is being done right
        if(seq_num_old < seq_num_new)
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
    if(!aodv_answer_to_rreq(pkt) && aodv_pkt_get_ttl(pkt) > 1)
    {
        aodv_pkt_set_address(pkt, INADDR_BROADCAST);
        aodv_pkt_decrease_ttl(pkt);
        rreq->hop_count++;
        
        msh_route_set_dst_ip(find_route, dest_addr);
        
        route = routing_table_find(data.routing_table,
            find_route, RTFIND_BY_DEST_LONGEST_PREFIX_MATCHING);
        
        // If we didn't send a rrep is because we either don't have a route
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
    
    // Free the mallocs! we don't need this temporal route anymore
    msh_route_destroy(find_route);
}

uint8_t aodv_answer_to_rreq(struct aodv_pkt* pkt_rreq)
{
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
