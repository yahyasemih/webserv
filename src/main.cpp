//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server.hpp"

int main(int argc, char **argv) {
    std::string path;
    if (argc == 1) {
        path = "default.conf";
    } else {
        path = argv[1];
    }
    try {
        server s(path);
        s.start();
    } catch (const std::exception &e) {
        std::cerr << "Error while starting server: " << e.what() << std::endl;
    }
    std::cout << std::tostring(10) << std::endl;
    return 0;
}