//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#include "http_config.hpp"

http_config::http_config() : root("."), error_page("error.html"), access_log("/dev/stdout"),
        client_max_body_size(80 * 1024 * 1024) { // 80m
}

std::vector<server_config> &http_config::get_server_configs() {
    return server_configs;
}

void http_config::set_server_configs(const std::vector<server_config> &server_configs) {
    this->server_configs = server_configs;
}

const std::string &http_config::get_root() const {
    return root;
}

void http_config::set_root(const std::string &root) {
    this->root = root;
}

const std::string &http_config::get_error_page() const {
    return error_page;
}

void http_config::set_error_page(const std::string &error_page) {
    this->error_page = error_page;
}

const std::string &http_config::get_access_log() const {
    return access_log;
}

void http_config::set_access_log(const std::string &access_log) {
    this->access_log = access_log;
}

const std::string &http_config::get_error_log() const {
    return error_log;
}

void http_config::set_error_log(const std::string &error_log) {
    this->error_log = error_log;
}

size_t http_config::get_client_max_body_size() const {
    return client_max_body_size;
}

void http_config::set_client_max_body_size(size_t client_max_body_size) {
    this->client_max_body_size = client_max_body_size;
}

const std::vector<std::string> &http_config::get_indexes() const {
    return indexes;
}

void http_config::set_indexes(const std::vector<std::string> &indexes) {
    this->indexes = indexes;
}
