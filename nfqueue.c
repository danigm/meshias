#include "nfqueue.h"

bool nfqueue_init()
{
    /* NF_QUEUE initializing */
    printf("opening library handle\n");
    DATA(handle) = nfq_open();
    if (!DATA(handle))
    {
        fprintf(stderr, "error during nfq_open()\n");
        return ERROR;
    }

    printf("unbinding existing nf_queue handler for AF_INET (if any)\n");
    if (nfq_unbind_pf(DATA(handle), AF_INET) < 0)
    {
        fprintf(stderr, "error during nfq_unbind_pf()\n");
        return ERROR;
    }

    printf("binding nfnetlink_queue as nf_queue handler for AF_INET\n");
    if (nfq_bind_pf(DATA(handle), AF_INET) < 0)
    {
        fprintf(stderr, "error during nfq_bind_pf()\n");
        return ERROR;
    }

    printf("binding this socket to queue '0'\n");
    DATA(queue) = nfq_create_queue(DATA(handle),0, &manage_pkt, NULL);
    if (!DATA(queue))
    {
        fprintf(stderr, "error during nfq_create_queue()\n");
        return ERROR;
    }

    printf("setting copy_packet mode\n");
    if (nfq_set_mode(DATA(queue), NFQNL_COPY_PACKET, 0xffff) < 0)
    {
        fprintf(stderr, "can't set packet_copy mode\n");
        return ERROR;
    }

    DATA(netlink_handle) = nfq_nfnlh(DATA(handle));
    DATA(nfqueue_fd) = nfnl_fd(DATA(netlink_handle));
    /* End of NF_QUEUE initializing */

    /* Adding all sockets to the set */
    FD_SET(DATA(nfqueue_fd),&all_fd);
}
 
void nfqueue_receive()
{
    while ((received = recv(DATA(nfqueue_fd), buf, sizeof(buf), 0))
            && received >= 0)
    {
        printf("pkt received\n");
        nfq_handle_packet(DATA(handle), buf, received);
    }
}

/* returns packet id */
static u_int32_t print_pkt(struct nfq_data *packet)
{
    int id = 0;
    int i;
    int length;
    struct nfqnl_msg_packet_hdr *packetHeader;
    struct nfqnl_msg_packet_hw *macAddress;
    struct timeval timeVal;
    u_int32_t mark;
    u_int32_t device; 
    char *data;
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

    length = nfq_get_payload(packet, &data);
    if (length >= 0)
    {
        printf("data[%d]:{", length);
        for(i=0;i<length;i++)
        {
            if(isprint(data[i]))
                fputc(data[i],stdout);
            else
                fputc(' ',stdout);
        }
    }

    puts("}");

    /* Test nfq_headers*/
    /* IP */
    ip_header=nfq_get_iphdr(packet);
    if(ip_header!=NULL)
    {
        printf("source: %d\ndest: %d\n",nfq_get_ip_saddr(ip_header),nfq_get_ip_daddr(ip_header));
    }

    /* TCP */
    tcp_header=nfq_get_tcphdr(packet);
    if(tcp_header!=NULL)
    {
        printf("source: %d\ndest: %d\n",nfq_get_tcp_source(tcp_header),nfq_get_tcp_dest(tcp_header));
    }

    return id;
}

u_int32_t nfqueue_get_id(struct nfq_data *packet)
{
    int id = -1;
    struct nfqnl_msg_packet_hdr *packetHeader;

    if((packetHeader = nfq_get_msg_packet_hdr(packet))!=NULL)
        id = ntohl(packetHeader->packet_id);
       
    return id;
}

u_int32_t nfqueue_get_dest(struct nfq_data *packet)
{
    u_int_32 dest=0;
    struct nfq_iphdr* ip_header;
    
    if((ip_header=nfq_get_iphdr(packet))!=NULL)
    {
        dest=nfq_get_ip_daddr(ip_header);
    }
    return dest;
}

static int manage_pkt(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data)
{
    u_int32_t route = nfqueue_get_dest(nfa);
    /* print the package
    print_pkg(nfa);
    printf("=====================================================\n");
    */
    /* checking if the route exists */
    if(route_exists(route))
    {
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
    else
    {
        /* if not we put the packet in the waiting queue
         *  and generate a aodv packet to know the new route
         */
        pq_add(route,nfqueue_get_id(nfa));
        aodv_create_route(route);
        return nfq_set_verdict(qh, id, NF_STOLEN, 0, NULL);
    }
}
