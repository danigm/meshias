#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>


int main()
{
    int sequence_number=0;
    int fd;
    struct nlmsghdr *nh;    /* The nlmsghdr with payload to send. */
    struct sockaddr_nl sa;
    struct iovec iov = { (void *) nh, nh->nlmsg_len };
    struct msghdr msg = { (void *)&sa, sizeof(sa), &iov, 1, NULL, 0, 0 };

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    nh->nlmsg_pid = 0;
    nh->nlmsg_group=0;
    nh->nlmsg_seq = ++sequence_number;
    /* Request an ack from kernel by setting NLM_F_ACK. */
    nh->nlmsg_flags |= NLM_F_ACK;

    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    //  bind(fd, (struct sockaddr*)&sa, sizeof(sa));

    sendmsg(fd, &msg, 0);

    return 0;
}
