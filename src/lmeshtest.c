#include <stdlib.h>
#include "statistics.h"
#include <arpa/inet.h>
#include "libmeshias.h"

int main()
{
    void **routes = mesh_get_routes("localhost");
    struct msh_route *route;
    int i = 0;
    for (i=0; routes[i] != NULL; i++) {
        route = (struct msh_route*)routes[i];
        printf("%s\n", inet_ntoa(route->dst_ip));
        msh_route_destroy(route);
    }

    free(routes);

    struct statistics_t *stats = mesh_get_stats("localhost");
    printf("%d\n", stats->packets_dropped);
    free(stats);

    return 0;
}
