//
// Created by Yahya Ez-zainabi on 9/17/22.
//

#ifndef WEBSERV_RESPONSE_BUILDER_HPP
#define WEBSERV_RESPONSE_BUILDER_HPP

#include <sys/types.h>

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "constants.hpp"

class response_builder {
public:
    response_builder();
    response_builder &set_status(in_port_t status);
    response_builder &set_header(const std::string &key, const std::string &value);
    response_builder &set_body(const std::string &body);
    response_builder &append_body(const std::string &body);
    response_builder &append_body(std::istream &stream);
    response_builder &truncate_body(size_t begin, size_t end);
    std::string build() const;
private:
    typedef std::map<std::string, std::string> headers_map;
    in_port_t status;
    headers_map headers;
    std::string body;
    size_t body_size;
};

#endif //WEBSERV_RESPONSE_BUILDER_HPP
