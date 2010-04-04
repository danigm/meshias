include_directories (.)

set (MESHIAS_SRCS meshias.c msh_data.c
    nfqueue.c sockets/aodv_socket.c log.c routing_table.c rreq_fifo.c
    packets_fifo.c sockets/fds.c statistics.c
    communication_interface.c aodv/packet.c aodv/logic.c)

# Add executable called "meshias". The extensions are automatically found
add_executable (meshias ${MESHIAS_SRCS})

# Link the executable to the needed library. 
target_link_libraries (meshias netfilter_queue nfnetlink nl libmeshias)

install(TARGETS meshias RUNTIME DESTINATION bin)
#install(TARGETS meshias-tools RUNTIME DESTINATION bin)
