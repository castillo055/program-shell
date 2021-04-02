#include "./program_shell.h"

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

int init(unsigned short port_number) {
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

    new std::thread([&, socket_fd]{
        await_connection(socket_fd, 5);
    });

    return 0;
}

int test(int argc, char* argv[]) {
    std::cout << "hello world!" << std::endl;

    return 0;
}

void await_connection(int socket_fd, int conn_queue_size) {
    listen(socket_fd, conn_queue_size);

    env_map env;
    env.insert(std::pair<std::string, std::string>{"TERM_HEADER", std::string("[TESTING VERSION v0.1a]\n")});
    env.insert(std::pair<std::string, std::string>{"TERM_PROMPT", std::string("$_>")});

    path_map path;
    path.insert(std::pair<std::string, command>{"test", test});

    sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    while (true) {
        int conn_socket_fd = accept(socket_fd, (struct sockaddr *) &cli_addr, &cli_addr_len);

        if (conn_socket_fd <0) {
            std::cout << "[ERROR] Couldn't establish connection (" << errno << ")" << std::endl;
            continue;
        }

        std::cout << "New connection accepted" << std::endl;
        std::thread* th = terminal::create(conn_socket_fd, &env, &path);

        usleep(1000000);

        std::cout << "[AFTER  scope] " << env["test"] << std::endl;
    }

    close(socket_fd);
}
