#include "communication_interface.h"
#include "meshias-tools.h"
#include "statistics.h"
#include "route_obj.h"

#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
char *COMMANDS[] = {
    MSG_SHOW_ROUTES,
    MSG_SHOW_STATISTICS,
    MSG_CLEAN_STATISTICS,
};
*/

/**
 * Sends a command to the meshias daemon using a tcp socket.
 * Returns a char* that you must free, if fails, returns NULL.
 */
char *send_meshias_command(char* command, char *host)
{
    int numbytes;
    int i;
    int bufsize = 1024;
    int received_size = 1024;
    char *received;
    char buf[bufsize];
    int fd;
    struct sockaddr_in addr;

    struct hostent *he;
    if ((he = gethostbyname(host)) == NULL) {
        return NULL;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(MESH_PORT);
    addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(addr.sin_zero), 8);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return NULL;
    }

    if (connect(fd, (struct sockaddr *)&addr,
                sizeof(struct sockaddr)) == -1) {
        close (fd);
        return NULL;
    }

    snprintf (buf, bufsize, command);

    if (send(fd, buf, strlen(buf), 0) < 0) {
        close (fd);
        return NULL;
    }

    received = malloc(received_size*sizeof(char));
    memset(received, 0, received_size);
    i = 0;
    while ((numbytes = recv(fd, buf, bufsize, 0)) > 0) {
        if (i >= received_size) {
            received_size *= 2;
            received = realloc(received, received_size*sizeof(char));
            memset(received + i, 0, received_size - i);
        }
        snprintf(received + i, received_size - i, buf);
        i += numbytes;
    }

    close (fd);

    return received;
}

int mesh_kill(char *host)
{
    char *response = send_meshias_command(MSG_KILL, host);
    if (!response) {
        return -1;
    }

    free(response);
    return 0;
}

int mesh_restart(char *host)
{
    char *response = send_meshias_command(MSG_RESTART, host);
    if (!response) {
        return -1;
    }

    free(response);
    return 0;
}

/**
 * The returned struct must be freed
 * It's a NULL terminated array
 */
void **mesh_get_routes(char *host)
{
    int i, j;
    int routes_size = 10;
    void **routes = NULL;

    char *response = send_meshias_command(MSG_SHOW_ROUTES, host);
    if (!response) {
        return NULL;
    }

    routes = malloc(routes_size*sizeof(struct msh_route*));
    for (i=0; i<routes_size; i++) routes[i] = NULL;

    struct msh_route *route;
    struct in_addr dst_ip;
    struct in_addr next_hop;
    char *part;
    j = 0;
    part = strtok(response, "\n");
    while (part) {
        if (j >= routes_size - 1) {
            routes_size *= 2;
            routes = realloc(routes, routes_size);
            for (i=j; i<routes_size; i++) routes[i] = NULL;
        }

        route = msh_route_alloc();
        sscanf(part, "%d#%d#%d#%d#%d#%d#%d",
             &(dst_ip.s_addr),
             &(route->prefix_sz),
             &(route->dest_seq_num),
             &(route->flags),
             &(route->hop_count),
             &(next_hop.s_addr),
             &(route->net_iface));
        msh_route_set_dst_ip(route, dst_ip);
        msh_route_set_next_hop(route, next_hop);
        routes[j] = route;

        part = strtok(NULL, "\n");
        j++;
    }

    free(response);
    return routes;
}
