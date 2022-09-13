//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server.hpp"

server::server() : conf("default.conf") {
}

server::server(const std::string &config_file) : conf(config_file) {
}
