//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <string>
#include "config.hpp"

class server {
    config conf;
public:
    server();
    explicit server(const std::string &config_file);
};

#endif //WEBSERV_SERVER_HPP
