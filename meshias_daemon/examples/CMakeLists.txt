# The name of our project is "MESHIAS". CMakeLists files in this project can
# refer to the root source directory of the project as ${MESHIAS_SOURCE_DIR} and
# to the root binary directory of the project as ${MESHIAS_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (MESHIAS_EXAMPLES)

set (LIBNL_SRCS libnl-routes.c)
set (ALARM_SRCS ./alarm.c ../alarm/alarm.c ../alarm/rbtree.c)
set (RECVMSG_SRCS recvmsg.c)
set (SENDMSG_SRCS sendmsg.c)
set (LINUXLISTS_SRCS linuxlists.c)
set (GETIP_SRCS getip.c)
set (NFQUEUE_SRCS nfqueue.c)
set (BROADCASTER_SRCS broadcaster.c)
set (SELECT_SRCS select.c)
set (DEBUG_SRCS debug.c ../log.c)

add_executable (libnl ${LIBNL_SRCS})
add_executable (debug ${DEBUG_SRCS})
add_executable (alarm ${ALARM_SRCS})
add_executable (recvmsg ${RECVMSG_SRCS})
add_executable (sendmsg ${SENDMSG_SRCS})
add_executable (linuxlists ${LINUXLISTS_SRCS})
add_executable (getip ${GETIP_SRCS})
add_executable (nfqueue ${NFQUEUE_SRCS})
add_executable (broadcaster ${BROADCASTER_SRCS})
add_executable (select ${SELECT_SRCS})

# Link the executable to the needed library.
target_link_libraries (libnl nl)
target_link_libraries (nfqueue netfilter_queue nfnetlink)

install(TARGETS libnl alarm recvmsg sendmsg linuxlists getip nfqueue broadcaster select RUNTIME DESTINATION bin)
