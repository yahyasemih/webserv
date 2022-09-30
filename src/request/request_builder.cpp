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

const std::string &request_builder::get_uri() const {
    return uri;
}

request_builder &request_builder::set_uri(const std::string &uri) {
    this->uri = uri;
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

std::vector<char> &request_builder::get_body() {
    return body;
}

request_builder &request_builder::append_body(const char *buffer, size_t size) {
    body.insert(body.end(), buffer, buffer + size);
    return *this;
}

const std::map<std::string, std::string> &request_builder::get_headers() const {
    return headers;
}

std::string &request_builder::get_header(const std::string &key) {
    return headers[key];
}

const std::string &request_builder::get_header(const std::string &key) const {
    return headers.at(key);
}

request_builder &request_builder::set_header(const std::string &key, const std::string &value) {
    this->headers[key] = value;
    return *this;
}

bool request_builder::is_bad_request() const {
    if (get_http_version() != constants::HTTP_VERSION) {
        return true;
    }
    if (constants::VALID_METHODS.find(get_method()) == constants::VALID_METHODS.end()) {
        return true;
    }
    if (get_uri().empty() || get_uri().size() > FILENAME_MAX * 2) {
        return true;
    }
    if (get_path().empty() || get_path().size() > FILENAME_MAX) {
        return true;
    }
    if (get_query_string().size() > FILENAME_MAX) {
        return true;
    }
    return false;
}

void request_builder::reset() {
    *this = request_builder();
}
