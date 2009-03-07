#include "msh_data.h"
#include "aodv_logic.h"
#include "nfqueue.h"
#include <sys/socket.h>
#include <arpa/inet.h>

int nfqueue_init()
{
    // NF_QUEUE initializing
    debug(3,"Nf_queue: opening library handle");
    data.handle = nfq_open();
    if (!data.handle)
    {
        debug(1,"FATAL ERROR: during nfq_open()");
        return ERR_INIT;
    }

    debug(3,"unbinding existing nf_queue handler for AF_INET (if any)");
    if (nfq_unbind_pf(data.handle, AF_INET) < 0)
    {
        debug(1,"FATAL ERROR: during nfq_unbind_pf()");
        return ERR_INIT;
    }

    debug(3,"binding nfnetlink_queue as nf_queue handler for AF_INET");
    if (nfq_bind_pf(data.handle, AF_INET) < 0)
    {
        debug(1,"FATAL ERROR: during nfq_bind_pf()");
        return ERR_INIT;
    }

    debug(3,"binding this socket to queue '0'");
    data.queue = nfq_create_queue(data.handle, 0, &manage_packet, NULL);
    if (!data.queue)
    {
        debug(1,"FATAL ERROR: during nfq_create_queue()");
        return ERR_INIT;
    }

    debug(3,"setting copy_packet mode");
    if (nfq_set_mode(data.queue, NFQNL_COPY_PACKET, 0xffff) < 0)
    {
        debug(1,"FATAL_ERROR: can't set packet_copy mode\n");
        return ERR_INIT;
    }

    data.netlink_handle = nfq_nfnlh(data.handle);
    data.nfqueue_fd = nfnl_fd(data.netlink_handle);
    // End of NF_QUEUE initializing

    // Adding nfqueue socket to the set
    FD_SET(data.nfqueue_fd,&data.all_fd);

    // max_fd
    if(data.nfqueue_fd>=data.max_fd)
      data.max_fd=data.nfqueue_fd+1;
    
    debug(3,"Nf_queue initialized sucessfully");
    return 0;
}
 
void nfqueue_receive_packets()
{
    char buf[4096] __attribute__ ((aligned));
    int received;
    while ( (received = recv(data.nfqueue_fd, buf, sizeof(buf), 0)) >= 0 )
    {
        debug(3,"Netfilter_queue packet received");
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

uint32_t nfqueue_packet_get_id(struct nfq_data *packet)
{
    int id = -1;
    struct nfqnl_msg_packet_hdr *packetHeader;

    if( (packetHeader = nfq_get_msg_packet_hdr(packet)) != NULL )
        id = ntohl(packetHeader->packet_id);
       
    return id;
}

struct in_addr nfqueue_packet_get_dest(struct nfq_data *packet)
{
    struct in_addr dest;
    struct nfq_iphdr* ip_header;
    
    if( (ip_header = nfq_get_iphdr(packet)) != NULL )
    {
        dest.s_addr = ntohl(nfq_get_ip_daddr(ip_header));
    }
    return dest;
}

static int manage_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data2)
{
    struct in_addr dest;
    dest.s_addr=nfqueue_packet_get_dest(nfa).s_addr;
    uint32_t id = nfqueue_packet_get_id(nfa);
    // routing_table_use_route() will set invalid_route if it finds a route but
    // it's marked as invalid.
    struct msh_route *invalid_route = 0;
    
    printf ("packet for %s: ", inet_ntoa(dest));
    
    // If there's a route for the packet, let it go
    if(routing_table_use_route(data.routing_table, dest, &invalid_route))
    {
        puts("ACCEPT");
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
    else
    {
        puts("STOLEN");
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
