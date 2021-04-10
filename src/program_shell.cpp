#include "program_shell.h"

#include <iostream>
#include <cerrno>

using namespace std;

int program_shell::init(unsigned short port_number) {
    // Allocate socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) return -1;

    // Set socket to be reused
    const int enabled = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(int)) < 0)
        return -2;

    // Bind socket
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_number);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(socket_fd, (const sockaddr*) &addr, sizeof(addr)) <0) return -3;

    // Start new thread and await connection
    conn_listener_th = new thread([&, socket_fd]{
        await_connection(socket_fd, 5);
    });

    return 0;
}

void program_shell::await_connection(int socket_fd, int conn_queue_size) {
    (void)listen(socket_fd, conn_queue_size);

    sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    while (true) {
        int conn_socket_fd = accept(socket_fd, (struct sockaddr *) &cli_addr, &cli_addr_len);

        if (conn_socket_fd <0) {
            switch (errno) {
                default:
                    cout << "[ERROR] Couldn't establish connection (" << errno << ")" << endl;
                    continue;
            }
        }

        cout << "New connection accepted" << endl;
        thread* th = terminal::create(conn_socket_fd, &PATH, &global_environment);
    }

    close(socket_fd);
}

void program_shell::set_var(string name, string value) {
    (void)global_environment.insert({name, value});
}
void program_shell::add_cmd(string name, command function) {
    PATH.insert({name, function});
}
