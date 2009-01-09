#ifndef WAIT_QUEUE_H
#define WAIT_QUEUE_H

struct pq_element;
struct nfq_q_handle;


/**
 *  Initializes a packet_queue.
 */
void pq_init()

/**
 *  Erase all the elements from a given list.
 */
void pq_shutdown();

//TODO: pq_add deberia tomar un pq_element, y deberia existir un pq_create para
//tener el punto de variacion en un sitio bien localizado y hacer mas flexible
//la API (por si quieres por ejemplo crear un paquete, luego pasarselo a una
//funcion para que lo modifique y luego entonces anyadirlo y cosas asi.
/**
 * Creates in memory a package given a route and an id, and adds it to the given
 * list.
 */
void pq_add(u_int_32 route, int id);

/**
 * When there is no route for the destination ip of a packet, this function
 * drops the packet, and removes and delete it from the given queue.
 * 
 * @arg qh  The nfq from which we will DROP the packet
 * @arg id  The identifier of the packet to be dropped
 *
 * @return the result of the call to @ref nfq_set_verdict()
 */
int pq_del(struct nfq_q_handle *qh, int id);

//TODO: Hablar con alex sobre cómo realmente queremos implementar esta funcion
/**
 * Finds a packet by its id in the given queue, removes it from the queue and
 * returns it. If it's not found it returns NULL.
 */
struct packet* pq_take_by_id(int id);

/**
 * Does the same as @ref pq_take_by_id() but searching by route.
 */
struct packet* pq_take_by_route(u_int_32 route);

void pq_new_route(u_int_32 route);

#endif
