# The name of our project is "MESHIAS". CMakeLists files in this project can 
# refer to the root source directory of the project as ${MESHIAS_SOURCE_DIR} and 
# to the root binary directory of the project as ${MESHIAS_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (MESHIAS)

set (CMAKE_C_FLAGS "-g -Wall")

include_directories (.)

set (MESHIAS_SRCS meshias.c msh_data.c
    nfqueue.c sockets/aodv_socket.c log.c routing_table.c rreq_fifo.c
    packets_fifo.c sockets/fds.c statistics.c
    communication_interface.c aodv/packet.c aodv/logic.c)

# set  (MESHIAS_TOOLS_SRCS meshias-tools.c local.c utils.c)
set (LIBMESH libmeshias.c msh_route.c alarm/alarm.c alarm/rbtree.c
    utils.c aodv/configuration_parameters.c)
set (LMESHTEST lmeshtest.c)

# Recurse into the "libnetfilter_queue" subdirectory. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 
add_subdirectory (libnetfilter_queue)

# Add executable called "meshias". The extensions are automatically found
add_library (libmeshias ${LIBMESH})
add_executable (meshias ${MESHIAS_SRCS})
#add_executable (meshias-tools ${MESHIAS_TOOLS_SRCS})
add_executable (lmeshtest ${LMESHTEST} ${LIBMESH})

# Link the executable to the needed library. 
target_link_libraries (meshias netfilter_queue nfnetlink nl libmeshias)
#target_link_libraries (meshias-tools nl)
target_link_libraries (lmeshtest netfilter_queue nfnetlink nl libmeshias)

install(TARGETS meshias RUNTIME DESTINATION bin)
#install(TARGETS meshias-tools RUNTIME DESTINATION bin)
