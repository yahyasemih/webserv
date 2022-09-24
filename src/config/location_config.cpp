//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#include "location_config.hpp"

location_config::location_config() : route("/"), client_max_body_size(), redirect("no"), upload_dir("."),
        list_directory(true) {
    accepted_methods.insert("GET");
}

const std::string &location_config::get_root() const {
    return root;
}

void location_config::set_root(const std::string &root) {
    this->root = root;
}

const std::string &location_config::get_route() const {
    return route;
}

void location_config::set_route(const std::string &route) {
    this->route = route;
}

const std::string &location_config::get_cgi_path() const {
    return cgi_path;
}

void location_config::set_cgi_path(const std::string &cgi_path) {
    this->cgi_path = cgi_path;
}

const std::string &location_config::get_cgi_extension() const {
    return cgi_extension;
}

void location_config::set_cgi_extension(const std::string &cgi_extension) {
    this->cgi_extension = cgi_extension;
}

const std::string &location_config::get_error_page() const {
    return error_page;
}

void location_config::set_error_page(const std::string &error_page) {
    this->error_page = error_page;
}

size_t location_config::get_client_max_body_size() const {
    return client_max_body_size;
}

void location_config::set_client_max_body_size(size_t client_max_body_size) {
    this->client_max_body_size = client_max_body_size;
}

const std::set<std::string> &location_config::get_accepted_methods() const {
    return accepted_methods;
}

void location_config::set_accepted_methods(const std::set<std::string> &accepted_methods) {
    this->accepted_methods = accepted_methods;
}

const std::string &location_config::get_redirect() const {
    return redirect;
}

void location_config::set_redirect(const std::string &redirect) {
    this->redirect = redirect;
}

const std::vector<std::string> &location_config::get_indexes() const {
    return indexes;
}

void location_config::set_indexes(const std::vector<std::string> &indexes) {
    this->indexes = indexes;
}

const std::string &location_config::get_upload_dir() const {
    return upload_dir;
}
void location_config::set_upload_dir(const std::string &upload_dir) {
    this->upload_dir = upload_dir;
}

bool location_config::is_list_directory() const {
    return list_directory;
}

void location_config::set_list_directory(bool list_directory) {
    this->list_directory = list_directory;
}

bool location_config::is_cgi_route() const {
    return !cgi_path.empty() && !cgi_extension.empty();
}
