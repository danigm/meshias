#ifndef _FDS_H_
#define _FDS_H_

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

struct fds
{
    int maxfd;
    fd_set readfds;
};

struct fds *create_fds(void);
void destroy_fds(struct fds *);
int register_fd(int fd, struct fds *fds);

#endif
