#ifndef MESHIAS_TOOLS_H
#define MESHIAS_TOOLS_H

int main(int argc, char **argv);
int get_command(char *command);
int check_command(char *command);
int is_help_command(char *command);
void help_command();
int is_quit_command(char *command);
int send_command(char* command);

#endif
