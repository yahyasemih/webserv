//
// Created by Yahya Ez-zainabi on 9/21/22.
//

#include "request_builder.hpp"

const std::string &request_builder::get_method() const {
    return method;
}

request_builder &request_builder::set_method(const std::string &method) {
    this->method = method;
    return *this;
}

const std::string &request_builder::get_path() const {
    return path;
}

request_builder &request_builder::set_path(const std::string &path) {
    this->path = path;
    return *this;
}

const std::string &request_builder::get_query_string() const {
    return query_string;
}

request_builder &request_builder::set_query_string(const std::string &query_string) {
    this->query_string = query_string;
    return *this;
}

const std::string &request_builder::get_http_version() const {
    return http_version;
}

request_builder &request_builder::set_http_version(const std::string &http_version) {
    this->http_version = http_version;
    return *this;
}

const std::string &request_builder::get_body() const {
    return body;
}

request_builder &request_builder::set_body(const std::string &body) {
    this->body += body;
    return *this;
}

const std::string &request_builder::get_header(const std::string &key) const {
    return headers.at(key);
}

std::string &request_builder::get_header(const std::string &key) {
    return headers[key];
}

request_builder &request_builder::set_header(const std::string &key, const std::string &value) {
    this->headers.insert(std::make_pair(key, value));
    return *this;
}
