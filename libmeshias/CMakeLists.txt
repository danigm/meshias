set (LIBMESH libmeshias.c msh_route.c alarm/alarm.c alarm/rbtree.c
    utils.c aodv/configuration_parameters.c)

add_library (libmeshias ${LIBMESH})
