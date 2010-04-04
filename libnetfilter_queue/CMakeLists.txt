cmake_minimum_required (VERSION 2.6)
project (LIBNETFILTER_QUEUE)
set(LIBNETFILTER_QUEUE_SRCS libnetfilter_queue.c libnetfilter_queue_headers.c)

# Create a library called "libnetfilter_queue" which includes the needed source files. 
# The extension is already found. Any number of sources could be listed here. 
add_library (netfilter_queue ${LIBNETFILTER_QUEUE_SRCS})

install(TARGETS netfilter_queue
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
install(FILES libnetfilter_queue.h libnetfilter_queue_headers.h DESTINATION include)
