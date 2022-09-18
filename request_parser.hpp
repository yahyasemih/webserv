//
// Created by Yahya EZ-ZAINABI on 17/9/2022.
//

#ifndef WEBSERV_REQUEST_PARSER_HPP
#define WEBSERV_REQUEST_PARSER_HPP

#include <string>
#include <sstream>
#include <unordered_map>

class request_parser {
public:
    explicit request_parser(const std::string &request);
    const std::string &get_method() const;
    const std::string &get_path() const;
    const std::string &get_http_version() const;
    const std::string &get_body() const;
    const std::unordered_map<std::string, std::string> &get_headers() const;
private:
    std::string method;
    std::string path;
    std::string http_version;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
};


#endif //WEBSERV_REQUEST_PARSER_HPP
