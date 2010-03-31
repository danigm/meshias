#include <stdlib.h>
#include "libmeshias.h"

int main()
{
    void **routes = mesh_get_routes("localhost");
    int i = 0;
    for (i=0; routes[i] != NULL; i++) {
        printf("x: %s\n", inet_htoa(((struct msh_route*)routes[i])->dst_ip));
        msh_route_destroy(routes[i]);
    }

    free(routes);

    //mesh_kill("localhost");
    //mesh_restart("localhost");
    return 0;
}
