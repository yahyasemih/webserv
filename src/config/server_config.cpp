//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server_config.hpp"

server_config::server_config() : client_max_body_size() {
}

void server_config::add_address(const std::string &ip, in_port_t port) {
    address_port address(ip, port);
    this->addresses.insert(address);
}

const std::set<address_port> &server_config::get_addresses() const {
    return addresses;
}

const std::string &server_config::get_root() const {
    return root;
}

void server_config::set_root(const std::string &root) {
    this->root = root;
}

const std::set<std::string> &server_config::get_server_names() const {
    return server_names;
}

void server_config::set_server_names(const std::set<std::string> &server_names) {
    this->server_names = server_names;
}

std::vector<location_config> &server_config::get_location_configs() {
    return location_configs;
}

const std::vector<location_config> &server_config::get_location_configs() const {
    return location_configs;
}

void server_config::set_location_configs(const std::vector<location_config> &location_configs) {
    this->location_configs = location_configs;
}

const std::string &server_config::get_error_page() const {
    return error_page;
}

void server_config::set_error_page(const std::string &error_page) {
    this->error_page = error_page;
}

const std::string &server_config::get_access_log() const {
    return access_log;
}

void server_config::set_access_log(const std::string &access_log) {
    this->access_log = access_log;
}

const std::string &server_config::get_error_log() const {
    return error_log;
}

void server_config::set_error_log(const std::string &error_log) {
    this->error_log = error_log;
}

size_t server_config::get_client_max_body_size() const {
    return client_max_body_size;
}

void server_config::set_client_max_body_size(size_t client_max_body_size) {
    this->client_max_body_size = client_max_body_size;
}

const std::vector<std::string> &server_config::get_indexes() const {
    return indexes;
}

void server_config::set_indexes(const std::vector<std::string> &indexes) {
    this->indexes = indexes;
}

const std::string &server_config::get_cgi_path() const {
    return cgi_path;
}

void server_config::set_cgi_path(const std::string &cgi_path) {
    this->cgi_path = cgi_path;
}

const std::string &server_config::get_cgi_extension() const {
    return cgi_extension;
}

void server_config::set_cgi_extension(const std::string &cgi_extension) {
    this->cgi_extension = cgi_extension;
}

bool server_config::is_cgi_route() const {
    return !cgi_path.empty() && !cgi_extension.empty();
}
