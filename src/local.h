#ifndef _LOCAL_SOCKET_H_
#define _LOCAL_SOCKET_H_

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX   108
#endif

#define MAX_MSG_LENGTH 1024

struct local_conf {
    int backlog;
    int reuseaddr;
    char path[UNIX_PATH_MAX];
};

struct local_server {
    int fd;
    char path[UNIX_PATH_MAX];
};

/* local server */
/**To create the listening socket*/
int local_server_create(struct local_server *server, struct local_conf *conf);

/** To delete the listening socket */
void local_server_destroy(struct local_server *server);

/** To create a answer to the request */
int local_server_do_step(struct local_server *server, void (*process)(int fd, void *data));

/* local client */
/** Create a client socket, this function mustn't be used directly.  */
static int local_client_create(struct local_conf *conf);

/** Destroy a client socket, this function mustn't be used directly.  */
static void local_client_destroy(int fd);

/** This function receives the request from the socket and then calls
  * process function with the request. This function musn't be used
  * directly.
  */
static int local_client_do_step(int fd, void (*process)(void *buf));

/** Only this function must be used. It creates the socket, it sends the request
  * and it waits for the answer. The function step process the answer. Then
  * the socket is destroyed automatically. @param request we want to send.
  */
int local_do_request(char* request, struct local_conf *,
                     void (*step)(void *buf));

/** Auxiliar function to be used @see local_do_request. It only prints
  * the argument buf;
  */
void local_step(char *buf);

#endif
