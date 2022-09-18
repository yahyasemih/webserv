//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server_config.hpp"

const std::string &server_config::get_host() const {
    return host;
}

void server_config::set_host(const std::string &host) {
    server_config::host = host;
}

short server_config::get_port() const {
    return port;
}

void server_config::set_port(short port) {
    server_config::port = port;
}

const std::unordered_set<std::string> &server_config::get_server_names() const {
    return server_names;
}

const std::string &server_config::get_root() const {
    return root;
}

void server_config::set_root(const std::string &root) {
    this->root = root;
}

void server_config::set_server_names(const std::unordered_set<std::string> &server_names) {
    server_config::server_names = server_names;
}

const std::vector<location_config> &server_config::get_location_configs() const {
    return location_configs;
}

void server_config::set_location_configs(const std::vector<location_config> &location_configs) {
    server_config::location_configs = location_configs;
}

const std::string &server_config::get_error_page() const {
    return error_page;
}

void server_config::set_error_page(const std::string &error_page) {
    server_config::error_page = error_page;
}

const std::string &server_config::get_access_log() const {
    return access_log;
}

void server_config::set_access_log(const std::string &access_log) {
    server_config::access_log = access_log;
}

const std::string &server_config::get_error_log() const {
    return error_log;
}

void server_config::set_error_log(const std::string &error_log) {
    server_config::error_log = error_log;
}

const std::string &server_config::get_client_max_body_size() const {
    return client_max_body_size;
}

void server_config::set_client_max_body_size(const std::string &client_max_body_size) {
    server_config::client_max_body_size = client_max_body_size;
}

const std::string &server_config::get_index() const {
    return index;
}

void server_config::set_index(const std::string &index) {
    server_config::index = index;
}
