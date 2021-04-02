//
// Created by victor on 3/29/21.
//

#include "terminal.h"
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

std::thread* terminal::create(int connection_fd,
                   env_map* env_global,
                   const path_map* path){
    return new std::thread(th_function, env_global, connection_fd);
}

void terminal::th_function(env_map *env_global, int connection_fd) {
    env_map env_local;

    for (auto i = env_global->begin(); i != env_global->end(); ++i) {
        env_var pair = {i->first, i->second};
        env_local.insert(pair);
    }

    std::string term_header = (env_local)["TERM_HEADER"];
    send(connection_fd, term_header.c_str(), term_header.size(), 0);

    env_var test_var = {"test", "lol"};

    env_local.insert(test_var);
    std::string test_str = (env_local)["test"];
    std::cout << "[INSIDE  scope] " << connection_fd << " : " << (env_local)["test"] << std::endl;

    usleep(1000000);

    std::cout << "[INSIDE  scope] " << connection_fd << " : " << (env_local)["TERM_HEADER"] << std::endl;

    close(connection_fd);
}
