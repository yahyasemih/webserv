//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP


#include "http_config.hpp"

class config {
    http_config http_conf;
    std::string error_log;
public:
    explicit config(const std::string &config_file);
};

#endif //WEBSERV_CONFIG_HPP
