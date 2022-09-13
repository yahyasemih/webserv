//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#ifndef WEBSERV_SERVER_CONFIG_HPP
#define WEBSERV_SERVER_CONFIG_HPP

#include <string>
#include <vector>

#include "location_config.hpp"

class server_config {
    std::string host;
    short port;
    std::vector<std::string> server_names;
    std::vector<location_config> location_configs;
    std::string error_page;
    std::string access_log;
    std::string error_log;
    std::string client_max_body_size;
    std::string index;
};

#endif //WEBSERV_SERVER_CONFIG_HPP
