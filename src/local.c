/*
 * (C) 2006-2007 by Pablo Neira Ayuso <pablo@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Description: UNIX sockets library
 */

#include "local.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>

int local_server_create(struct local_server *server, struct local_conf *conf)
{
    int fd;
    socklen_t len;
    struct sockaddr_un local;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        return -1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &conf->reuseaddr,
                   sizeof(conf->reuseaddr)) == -1) {
        close(fd);
        unlink(conf->path);
        return -1;
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, conf->path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    unlink(conf->path);

    if (bind(fd, (struct sockaddr *) &local, len) == -1) {
        close(fd);
        unlink(conf->path);
        return -1;
    }

    if (listen(fd, conf->backlog) == -1) {
        close(fd);
        unlink(conf->path);
        return -1;
    }

    server->fd = fd;
    strcpy(server->path, conf->path);

    return 0;
}

void local_server_destroy(struct local_server *server)
{
    unlink(server->path);
    close(server->fd);
}

int local_server_do_step(struct local_server *server, void (*process)(int fd, void *data))
{
    int rfd;
    struct sockaddr_un local;
    socklen_t sin_size = sizeof(struct sockaddr_un);
    int numbytes;
    char buf[MAX_MSG_LENGTH];

    rfd = accept(server->fd, (struct sockaddr *) &local, &sin_size);

    if (rfd == -1)
        return -1;

    if ((numbytes = recv(rfd, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[sizeof(buf)-1] = '\0';

        if (process)
            process(rfd, buf);
    }

    close(rfd);

    return 0;
}

int local_client_create(struct local_conf *conf)
{
    socklen_t len;
    struct sockaddr_un local;
    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        return -1;

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, conf->path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);

    if (connect(fd, (struct sockaddr *) &local, len) == -1) {
        perror("connect: ");
        close(fd);
        return -1;
    }

    return fd;
}

void local_client_destroy(int fd)
{
    close(fd);
}

int local_client_do_step(int fd, void (*process)(void *buf))
{
    int numbytes;
    char buf[MAX_MSG_LENGTH];

    memset(buf, 0, sizeof(buf));

    while ((numbytes = recv(fd, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[sizeof(buf)-1] = '\0';

        if (process)
            process(buf);

        memset(buf, 0, sizeof(buf));
    }

    return 0;
}

void local_step(char *buf)
{
    printf("%s", buf);
}

int local_do_request(char* request, struct local_conf *conf,
                     void (*step)(void *buf))
{
    int fd, ret;

    fd = local_client_create(conf);

    if (fd == -1)
        return -1;

    ret = send(fd, request, strlen(request), 0);

    if (ret == -1)
        return -1;

    local_client_do_step(fd, step);

    local_client_destroy(fd);

    return 0;
}
