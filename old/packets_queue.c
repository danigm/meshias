#include <linux/netfilter.h>        /* for NF_ACCEPT */
#include "libnetfilter_queue/libnetfilter_queue.h"

#include "packets_queue.h"

struct pq_element
{
    struct pq_element *next;
    u_int_32 route;
    int id;
};


void pq_init()
{
    (**first).next = NULL;
}

void pq_shutdown()
{
    struct pq_element *aux;
    struct pq_element *next;

    if(!first)
        return;

    for(aux = *first; aux != NULL; aux = next)
    {
        next = aux->next;
        free(aux);
    }

    *first = NULL;
}

void pq_add(u_int_32 route, int id)
{
    struct pq_element* new_packet = (*pq_element) malloc(sizeof(struct pq_element));
    new_packet->next = *first;
    new_packet->route = route;
    new_packet->id = id;
    *first = new_packet;
}

int pq_del(struct nfq_q_handle *qh, int id)
{
    free(pq_take_by_id(first, id));

    return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
}

struct packet* pq_take_by_id(int id)
{
    struct pq_element *packet = NULL;

    /* If the queue is empty */
    if(*first == NULL)
        return NULL;

    packet = *first;

    /* If it is the first */
    if(packet->id == id)
    {
        (*first) = packet->next;
        return packet;
    }

    for(; packet->next != NULL; packet = packet->next)
    {
        if(packet->next->id == id)
        {
            packet->next = packet->next->next;
            return aux;
        }
    }
    
    /* Not found */
    return NULL;
}

struct packet* pq_take_by_route(u_int_32 route)
{
    struct pq_element *packet = NULL;

    /* If the queue is empty */
    if(*first == NULL)
        return NULL;

    packet = *first;

    /* If it is the first */
    if(packet->route == route)
    {
        (*first) = packet->next;
        return packet;
    }

    for(; packet->next != NULL; packet = packet->next)
    {
        if(packet->next->route == route)
        {
            packet->next = packet->next->next;
            return aux;
        }
    }
    
    /* Not found */
    return NULL;
}

void pq_new_route(u_int_32 route)
{
    struct pq_element *packet=NULL;
    while(packet=pq_take_by_route(route))
    {
        nfq_set_verdict(DATA(queue),packet->id, NF_ACCEPT, 0, NULL);
    }
}
