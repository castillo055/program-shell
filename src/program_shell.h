#ifndef PROGRAM_SHELL_PROGRAM_SHELL_H
#define PROGRAM_SHELL_PROGRAM_SHELL_H

#include "terminal.h"

int init(unsigned short port_number);

void await_connection(int socket_fd, int conn_queue_size);

#endif //PROGRAM_SHELL_PROGRAM_SHELL_H
