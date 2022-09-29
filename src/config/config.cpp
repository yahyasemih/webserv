//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#include "config.hpp"

config::config() : error_log("/dev/stderr") {
}

http_config &config::get_http_conf() {
    return http_conf;
}

const std::string &config::get_error_log() const {
    return error_log;
}

void config::set_error_log(const std::string &error_log) {
    this->error_log = error_log;
}
