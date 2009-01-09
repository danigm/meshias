#ifndef _MCAST_H_
#define _MCAST_H_

#include <stdint.h>
#include <netinet/in.h>
#include <net/if.h>

struct mcast_conf {
	int ipproto;
	int reuseaddr;
	int checksum;
	unsigned short port;
	union {
		struct in_addr inet_addr;
		struct in6_addr inet_addr6;
	} in;
	union {
		struct in_addr interface_addr;
		unsigned int interface_index6;
	} ifa;
	int mtu;
	int sndbuf;
	int rcvbuf;
	char iface[IFNAMSIZ];
};

struct mcast_stats {
	uint64_t bytes;
	uint64_t messages;
	uint64_t error;
};

struct mcast_sock {
	int fd;
	union {
		struct sockaddr_in ipv4;
		struct sockaddr_in6 ipv6;
	} addr;
	socklen_t sockaddr_len;
	struct mcast_stats stats;
};

struct mcast_sock *mcast_server_create(struct mcast_conf *conf);
void mcast_server_destroy(struct mcast_sock *m);

struct mcast_sock *mcast_client_create(struct mcast_conf *conf);
void mcast_client_destroy(struct mcast_sock *m);

ssize_t mcast_send(struct mcast_sock *m, void *data, int size);
ssize_t mcast_recv(struct mcast_sock *m, void *data, int size);

struct mcast_stats *mcast_get_stats(struct mcast_sock *m);
void mcast_dump_stats(int fd, struct mcast_sock *s, struct mcast_sock *r);

#endif
