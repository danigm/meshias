#include "routes_cache.h"

struct rc_entry
{
    u_int16_t distance;
    u_int32_t next_hop;
    u_int32_t dest;
    struct rc_entry* next;
}

void rc_init(struct rc_entry **first)
{
    **first.next = NULL;
}

void rc_shutdown(struct rc_entry **first)
{
    struct rc_entry *aux;
    struct rc_entry *next;

    if(!first)
        return;

    for(aux = *first; aux != NULL; aux = next)
    {
        next = aux->next;
        free(aux);
    }

    *first = NULL;
}

void rc_add(struct rc_entry **first, u_int16_t distance, u_int32_t next_hop,
    u_int32_t dest)
{
    struct rc_entry* new_entry = (*rc_entry) malloc(sizeof(struct rc_entry));

    new_entry->dest = dest;
    new_entry->next_hop = next_hop;
    new_entry->distance = distance;
    new_entry->next = *first;
    *first = new_entry;
}

void rc_del(struct rc_entry **first, /*TODO*/)
{
   
}

u_int16_t rc_get_distance(struct rc_entry *entry)
{
    return (entry) ? entry->distance : 0;
}

u_int32_t rc_get_next_hop(struct rc_entry *entry)
{
    return (entry) ? entry->next_hop : 0;
}

u_int32_t rc_get_dest(struct rc_entry *entry)
{
    return (entry) ? entry->dest : 0;
}

void rc_set_distance(struct rc_entry *entry, u_int16_t distance)
{
    if(!entry)
        return;
    
    entry->distance = distance;
}

void rc_set_next_hop(struct rc_entry *entry, u_int32_t next_hop)
{   
    if(!entry)
        return;

    entry->next_hop = next_hop;
}

void rc_set_dest(struct rc_entry *entry, u_int32_t dest)
{
    if(!entry)
        return;

    entry->dest = dest;
}

int rc_find_by_(struct rc_entry **first, struct rc_entry *entry)
{
    
}
