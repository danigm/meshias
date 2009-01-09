# The name of our project is "MESHIAS". CMakeLists files in this project can 
# refer to the root source directory of the project as ${MESHIAS_SOURCE_DIR} and 
# to the root binary directory of the project as ${MESHIAS_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (MESHIAS)
set(MESHIAS_SRCS nfqnl_test.c)

# Recurse into the "libnetfilter_queue" subdirectory. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 
add_subdirectory (libnetfilter_queue)

# Add executable called "daemon". The extensions are automatically found
add_executable (daemon ${MESHIAS_SRCS}) 
 
# Link the executable to the Hello library. 
target_link_libraries (daemon netfilter_queue nfnetlink)

install(TARGETS daemon RUNTIME DESTINATION bin)
