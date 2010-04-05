#ifndef __LIB_MESHIAS__
#define __LIB_MESHIAS__

typedef enum
{
    UNIX_SOCKET,
    TCP_SOCKET
} meshias_connection_t;

typedef struct
{
    meshias_connection_t type;
    int socket;
} meshias_connection;

typedef struct
{
    meshias_connection_t type;
    int socket;
    uint32_t ip;
    uint16_t port;
} meshias_connection_tcp;

typedef struct
{
    meshias_connection_t type;
    int socket;
    char* path;
} meshias_connection_unix;

/** Crea conexion */
meshias_connection* meshias_connection_alloc(meshias_connection_t type, char* path);

/** Libera la memoria de la conexion */
void meshias_connection_free(meshias_connection* con);

/** Conecta con el servidor */
int meshias_connect(meshias_connection* con, char* direccion);

/** Desconecta del servidor */
int meshias_close(meshias_connection* con);

/** Recibe estadisticas */
struct meshias_statistics* meshias_get_statistics(meshias_connection* con);

/** Resetea estadisticas */
int meshias_reset_statistics(meshias_connection* con);

//int meshias_get_statistics(meshias_connection* con, uint32_t miliseconds, callback);

/** Inicia aodv */
int meshias_start(meshias_connection* con);

/** Para el aodv */
int meshias_stop(meshias_connection* con);

/** Mata al demonio completo */
int meshias_kill(meshias_connection* con);

/** Obtiene la lista de rutas */
int meshias_get_route_list(meshias_connection* con);

/** Anade una ruta */
int meshias_add_route(meshias_connection* con, struct meshias_route);

/** Elimina una ruta */
int meshias_del_route(meshias_connection* con, struct meshias_route);

/** Obtiene las opciones aodv */
struct meshias_aodv_options* meshias_get_aodv_options(meshias_connection* con);

/** Modifica las opciones aodv */
int meshias_set_aodv_options(meshias_connection* con, struct meshias_aodv_options);

/** Obtiene la interfaz en la que esta corriendo el demonio aodv */
int meshias_get_interface(meshias_connection* con);

#endif
