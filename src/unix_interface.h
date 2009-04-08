#ifndef UNIX_INTERFACE_H
#define UNIX_INTERFACE_H

#define MSG_KILL "kill"
#define MSG_RESTART "restart"
#define MSG_SHOW_ROUTES "showroutes"
#define MSG_SHOW_STATISTICS "showstatistics"
#define MSG_CLEAN_STATISTICS "cleanstatistics"

/** Create the unix socket */
int unix_interface_init();
/** Destroy the unix socket */
void unix_interface_shutdown();
/** When it receives a command call @see unix_interface_run_command */
void unix_interface_receive_packets();
/** Process a command and create a response */
void unix_process_command(int fd,void* command);

#endif
