# The name of our project is "MESHIAS". CMakeLists files in this project can 
# refer to the root source directory of the project as ${MESHIAS_SOURCE_DIR} and 
# to the root binary directory of the project as ${MESHIAS_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (MESHIAS_EXAMPLES)

set (LIBNL_SRCS libnl-routes.c)
set (ALARM_SRCS ../src/alarm/alarm.c ../src/alarm/rbtree.c alarm.c)
set (RECVMSG_SRCS recvmsg.c)
set (SENDMSG_SRCS sendmsg.c)
set (LINUXLISTS_SRCS linuxlists.c)

add_executable (libnl ${LIBNL_SRCS})
add_executable (alarm ${ALARM_SRCS})
add_executable (recvmsg ${RECVMSG_SRCS})
add_executable (sendmsg ${SENDMSG_SRCS})
add_executable (linuxlists ${LINUXLISTS_SRCS})

# Link the executable to the needed library. 
target_link_libraries (libnl nl)

install(TARGETS libnl alarm recvmsg sendmsg linuxlists RUNTIME DESTINATION bin)
