//
// Created by victor on 3/29/21.
//

#ifndef PROGRAM_SHELL_TERMINAL_H
#define PROGRAM_SHELL_TERMINAL_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unordered_map>
#include <string>
#include <thread>
#include <cstring>
#include <sys/stat.h>

namespace terminal {
    class term_stream;
}

typedef std::pair<std::string, std::string> env_var;
typedef std::unordered_map<std::string, std::string> env_map;

typedef int (*command)(terminal::term_stream& stream, int argc, const char* argv[]);
typedef std::pair<std::string, command> path_entry;
typedef std::unordered_map<std::string, command> path_map;

namespace terminal {
    class term_stream {
    public:
        explicit term_stream(int fd): file_decryptor(fd) {}
        ~term_stream() { (void)close(file_decryptor); }

        term_stream& operator<< (const std::string& output) { return transmit((void*)output.c_str(), output.size()); }
        term_stream& operator>> (std::string& input) {
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
    private:
        int file_decryptor;

        term_stream& receive(void* data, int len) {
            int bytes, off = 0;
            do {
                bytes = recv(file_decryptor,
                            (void*)((uint8_t*)data + off),
                            len - off,
                            0);
                off += bytes;
            } while (off < len && bytes > 0);

            return *this;
        }
        term_stream& transmit(void* data, int len) {
            (void)send(file_decryptor, data, len, 0);
            return *this;
        }
    };

    std::thread *create(int connection_fd,
                        const path_map *path,
                        const env_map *env_global);

    void th_function(term_stream* stream,
                     const path_map *path,
                     const env_map* env_global);
}


#endif //PROGRAM_SHELL_TERMINAL_H
