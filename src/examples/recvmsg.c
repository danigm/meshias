#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>



#define BUF_SIZE 2024
#define SIZE 1024
#define AODV_UDP_PORT 1654

int aodv_get_ttl(struct msghdr* msgh)
{
    struct cmsghdr *cmsg;
    int ttl;

    /* Recibir los datos auxiliares en msgh */
    for (cmsg = CMSG_FIRSTHDR(msgh); cmsg != NULL;
            cmsg = CMSG_NXTHDR(msgh, cmsg)) {
        printf("len: %d, level: %d, type: %d\n", cmsg->cmsg_len, cmsg->cmsg_level, cmsg->cmsg_type);

        if (cmsg->cmsg_level == SOL_IP
                && cmsg->cmsg_type == IP_TTL) {
            ttl = *(int*)CMSG_DATA(cmsg);
        }
    }

    /*
     * FIXME TTL no encontrado
     */
    return ttl;
}

struct in_pktinfo* get_pkt(struct msghdr* msgh) {
    struct cmsghdr *cmsg;

    /* Recibir los datos auxiliares en msgh */
    for (cmsg = CMSG_FIRSTHDR(msgh); cmsg != NULL;
            cmsg = CMSG_NXTHDR(msgh, cmsg)) {
        if (cmsg->cmsg_level == SOL_IP
                && cmsg->cmsg_type == IP_PKTINFO) {
            return (struct in_pktinfo*)CMSG_DATA(cmsg);
        }
    }

    return NULL;
}

int aodv_get_type(const char* b)
{
    const int *type = (const int*)b;
    return *type;
}

int main(int argc, char *argv[])
{
    int fd = 0;
    int fd2 = 0;

    char buf[SIZE];
    char buffer[SIZE] = "morte";
    char crap[SIZE] = "morte";
    struct iovec io;
    struct sockaddr_in address;
    int broadcast = 1;
    int numbytes;
    struct msghdr msgh;

    //if( (fd = socket(AF_INET,SOCK_STREAM,0)) == -1 )
    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("error socket");
        return -1;
    }

    //IPv4
    address.sin_family = AF_INET;
    // Set port
    address.sin_port = htons(AODV_UDP_PORT);
    // Listen from any ip
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *)&address,
             sizeof(address)) == -1) {
        perror("error bind");
        close(fd);
        return -1;
    }

    if (setsockopt(fd, SOL_IP, IP_RECVTOS, &broadcast,
                   sizeof broadcast) == -1) {
        perror("error set socket");
        close(fd);
        return -1;
    }

    if (setsockopt(fd, SOL_IP, IP_PKTINFO, &broadcast,
                   sizeof broadcast) == -1) {
        perror("error set socket");
        close(fd);
        return -1;
    }

    if (setsockopt(fd, SOL_IP, IP_RECVTTL, &broadcast,
                   sizeof broadcast) == -1) {
        perror("error set socket");
        close(fd);
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcast,
                   sizeof broadcast) == -1) {
        perror("error set socket");
        close(fd);
        return -1;
    }

    while (1) {
        memset(&msgh, 0, sizeof(msgh));

        memset(buf, 0, sizeof(buf));
        msgh.msg_name = &buf;
        msgh.msg_namelen = SIZE;

        memset(buffer, 0, sizeof(buffer));
        io.iov_base = buffer;
        io.iov_len = SIZE;

        msgh.msg_iov = &io;
        msgh.msg_iovlen = 1;

        msgh.msg_control = &crap;
        msgh.msg_controllen = sizeof(crap);

        //Receive the packet
        if ((numbytes = recvmsg(fd, &msgh, 0)) == -1) {
            perror("FATAL ERROR: recvmsg");
            return (-1);
        }

        printf("recibido: %d\n", numbytes);

        if (msgh.msg_name == NULL) {
            puts("puta");
        }

        printf("name:%d, iov:%d, control:%d, flags:%d another:%d\n",
               msgh.msg_namelen, msgh.msg_iovlen,
               msgh.msg_controllen, msgh.msg_flags, msgh.msg_iov->iov_len);
        printf("Buffer: %s\n", buffer);
        printf("Daemon: Packet has been received with ttl %d\n",
               aodv_get_ttl(&msgh));
        //printf("%s \n",inet_ntoa(msgh.msg_name));
        struct sockaddr_in addr = *(struct sockaddr_in*)msgh.msg_name;
        printf("recibido de %s:%d \n", inet_ntoa(addr.sin_addr), htons(addr.sin_port));

        switch (msgh.msg_flags) {
        case MSG_EOR:
            puts("MSG_EOR");
            break;
        case MSG_TRUNC:
            puts("MSG_TRUNC");
            break;
        case MSG_CTRUNC:
            puts("MSG_CTRUNC");
            break;
        case MSG_OOB:
            puts("MSG_OOB");
            break;
        case MSG_ERRQUEUE:
            puts("MSG_ERRQUEUE");
            break;
        case MSG_DONTWAIT:
            puts("MSG_DONTWAIT");
            break;
        }
    }

    close(fd);

    return 0;
}
