#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    struct ifaddrs *id;
    struct sockaddr_in *sin;
    int val = getifaddrs(&id);

    while (NULL != id->ifa_next)
    {
        if (NULL != id->ifa_addr)
        {
            if (AF_INET == id->ifa_addr->sa_family)
            {
                sin = (struct sockaddr_in *)(id->ifa_addr);
                printf ("%s:\n\tip %s\n", id->ifa_name,
                    inet_ntoa (sin->sin_addr));
            }
        }
        id = id->ifa_next;
    }

    return 0;
}
