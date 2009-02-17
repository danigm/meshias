# The name of our project is "MESHIAS". CMakeLists files in this project can 
# refer to the root source directory of the project as ${MESHIAS_SOURCE_DIR} and 
# to the root binary directory of the project as ${MESHIAS_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (MESHIAS)
set(MESHIAS_SRCS main.c msh_data.c alarm/alarm.c alarm/rbtree.c common.c nfqueue.c)

# Recurse into the "libnetfilter_queue" subdirectory. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 
add_subdirectory (libnetfilter_queue)

# Add executable called "meshias". The extensions are automatically found
add_executable (meshias ${MESHIAS_SRCS}) 

# Link the executable to the needed library. 
target_link_libraries (meshias netfilter_queue nfnetlink nl)

install(TARGETS meshias RUNTIME DESTINATION bin)
