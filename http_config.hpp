//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#ifndef WEBSERV_HTTP_CONFIG_HPP
#define WEBSERV_HTTP_CONFIG_HPP


#include "server_config.hpp"

class http_config {
    std::vector<server_config> server_configs; // could be a std::unordered_map "host:port" -> server_config
    std::string error_page;
    std::string access_log;
    std::string error_log;
    std::string client_max_body_size;
    std::string index;
};

#endif //WEBSERV_HTTP_CONFIG_HPP
