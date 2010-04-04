#ifndef COMM_INTERFACE_H
#define COMM_INTERFACE_H

#include <netinet/in.h>
#include <netlink/addr.h>

#define MSG_HELP "help"
#define MSG_QUIT "quit"
#define MSG_KILL "kill"
#define MSG_RESTART "restart"
#define MSG_SHOW_ROUTES "showroutes"
#define MSG_SHOW_STATISTICS "showstatistics"
#define MSG_CLEAN_STATISTICS "cleanstatistics"

struct route {
    struct in_addr dst_ip;
    uint8_t prefix_sz;
    uint32_t dest_seq_num;
    uint16_t flags;
    uint8_t hop_count;
    struct in_addr next_hop;
    uint32_t net_iface;
};

int comm_interface_init();
void comm_interface_shutdown();
void comm_process_command(int fd, char* command);
void comm_interface_receive_packets();

#endif
