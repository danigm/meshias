#ifndef FDS_H
#define FDS_H

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * This struct makes socket usage easier.
 */
struct fds {
    /** The socket with the highest number. */
    int maxfd;
    /** The socket's set. */
    fd_set readfds;
};

/**
 * Create a socket's set.
 */
struct fds *create_fds(void);

/**
 * Destroy the socket's set.
 */
void destroy_fds(struct fds *);

/**
 * Add a socket to the set.
 */
int register_fd(int fd, struct fds *fds);

#endif
