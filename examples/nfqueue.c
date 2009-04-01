#include "../src/libnetfilter_queue/libnetfilter_queue.h"
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>        /* for NF_ACCEPT */
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

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
#define ERR_INIT            -1
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

static int manage_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data2)
{
    uint32_t hook = nfqueue_packet_get_hook(nfa);
    
    // AODV traffic is handled elsewhere already (by the daemon)
    uint32_t id = nfqueue_packet_get_id(nfa);
        
    // packets coming from different hooks are handled in different ways
    switch(hook)
    {
        // Input table
        case NF_IP_LOCAL_IN:
            puts("capturing packet from INPUT iptables hook");
            return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
            break;
            
        case NF_IP_FORWARD:
            puts("capturing packet from FORWARD iptables hook");
            return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
            break;
            
        case NF_IP_LOCAL_OUT:
            puts("capturing packet from OUTPUT iptables hook");
            return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
            break;
            
        default:
            puts("error: capturing packet from an iptables hook we shouldn't");
            return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
            break;
    }
}

int main(int argc, char **argv)
{
    struct nfq_handle *handle;
    struct nfq_q_handle *queue;
    struct nfnl_handle *netlink_handle;
    uint32_t nfqueue_fd;
    
    handle = nfq_open();
    if (!handle)
    {
        perror("Error: during nfq_open()");
        return ERR_INIT;
    }

    if (nfq_unbind_pf(handle, AF_INET) < 0)
    {
        perror("Error: during nfq_unbind_pf()");
        return ERR_INIT;
    }

    if (nfq_bind_pf(handle, AF_INET) < 0)
    {
        perror("Error: during nfq_bind_pf()");
        return ERR_INIT;
    }

    queue = nfq_create_queue(handle, 0, &manage_packet, NULL);
    if (!queue)
    {
        perror("Error: during nfq_create_queue()");
        return ERR_INIT;
    }

    if (nfq_set_mode(queue, NFQNL_COPY_PACKET, 0xffff) < 0)
    {
        perror("Error: can't set packet_copy mode");
        return ERR_INIT;
    }

    netlink_handle = nfq_nfnlh(handle);
    nfqueue_fd = nfnl_fd(netlink_handle);
    // End of NF_QUEUE initializing
    
    char buf[4096] __attribute__ ((aligned));
    int received;
    
    while(1)
    {
        if( (received = recv(nfqueue_fd, buf, sizeof(buf), 0)) >= 0 )
        {
            // Call the handle
            nfq_handle_packet(handle, buf, received);
        }
    }
    // Here be dragons
    
    // Free the mallocs
    if(queue != NULL)
    {
        nfq_destroy_queue(queue);
    }
    if(handle != NULL)
    {
        nfq_close(handle);
    }
    
    return 0;
}
