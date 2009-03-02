#include "rreq_fifo.h"
#include "packets_fifo.h"
#include "aodv_logic.h"

void aodv_find_route(struct in_addr dest, struct msh_route *invalid_route,
    uint8_t prev_tries)
{
    //First, check that preev_tries hasn't exceeded its limit.
    if(prev_tries > RREQ_RETRIES())
    {
        // Route to the dest definitely not found: drop packets
        packets_fifo_drop_packets(data.packets_queue, dest);
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
    
    struct aodv_pkt* pkt = aodv_create_pkt();
    uint8_t hop_count = (invalid_route) ?
        msh_route_get_hop_count(invalid_route) : 0;
    uint32_t dest_seq_num = (invalid_route) ?
        msh_route_get_dest_seq_num(invalid_route) : 0;
    
    // We always want bidirectional communications
    uint8_t flags = RREQ_GRATUITOUS_RREP_FLAG;
    flags |= (!invalid_route) ? RREQ_UNKNOWN_SEQ_NUM_FLAG : 0;
    
    aodv_set_ttl(pkt, expanding_ring_search_ttl(hop_count, prev_tries));

    // We are going to send a new RREQ, so we increment the global rreq id
    // counter
    data.rreq_id++;
    
    // After gathering all the needed information, we can build our rreq
    aodv_build_rreq(pkt, flags, hop_count, data.rreq_id, dest.s_addr, dest_seq_num,
        data.ip_addr.s_addr, data.seq_num);
    
    //TODO: this function call shouldn't be needed!!
    uint32_t broadcast = 0xFFFFFFFF;
    aodv_set_address(pkt, broadcast);
    
    // Finally we send the packet
    aodv_send_packet(pkt);
}
