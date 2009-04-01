#include "msh_data.h"
#include "aodv_logic.h"
#include "nfqueue.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//TODO: Doesn't build
//#include <linux/netfilter_ipv4.h> // Iptables hooks
/* IP Hooks */
/* After promisc drops, checksum checks. */
#define NF_IP_PRE_ROUTING   0
/* If the packet is destined for this box. */
#define NF_IP_LOCAL_IN      1
/* If the packet is destined for another interface. */
#define NF_IP_FORWARD       2
/* Packets coming from a local process. */
#define NF_IP_LOCAL_OUT     3
/* Packets about to hit the wire. */
#define NF_IP_POST_ROUTING  4
#define NF_IP_NUMHOOKS      5


int nfqueue_init()
{
    // NF_QUEUE initializing
    data.handle = nfq_open();
    if (!data.handle)
    {
        perror("Error: during nfq_open()");
        return ERR_INIT;
    }

    if (nfq_unbind_pf(data.handle, AF_INET) < 0)
    {
        perror("Error: during nfq_unbind_pf()");
        return ERR_INIT;
    }

    if (nfq_bind_pf(data.handle, AF_INET) < 0)
    {
        perror("Error: during nfq_bind_pf()");
        return ERR_INIT;
    }

    data.queue = nfq_create_queue(data.handle, 0, &manage_packet, NULL);
    if (!data.queue)
    {
        perror("Error: during nfq_create_queue()");
        return ERR_INIT;
    }

    if (nfq_set_mode(data.queue, NFQNL_COPY_PACKET, 0xffff) < 0)
    {
        perror("Error: can't set packet_copy mode");
        return ERR_INIT;
    }

    data.netlink_handle = nfq_nfnlh(data.handle);
    data.nfqueue_fd = nfnl_fd(data.netlink_handle);
    // End of NF_QUEUE initializing

    // Adding nfqueue socket to the set
    register_fd(data.nfqueue_fd,data.fds);

    return 0;
}

void nfqueue_shutdown()
{
    // Free the mallocs
    if(data.queue != NULL)
    {
        nfq_destroy_queue(data.queue);
    }
    if(data.handle != NULL)
    {
        nfq_close(data.handle);
    }
}
 
void nfqueue_receive_packets()
{
    // Receive the data from the fd
    char buf[4096] __attribute__ ((aligned));
    int received;
    if( (received = recv(data.nfqueue_fd, buf, sizeof(buf), 0)) >= 0 )
    {
        // Call the handle
        nfq_handle_packet(data.handle, buf, received);
    }
}

static uint32_t nfqueue_packet_print(struct nfq_data *packet)
{
    int id = 0;
    int i;
    int length;
    struct nfqnl_msg_packet_hdr *packetHeader;
    struct nfqnl_msg_packet_hw *macAddress;
    struct timeval timeVal;
    uint32_t mark;
    uint32_t device; 
    char *pckt_data;
    struct nfq_iphdr* ip_header;
    struct nfq_tcphdr* tcp_header;
        
    packetHeader = nfq_get_msg_packet_hdr(packet);
    if (packetHeader)
    {
        id = ntohl(packetHeader->packet_id);
        printf("hw_protocol=0x%04x hook=%u id=%u\n",
            ntohs(packetHeader->hw_protocol), packetHeader->hook, id);
    }
        
    macAddress = nfq_get_packet_hw(packet);
    if (macAddress)
        printf("mac len %d address\n",ntohs(macAddress->hw_addrlen));
    else
        puts("no MAC adress");

    if (!nfq_get_timestamp(packet,&timeVal))
    {
        printf("timestamp %ld.%ld",timeVal.tv_sec,timeVal.tv_usec);
    }
    else
        puts("no timestamp");
    
    mark = nfq_get_nfmark(packet);
    if (mark)
        printf("mark=%u\n", mark);
    else
        puts("no mark");

    device = nfq_get_indev(packet);
    if (device)
        printf("indev=%u\n", device);
    else
        puts("no indev");

    device = nfq_get_outdev(packet);
    if (device)
        printf("outdev=%u\n", device);
    else
        puts("no outdev");

    length = nfq_get_payload(packet, &pckt_data);
    if (length >= 0)
    {
        printf("pckt_data[%d]:{", length);
        for(i=0;i<length;i++)
        {
            if(isprint(pckt_data[i]))
                fputc(pckt_data[i],stdout);
            else
                fputc(' ',stdout);
        }
    }

    puts("}");

    /* Test nfq_headers*/
    /* IP */
    ip_header = nfq_get_iphdr(packet);
    if(ip_header != NULL)
    {
        printf("source: %d\ndest: %d\n",nfq_get_ip_saddr(ip_header),nfq_get_ip_daddr(ip_header));
    }

    /* TCP */
    tcp_header=nfq_get_tcphdr(packet);
    if(tcp_header != NULL)
    {
        printf("source: %d\ndest: %d\n",nfq_get_tcp_source(tcp_header),nfq_get_tcp_dest(tcp_header));
    }

    return id;
}

static uint32_t nfqueue_packet_get_id(struct nfq_data *packet)
{
    uint32_t id = -1;
    struct nfqnl_msg_packet_hdr *packetHeader;

    if( (packetHeader = nfq_get_msg_packet_hdr(packet)) != NULL )
        id = ntohl(packetHeader->packet_id);
       
    return id;
}

static uint32_t nfqueue_packet_get_hook(struct nfq_data *packet)
{
    uint32_t hook = -1;
    struct nfqnl_msg_packet_hdr *packetHeader;

    if( (packetHeader = nfq_get_msg_packet_hdr(packet)) != NULL )
        hook = packetHeader->hook;
       
    return hook;
}

static struct in_addr nfqueue_packet_get_dest(struct nfq_data *packet)
{
    struct in_addr dest;
    struct nfq_iphdr* ip_header;
    
    if( (ip_header = nfq_get_iphdr(packet)) != NULL )
    {
        dest.s_addr = nfq_get_ip_daddr(ip_header);
    }
    return dest;
}

static struct in_addr nfqueue_packet_get_orig(struct nfq_data *packet)
{
    struct in_addr orig;
    struct nfq_iphdr* ip_header;
    
    if( (ip_header = nfq_get_iphdr(packet)) != NULL )
    {
        orig.s_addr = nfq_get_ip_saddr(ip_header);
    }
    return orig;
}

static int nfqueue_packet_is_aodv(struct nfq_data *packet)
{
    struct in_addr dest;
    struct nfq_iphdr* ip_header;
    struct nfq_udphdr* udp_header;
    
    if( (ip_header = nfq_get_iphdr(packet)) != NULL )
    {
        if(nfq_get_ip_protocol(ip_header)!=IPPROTO_UDP) // 17 is UDP
            return 0;
        
        // Now we know it's udp; is it AODV traffic?
        if( (udp_header = nfq_get_udphdr(packet)) == NULL)
            return 0;
        
        return nfq_get_udp_dest(udp_header) == AODV_UDP_PORT;
    }
    return 0;
}

static int manage_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data2)
{
    uint32_t hook=nfqueue_packet_get_hook(nfa);
    
    // AODV traffic is handled elsewhere already (by the daemon)
    uint32_t id=nfqueue_packet_get_id(nfa);
    if(nfqueue_packet_is_aodv(nfa))
    {
        printf("accepting AODV packet: id: %d\n", id);
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
        
    // packets coming from different hooks are handled in different ways
    switch(hook)
    {
        // Input table
        case NF_IP_LOCAL_IN:
            puts("capturing packet from INPUT iptables hook");
            return manage_input_packet(qh,nfmsg,nfa);
            break;
        // Forward table
        case NF_IP_FORWARD:
            puts("capturing packet from FORWARD iptables hook");
            return manage_forward_packet(qh,nfmsg,nfa);
            break;
        // Output table
        case NF_IP_LOCAL_OUT:
            puts("capturing packet from OUTPUT iptables hook");
            return manage_output_packet(qh,nfmsg,nfa);
            break;
        // Any other table: we shouldn't  be receiving packets from any other
        // hook. Accept the packet
        default:
            puts("error: capturing packet from an iptables hook we shouldn't");
            return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
            break;
    }
}

static int manage_output_packet(struct nfq_q_handle *qh,struct nfgenmsg
        *nfmsg, struct nfq_data *nfa)
{
    struct in_addr dest = { nfqueue_packet_get_dest(nfa).s_addr };
    
    uint32_t id = nfqueue_packet_get_id(nfa);
    // routing_table_use_route() will set invalid_route if it finds a route but
    // it's marked as invalid.
    struct msh_route *invalid_route = 0;
    
    printf ("packet for %s: ", inet_htoa(dest));
    
    // If there's a route for the packet, or it's a broadcast or it's an AODV
    // packet, let it go
    if(dest.s_addr == data.broadcast_addr.s_addr || nfqueue_packet_is_aodv(nfa))
    {
        puts("ACCEPT");
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
    struct in_addr orig = { nfqueue_packet_get_orig(nfa).s_addr };
    
    if(routing_table_use_route(data.routing_table, dest, &invalid_route, orig))
    {
        puts("ACCEPT");
        
        // Finally accept the packet
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
    else
    {
        puts("STOLEN: Route not found, finding route..");
        // Route not found. Queue the packet and find a route
        packets_fifo_push(data.packets_queue, id, dest);
        
        // Will only try to find a route if we are not already trying to find
        // one (i.e. when are not waiting response from a RREQ for that route
        // sent by us)
        if(!rreq_fifo_waiting_response_for(data.rreq_queue, dest))
            aodv_find_route(dest, invalid_route, 0);
        return nfq_set_verdict(qh, id, NF_STOLEN, 0, NULL);
    }
}

static int manage_input_packet(struct nfq_q_handle *qh,struct nfgenmsg
        *nfmsg, struct nfq_data *nfa)
{
    struct in_addr dest = { nfqueue_packet_get_dest(nfa).s_addr };
    
    uint32_t id = nfqueue_packet_get_id(nfa);
    // routing_table_use_route() will set invalid_route if it finds a route but
    // it's marked as invalid.
    struct msh_route *invalid_route = 0;
    
    printf ("packet for %s: ", inet_htoa(dest));
    
    // If there's a route for the packet, or it's a broadcast or it's an AODV
    // packet, let it go
    if(dest.s_addr == data.broadcast_addr.s_addr || nfqueue_packet_is_aodv(nfa))
    {
        puts("ACCEPT");
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
    struct in_addr orig = { nfqueue_packet_get_orig(nfa).s_addr };
    
    // Actually, we only need to process packets being forwarded in order to
    // be able to call routing_table_use_route() to update statistics
    if(routing_table_use_route(data.routing_table, dest, &invalid_route, orig))
    {
        puts("ACCEPT");
        
        // Finally accept the packet
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
    else
        puts("BUG: we are receiving a packet directed to us, routing_table_use_route() should by definition work");
}

static int manage_forward_packet(struct nfq_q_handle *qh,struct nfgenmsg
        *nfmsg, struct nfq_data *nfa)
{
    struct in_addr dest = { nfqueue_packet_get_dest(nfa).s_addr };
    
    uint32_t id = nfqueue_packet_get_id(nfa);
    // routing_table_use_route() will set invalid_route if it finds a route but
    // it's marked as invalid.
    struct msh_route *invalid_route = 0;
    
    printf ("packet for %s: ", inet_htoa(dest));
    
    // If there's a route for the packet, or it's a broadcast or it's an AODV
    // packet, let it go
    if(dest.s_addr == data.broadcast_addr.s_addr || nfqueue_packet_is_aodv(nfa))
    {
        puts("ACCEPT");
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
    struct in_addr orig = { nfqueue_packet_get_orig(nfa).s_addr };
    
    // Actually, we only need to process packets being forwarded in order to
    // be able to call routing_table_use_route() to update statistics
    if(routing_table_use_route(data.routing_table, dest, &invalid_route, orig))
    {
        puts("ACCEPT");
        
        // Finally accept the packet
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
    else
    {
        puts("BUG: we shoudln't be forwarding a packet if we haven't got a route for it (!)");
        
        //TODO: We should do something else (RERR..), but we happily accept the packet by now.
        puts("ACCEPT");
        
        // Finally accept the packet
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
}
