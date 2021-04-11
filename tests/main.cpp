//
// Created by victor on 3/17/21.
//
#include <program_shell.h>
//#include "program_shell.h"

#include <immintrin.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int test(terminal::term_stream& stream, int argc, const char* argv[]) {
    stream << "hello world! " << to_string(argc) << ": ";
    for (int i = 0; i < argc; i++)
        stream << "-" << argv[i];

    return 0;
}

int main() <%
    /*hello();

    int err = 0;
    if (!fork()) {
        char *args[] = {"st", "-c scratch", nullptr};
        err = execvp(args[0], args);
    }*/
    program_shell::set_var("TERM_HEADER", "[TESTING VERSION v0.1a]");
    program_shell::set_var("TERM_PROMPT", "$_>");

    program_shell::add_cmd("test", test);

    auto err = program_shell::init(33851);
    cout << err << endl;

    /*if (fork() == 0) {
        program_shell::stop();
        execv("/home/victor/.local/bin/st", {});
    }*/
    //conn_listener_th->join();
while(1);
    //delete conn_listener_th;
%>
