#ifndef __LIB_MESHIAS__
#define __LIB_MESHIAS__

#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "route_obj.h"

#define N_ELEMENTS(arr) (sizeof (arr) / sizeof ((arr)[0]))

int mesh_kill(char *host);
int mesh_restart(char *host);
void **mesh_get_routes(char *host);
struct statistics_t *mesh_get_stats(char *host);

#endif
