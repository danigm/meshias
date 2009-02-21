#include "msh_data.h"
#include "nfqueue.h"

int nfqueue_init()
{
    /* NF_QUEUE initializing */
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
    /* End of NF_QUEUE initializing */

    /* Adding all sockets to the set */
    //TODO: this probably doesn't work right now ¿?
    //FD_SET(data.nfqueue_fd,&all_fd);
    
    debug(3,"Nf_queue initialized sucessfully");
    return 0;
}
 
//TODO: remplace printf with log function
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

// TODO: Return a more menaningful type
uint32_t nfqueue_packet_get_dest(struct nfq_data *packet)
{
    uint32_t dest = 0;
    struct nfq_iphdr* ip_header;
    
    if( (ip_header = nfq_get_iphdr(packet)) != NULL )
    {
        dest=nfq_get_ip_daddr(ip_header);
    }
    return dest;
}

// TODO: function is not yet written. See header for details
static int manage_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data)
{
    uint32_t dest = nfqueue_packet_get_dest(nfa);
    uint32_t id = nfqueue_packet_get_id(nfa);
    
    /* checking if the route exists */
    //if(route_exists(route))
    {
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
//     else
//     {
//         /* if not we put the packet in the waiting queue
//          *  and generate an aodv packet to find the new route
//          */
//         //pq_add(route,nfqueue_get_id(nfa));
//         //aodv_create_route(dest);
//         return nfq_set_verdict(qh, id, NF_STOLEN, 0, NULL);
//     }
}
