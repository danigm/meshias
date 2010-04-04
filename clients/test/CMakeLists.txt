set (LMESHTEST lmeshtest.c)

add_executable (lmeshtest ${LMESHTEST} ${LIBMESH})

target_link_libraries (lmeshtest netfilter_queue nfnetlink nl libmeshias)
