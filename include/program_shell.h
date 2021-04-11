//
// Created by victor on 4/2/21.
//

#ifndef PROGRAM_SHELL_PROGRAM_SHELL_H
#define PROGRAM_SHELL_PROGRAM_SHELL_H

#include <string>
#include <unordered_map>

namespace terminal {
    class term_stream {
    public:
        term_stream& operator<< (const std::string& output);
        term_stream& operator>> (std::string& input);
    };
}

namespace program_shell {

    typedef std::pair<std::string, std::string> env_var;
    typedef std::unordered_map<std::string, std::string> env_map;

    typedef int (*command)(terminal::term_stream& stream, int argc, const char *argv[]);

    typedef std::pair<std::string, command> path_entry;
    typedef std::unordered_map<std::string, command> path_map;

    extern int init(unsigned short port_number);
    extern void stop();

    extern void set_var(std::string name, std::string value);

    extern void add_cmd(std::string name, command function);
}

#endif //PROGRAM_SHELL_PROGRAM_SHELL_H
