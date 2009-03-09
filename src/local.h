#ifndef _LOCAL_SOCKET_H_
#define _LOCAL_SOCKET_H_

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX   108
#endif

struct local_conf
{
    int backlog;
    int reuseaddr;
    char path[UNIX_PATH_MAX];
};

struct local_server
{
    int fd;
    char path[UNIX_PATH_MAX];
};

/* local server */
int local_server_create(struct local_server *server, struct local_conf *conf);
void local_server_destroy(struct local_server *server);
int do_local_server_step(struct local_server *server, void *data, 
    void (*process)(int fd, void *data));

/* local client */
int local_client_create(struct local_conf *conf);
void local_client_destroy(int fd);
int do_local_client_step(int fd, void (*process)(char *buf));
int do_local_request(int, struct local_conf *,void (*step)(char *buf));
void local_step(char *buf);

#endif
