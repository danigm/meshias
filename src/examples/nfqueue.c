#include <sys/socket.h>
#include <linux/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../libnetfilter_queue/libnetfilter_queue.h"
#include "../libnetfilter_queue/libnetfilter_queue_headers.c"
#include <linux/netfilter.h>        /* for NF_ACCEPT */

int oldid = -1;
static int manage_packet(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
                         struct nfq_data *nfa, void *data2)
{
    char *payload;
    struct nfqnl_msg_packet_hdr *packetHeader;
    char **puntero = (char**)(data2);

    if (oldid != -1) {
        nfq_set_verdict(qh, oldid, NF_ACCEPT, 0, NULL);
    }

    if ((packetHeader = nfq_get_msg_packet_hdr(nfa)) != NULL) {
        oldid = ntohl(packetHeader->packet_id);
    }

    nfq_get_payload(nfa, &payload);

    (*puntero) = payload + sizeof(struct nfq_iphdr) + sizeof(struct nfq_udphdr);
    payload = (*puntero);

    printf("manage_packet %p\n", (*puntero));

    return 0;
}

int main(int argc, char *argv[])
{
    struct nfq_handle *handle;
    struct nfq_q_handle *queue;
    struct nfnl_handle *netlink_handle;
    int nfqueue_fd;
    char *puntero = NULL;
    printf("init %p\n", puntero);

    // NF_QUEUE initializing
    handle = nfq_open();

    if (!handle) {
        perror("Error: during nfq_open()");
        goto end;
    }

    if (nfq_unbind_pf(handle, AF_INET) < 0) {
        perror("Error: during nfq_unbind_pf()");
        goto end;
    }

    if (nfq_bind_pf(handle, AF_INET) < 0) {
        perror("Error: during nfq_bind_pf()");
        goto end;
    }

    queue = nfq_create_queue(handle, 0, &manage_packet, &puntero);

    if (!queue) {
        perror("Error: during nfq_create_queue()");
        goto end;
    }

    if (nfq_set_mode(queue, NFQNL_COPY_PACKET, 0xffff) < 0) {
        perror("Error: can't set packet_copy mode");
        goto end;
    }

    netlink_handle = nfq_nfnlh(handle);
    nfqueue_fd = nfnl_fd(netlink_handle);
    // End of NF_QUEUE initializing

    char buf[4096] __attribute__((aligned));
    int received;

    while (received = recv(nfqueue_fd, buf, sizeof(buf), 0)) {
        if (puntero) {
            printf("antes: %p\n", puntero);
            printf("pone: %s\n", puntero);
        }

        // Call the handle
        nfq_handle_packet(handle, buf, received);
        printf("dps: %p\n", puntero);
        printf("pone dps: %s\n", puntero);

    }

end:

    // Free the mallocs
    if (queue != NULL) {
        nfq_destroy_queue(queue);
    }

    if (handle != NULL) {
        nfq_close(handle);
    }

    return 0;
}
