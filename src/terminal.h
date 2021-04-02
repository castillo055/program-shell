//
// Created by victor on 3/29/21.
//

#ifndef PROGRAM_SHELL_TERMINAL_H
#define PROGRAM_SHELL_TERMINAL_H

#include <unordered_map>
#include <string>
#include <thread>

typedef int (*command)(int argc, char* argv[]);
typedef std::unordered_map<std::string, std::string> env_map;
typedef std::pair<std::string, std::string> env_var;
typedef std::unordered_map<std::string, command> path_map;

namespace terminal {
    std::thread* create(int connection_fd,
                       env_map* env_global,
                       const path_map* path);

    void th_function(env_map* env_global, int connection_fd);
}


#endif //PROGRAM_SHELL_TERMINAL_H
