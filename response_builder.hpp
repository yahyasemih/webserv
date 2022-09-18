//
// Created by Yahya Ez-zainabi on 9/17/22.
//

#ifndef WEBSERV_RESPONSE_BUILDER_HPP
#define WEBSERV_RESPONSE_BUILDER_HPP

#include <map>
#include <sstream>
#include <string>

class response_builder {
public:
    response_builder &set_status(short status);
    response_builder &set_header(const std::string &key, const std::string &value);
    response_builder &set_body(const std::string &body);
    response_builder &append_body(const std::string &body);
    response_builder &append_body(std::istream &stream);
    std::string build() const;
private:
    typedef std::map<std::string, std::string> headers_map;
    typedef const std::map<short, std::string> status_map;
    short status;
    headers_map headers;
    std::string body;
    size_t body_size;

    static status_map status_str;
};


#endif //WEBSERV_RESPONSE_BUILDER_HPP
