//
// Created by victor on 3/17/21.
//
#include "../src/program_shell.h"
//#include "program_shell.h"

int main() {
    /*hello();

    int err = 0;
    if (!fork()) {
        char *args[] = {"st", "-c scratch", nullptr};
        err = execvp(args[0], args);
    }*/
    auto err = init(33850);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1);
#pragma clang diagnostic pop
}
