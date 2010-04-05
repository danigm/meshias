struct meshias_route
{
    uint32_t dest;
    uint32_t gw;
    uint16_t hops;
    uint32_t expire_time;
    struct meshias_route* null;
}

struct meshias_statistics
{
}

struct meshias_aodv_options
{
}

typedef enum
{
    // void
    meshias_packet_connect_req,
    // uint8_t answer
    meshias_packet_connect_res,

    // void
    meshias_packet_close_req,
    // uint8_t answer
    meshias_packet_close_res,

    // void
    meshias_packet_start_req,
    // uint8_t answer
    meshias_packet_start_res,

    // void
    meshias_packet_stop_req,
    // uint8_t answer
    meshias_packet_stop_res,

    // void
    meshias_packet_kill_req,
    // uint8_t answer
    meshias_packet_kill_res,

    // struct meshias_route
    meshias_packet_add_route_req,
    // uint8_t answer
    meshias_packet_add_route_res,

    // struct meshias_route
    meshias_packet_del_route_req,
    // uint8_t answer
    meshias_packet_del_route_res,

    // void
    meshias_packet_get_route_list_req,
    // struct meshias_route (list)
    meshias_packet_get_route_list_res,

    // void
    meshias_packet_get_aodv_options_req,
    // uint8_t answer
    meshias_packet_get_aodv_options_res,

    // struct meshias_aodv_options
    meshias_packet_set_aodv_options_req,
    // void
    meshias_packet_set_aodv_options_res,

    // void
    meshias_packet_get_interface_req,
    // char* intefaz
    meshias_packet_get_interface_res

} meshias_packet_t;



struct meshias_packet
{
    uint32_t size;
    uint8_t type;
}
