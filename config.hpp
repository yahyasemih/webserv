//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP


#include "http_config.hpp"

class config {
public:
    config();
    http_config &get_http_conf();
    void set_http_conf(const http_config &http_conf);
    const std::string &get_error_log() const;
    void set_error_log(const std::string &error_log);

private:
    http_config http_conf;
    std::string error_log;
};

#endif //WEBSERV_CONFIG_HPP
