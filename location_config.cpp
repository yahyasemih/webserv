//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#include "location_config.hpp"

const std::string &location_config::get_error_page() const {
    return error_page;
}

void location_config::set_error_page(const std::string &error_page) {
    location_config::error_page = error_page;
}

const std::string &location_config::get_client_max_body_size() const {
    return client_max_body_size;
}

void location_config::set_client_max_body_size(const std::string &client_max_body_size) {
    location_config::client_max_body_size = client_max_body_size;
}

const std::vector<std::string> &location_config::get_accepted_methods() const {
    return accepted_methods;
}

void location_config::set_accepted_methods(const std::vector<std::string> &accepted_methods) {
    location_config::accepted_methods = accepted_methods;
}

const std::string &location_config::get_redirect() const {
    return redirect;
}

void location_config::set_redirect(const std::string &redirect) {
    location_config::redirect = redirect;
}

const std::string &location_config::get_index() const {
    return index;
}

void location_config::set_index(const std::string &index) {
    location_config::index = index;
}
