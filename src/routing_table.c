#include "routing_table.h"

static struct nl_af_group msh_route_groups[] =
{
    { AF_INET, RTNLGRP_IPV4_ROUTE },
    { END_OF_GROUP_LIST },
};


static struct nl_cache_ops msh_route_ops = {
    .co_name                = "route/meshias",
    .co_hdrsize             = sizeof(struct msh_route),
    .co_msgtypes            = {
        { RTM_NEWROUTE, NL_ACT_NEW, "new" },
        { RTM_DELROUTE, NL_ACT_DEL, "del" },
        { RTM_GETROUTE, NL_ACT_GET, "get" },
        END_OF_MSGTYPES_LIST,
    },
    .co_protocol            = NETLINK_ROUTE,
    .co_groups              = msh_route_groups,
    .co_request_update      = msh_route_request_update,
    .co_msg_parser          = msh_route_msg_parser,
    .co_obj_ops             = &msh_route_obj_ops,
};
