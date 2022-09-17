//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server.hpp"

int main(int argc, char **argv) {
    std::string path;
    if (argc == 1) {
        path = "default.conf";
    } else {
        path = argv[0];
    }
    server s(path);
    s.start();
    return 0;
}