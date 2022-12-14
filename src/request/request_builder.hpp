//
// Created by Yahya Ez-zainabi on 9/21/22.
//

#ifndef WEBSERV_REQUEST_BUILDER_HPP
#define WEBSERV_REQUEST_BUILDER_HPP

#include <map>
#include <string>
#include <vector>

#include "constants.hpp"

class request_builder {
public:
    const std::string &get_method() const;
    request_builder &set_method(const std::string &method);
    const std::string &get_path() const;
    request_builder &set_path(const std::string &path);
    const std::string &get_uri() const;
    request_builder &set_uri(const std::string &uri);
    const std::string &get_query_string() const;
    request_builder &set_query_string(const std::string &query_string);
    const std::string &get_http_version() const;
    request_builder &set_http_version(const std::string &http_version);
    std::vector<char> &get_body();
    request_builder &append_body(const char *buffer, size_t size);
    std::string &get_header(const std::string &key);
    const std::string &get_header(const std::string &key) const;
    const std::map<std::string, std::string> &get_headers() const;
    request_builder &set_header(const std::string &key, const std::string &value);

    bool is_bad_request() const;
    void reset();
private:
    std::string method;
    std::string uri;
    std::string path;
    std::string query_string;
    std::string http_version;
    std::vector<char> body;
    std::map<std::string, std::string> headers;
};

#endif //WEBSERV_REQUEST_BUILDER_HPP
