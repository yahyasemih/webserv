//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#include "config.hpp"

config::config(const std::string &) : http_conf("") {
}

const http_config &config::get_http_conf() const {
    return http_conf;
}

void config::set_http_conf(const http_config &http_conf) {
    this->http_conf = http_conf;
}

const std::string &config::get_error_log() const {
    return error_log;
}

void config::set_error_log(const std::string &error_log) {
    this->error_log = error_log;
}