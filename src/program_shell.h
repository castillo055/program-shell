#ifndef PROGRAM_SHELL_PROGRAM_SHELL_H
#define PROGRAM_SHELL_PROGRAM_SHELL_H

#include "terminal.h"

namespace program_shell {
    std::thread *conn_listener_th;

    env_map global_environment;
    path_map PATH;

    int init(unsigned short port_number);
    void stop();

    void await_connection(int socket_fd, int conn_queue_size);

    void set_var(std::string name, std::string value);

    void add_cmd(std::string name, command function);
}

#endif //PROGRAM_SHELL_PROGRAM_SHELL_H
