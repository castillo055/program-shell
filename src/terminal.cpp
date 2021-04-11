//
// Created by victor on 3/29/21.
//

#include "terminal.h"

#include <iostream>
#include <list>
#include <bits/types/siginfo_t.h>
#include <csignal>

using namespace std;

typedef list<string> arg_list;

thread * terminal::create(int connection_fd, const path_map *path, const env_map *env_global) {
    auto* stream = new term_stream(connection_fd);

    auto* th = new thread(th_function, stream, path, env_global);
    return th;
}

void terminal::th_function(terminal::term_stream* stream, const path_map *path, const env_map *env_global) {
    env_map env_local;

    for (auto & i : *env_global) {
        env_var pair = {i.first, i.second};
        (void)env_local.insert(pair);
    }

    string term_header = env_local["TERM_HEADER"];
    string term_prompt = env_local["TERM_PROMPT"];

    *stream << term_header << "\n" << term_prompt;

    string input, cmd;
    while ((*stream >> input, cmd != "exit")) {
        int index, prev_index = 0;
        arg_list args;

        // Trim input
        input = input.substr(0, input.find_last_of('\n'));

        if ((index = input.find(' ')) == string::npos) {
            cmd = input;
        } else {
            cmd = input.substr(0, index);
            prev_index = index;

            index = input.find(' ', prev_index+1);

            while (prev_index != string::npos) {
                string arg = input.substr(prev_index+1,
                                          index==string::npos?
                                          input.length()-prev_index-1 : index-prev_index-1);
                args.push_back(arg);
                prev_index = index;

                index = input.find(' ', prev_index+1);
            }
        }

        // Get argc and allocate argv array
        int argc = args.size() +1;
        const char* argv[argc];

        // Store command name in array
        argv[0] = cmd.c_str();

        // Store rest of arguments in array
        int j = 1;
        for (string const &s: args)
            argv[j++] = s.c_str();

        // Mask out SIGPIPE - This is necessary in case connection has been
        //  terminated. The function recv() doesn't report this error.
        sigset_t oldset;
        sigset_t newset;
        siginfo_t si;
        struct timespec ts = {0};

        (void)sigemptyset(&newset);
        (void)sigaddset(&newset, SIGPIPE);
        (void)pthread_sigmask(SIG_BLOCK, &newset, &oldset);

        // Call function from path
        int exit_code = 1;
        auto cmd_entry = path->find(cmd);
        if (cmd_entry == path->end())
            *stream << "program_shell: command not found: " << cmd;
        else {
            command cmd_function = cmd_entry->second;
            exit_code = cmd_function(*stream, argc, argv);
        }
        // Display exit code and prompt again
        *stream << "\n[" << to_string(exit_code) << "] " << term_prompt;

        // Wait for SIGPIPE and unmask it
        while (sigtimedwait(&newset, &si, &ts)>=0 || errno != EAGAIN) {
            cout << "Connection terminated" << endl;
            goto sigpipe;
        }
        (void)pthread_sigmask(SIG_SETMASK, &oldset, 0);
    }

    sigpipe:
    delete stream;
}

terminal::term_stream& terminal::term_stream::operator<< (const std::string& output) { return transmit((void*)output.c_str(), output.size()); }

terminal::term_stream& terminal::term_stream::operator>> (std::string& input) {
    (void)input.erase();

    char buf[128];
    int len = sizeof(buf) / sizeof(char);

    do {
        bzero(buf, len);

        int bytes = recv(file_decryptor, buf, len, 0);

        if (bytes > 0)
            (void)input.append(buf, len);
    } while (buf[len-1] != '\0');


    return *this;
}
