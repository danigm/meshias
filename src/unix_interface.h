#ifndef UNIX_INTERFACE_H
#define UNIX_INTERFACE_H

enum commands_t
{
    KILL,
    RESTART,
    SHOW_ROUTES,
    SHOW_STATISTICS,
    CLEAN_STATISTICS
};

int unix_interface_init();
void unix_interface_receive_packets();
void unix_interface_shutdown();
void unix_interface_run_command(enum commands_t command);

#endif
