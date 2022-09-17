//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#include "http_config.hpp"

http_config::http_config(const std::string &) {
    server_config server_conf;
    server_conf.set_host("127.0.0.1");
    server_conf.set_port(8080);
    server_configs.push_back(server_conf);
    server_conf.set_host("127.0.0.1");
    server_conf.set_port(9090);
    server_configs.push_back(server_conf);
}

const std::vector<server_config> &http_config::get_server_configs() const {
    return server_configs;
}

void http_config::set_server_configs(const std::vector<server_config> &server_configs) {
    http_config::server_configs = server_configs;
}

const std::string &http_config::get_error_page() const {
    return error_page;
}

void http_config::set_error_page(const std::string &error_page) {
    http_config::error_page = error_page;
}

const std::string &http_config::get_access_log() const {
    return access_log;
}

void http_config::set_access_log(const std::string &access_log) {
    http_config::access_log = access_log;
}

const std::string &http_config::get_error_log() const {
    return error_log;
}

void http_config::set_error_log(const std::string &error_log) {
    http_config::error_log = error_log;
}

const std::string &http_config::get_client_max_body_size() const {
    return client_max_body_size;
}

void http_config::set_client_max_body_size(const std::string &client_max_body_size) {
    http_config::client_max_body_size = client_max_body_size;
}

const std::string &http_config::get_index() const {
    return index;
}

void http_config::set_index(const std::string &index) {
    http_config::index = index;
}
