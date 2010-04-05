#include <stdlib.h>
#include "libmeshias.h"

static meshias_connection_unix *meshias_connection_alloc_unix(char *path);
static meshias_connection_tcp *meshias_connection_alloc_tcp(char *host, int port);

meshias_connection* meshias_connection_alloc(meshias_connection_t type, char* path) {
    meshias_connection *conn = NULL;
    int port;
    char host[255];

    switch (type) {
        case UNIX_SOCKET:
            conn = (meshias_connection *)meshias_connection_alloc_unix(path);
            break;
        case TCP_SOCKET:
            if (sscanf(path, "%s:%d", host, &port) != 2) {
                // TODO set error type
                return NULL;
            }
            conn = (meshias_connection *)meshias_connection_alloc_tcp(host, port);
            break;
        default:
            break;
    }
    return conn;
}

static meshias_connection_unix *meshias_connection_alloc_unix(char *path) {
    meshias_connection_unix *conn = malloc(sizeof(meshias_connection_unix));

    int fd;
    socklen_t len;
    struct sockaddr_un local;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        return NULL;
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);

    if (connect(fd, (struct sockaddr *) &local, len) == -1) {
        close(fd);
        return NULL;
    }

    conn->type = UNIX_SOCKET;
    conn->socket = fd;
    conn->path = malloc(strlen(path)*sizeof(char));
    memcpy(conn->path, path, strlen(path));

    return conn;
}

static meshias_connection_tcp *meshias_connection_alloc_tcp(char *host, int port) {
    meshias_connection_tcp *conn = malloc(sizeof(meshias_connection_tcp));

    int fd;
    struct sockaddr_in local;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return NULL;
    }

    struct hostent *he;
    if ((he = gethostbyname(host)) == NULL) {
        return NULL;
    }

    local.sin_family = AF_UNIX;
    local.sin_port = htons(port);
    local.sin_addr = *((struct in_local *)he->h_local);
    bzero(&(local.sin_zero), 8);

    if (connect(fd, (struct sockaddr *) &local, sizeof(struct sockaddr)) == -1) {
        close(fd);
        return NULL;
    }

    conn->type = TCP_SOCKET;
    conn->socket = fd;
    conn->port = port;
    conn->ip = local.sin_addr.s_addr;

    return conn;
}
