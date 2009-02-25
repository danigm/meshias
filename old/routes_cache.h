#ifndef ROUTES_CACHE_H
#define ROUTES_CACHE_H

struct rc_entry;

/**
 * Gets routes from kernel and adds them to the list
 */
void rc_init(struct rc_entry **first);

/**
 * Frees memory
 */
void rc_shutdown(struct rc_entry **first);


//TODO: rc_add debería tomar un rc_entry, y debería existir un pq_create para
//tener el punto de variación en un sitio bien localizado y hacer más flexible
//la API (por si quieres por ejemplo crear un paquete, luego pasarselo a una 
//función para que lo modifique y luego entonces añadirlo y cosas asi.
/**
 * Adds an entry to the route cache list
 */
void rc_add(struct rc_entry **first, u_int16_t distance, u_int32_t next_hop,
    u_int32_t dest);

/**
 * Removes and frees an entry from the route cache list
 */
void rc_del(struct rc_entry **first, /*TODO*/);

/**
 * Returns the number of hops from the host to the destination
 */
u_int16_t rc_get_distance(struct rc_entry *entry);

/**
 * Returns the next hop
 */
u_int32_t rc_get_next_hop(struct rc_entry *entry);

/**
 * Returns  the final destination
 */
u_int32_t rc_get_dest(struct rc_entry *entry);

/**
 * Change the number of hops from host to the destination
 */
void rc_set_distance(struct rc_entry *entry, u_int16_t distance);

/**
 * Changes the next hop
 */
void rc_set_next_hop(struct rc_entry *entry, u_int32_t next_hop);

/**
 * Changes the final destination
 */
void rc_set_dest(struct rc_entry *entry, u_int32_t dest);

//TODO: Redefinir bien esto.. ¿cuantos rc_find necesitamos? ¿qué argumentos se 
// les pasa?
/**
 * 
 * @return 1 if the route was found, otherwise returns 0.
 */
int rc_find_by_dest(struct rc_entry **first, u_int32_t dest);

#endif
