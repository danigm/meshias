# The name of our project is "MESHIAS". CMakeLists files in this project can 
# refer to the root source directory of the project as ${MESHIAS_SOURCE_DIR} and 
# to the root binary directory of the project as ${MESHIAS_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (MESHIAS)

set (CMAKE_C_FLAGS "-g -Wall")

include_directories (libmeshias libnetfilter_queue)

add_subdirectory (libnetfilter_queue)
add_subdirectory (libmeshias)
add_subdirectory (meshias_daemon)
add_subdirectory (clients)
