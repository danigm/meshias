#include <stdlib.h>
#include <arpa/inet.h>
#include "libmeshias.h"

int main()
{
    void **routes = mesh_get_routes("localhost");
    int i = 0;
    for (i=0; routes[i] != NULL; i++) {
        printf("%s\n", inet_ntoa(((struct msh_route*)routes[i])->dst_ip));
        msh_route_destroy(routes[i]);
    }

    free(routes);

    //mesh_kill("localhost");
    //mesh_restart("localhost");
    return 0;
}
