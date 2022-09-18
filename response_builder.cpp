//
// Created by Yahya Ez-zainabi on 9/17/22.
//

#include <iostream>
#include "response_builder.hpp"

static std::map<short, std::string> init_map() {
    std::map<short, std::string> map;

    // Informational 1xx
    map.insert(std::make_pair(100, "Continue"));
    map.insert(std::make_pair(101, "Switching Protocols"));
    // Successful 2xx
    map.insert(std::make_pair(200, "OK"));
    map.insert(std::make_pair(201, "Created"));
    map.insert(std::make_pair(202, "Accepted"));
    map.insert(std::make_pair(203, "Non-Authoritative Information"));
    map.insert(std::make_pair(204, "No Content"));
    map.insert(std::make_pair(205, "Reset Content"));
    map.insert(std::make_pair(206, "Partial Content"));
    // Redirection 3xx
    map.insert(std::make_pair(300, "Multiple Choices"));
    map.insert(std::make_pair(301, "Moved Permanently"));
    map.insert(std::make_pair(302, "Found"));
    map.insert(std::make_pair(303, "See Other"));
    map.insert(std::make_pair(304, "Not Modified"));
    map.insert(std::make_pair(305, "Use Proxy"));
    map.insert(std::make_pair(306, "(Unused)")); // Unused
    map.insert(std::make_pair(307, "Temporary Redirect"));
    // Client Error 4xx
    map.insert(std::make_pair(400, "Bad Request"));
    map.insert(std::make_pair(401, "Unauthorized"));
    map.insert(std::make_pair(402, "Payment Required"));
    map.insert(std::make_pair(403, "Forbidden"));
    map.insert(std::make_pair(404, "Not Found"));
    map.insert(std::make_pair(405, "Method Not Allowed"));
    map.insert(std::make_pair(406, "Not Acceptable"));
    map.insert(std::make_pair(407, "Proxy Authentication Required"));
    map.insert(std::make_pair(408, "Request Timeout"));
    map.insert(std::make_pair(409, "Conflict"));
    map.insert(std::make_pair(410, "Gone"));
    map.insert(std::make_pair(411, "Length Required"));
    map.insert(std::make_pair(412, "Precondition Failed"));
    map.insert(std::make_pair(413, "Request Entity Too Large"));
    map.insert(std::make_pair(414, "Request-URI Too Long"));
    map.insert(std::make_pair(415, "Unsupported Media Type"));
    map.insert(std::make_pair(416, "Requested Range Not Satisfiable"));
    map.insert(std::make_pair(417, "Expectation Failed"));
    // Server Error 5xx
    map.insert(std::make_pair(500, "Internal Server Error"));
    map.insert(std::make_pair(501, "Not Implemented"));
    map.insert(std::make_pair(502, "Bad Gateway"));
    map.insert(std::make_pair(503, "Service Unavailable"));
    map.insert(std::make_pair(504, "Gateway Timeout"));
    map.insert(std::make_pair(505, "HTTP Version Not Supported"));

    return map;
}

const response_builder::status_map response_builder::status_str = init_map();

response_builder &response_builder::set_status(short status) {
    this->status = status;
    return *this;
}

response_builder &response_builder::set_header(const std::string &key, const std::string &value) {
    headers.insert(std::make_pair(key, value));
    return *this;
}

response_builder &response_builder::set_body(const std::string &body) {
    this->body = body;
    this->body_size = this->body.size();
    return *this;
}

response_builder &response_builder::append_body(const std::string &body) {
    this->body.append(body);
    this->body_size = this->body.size();
    return *this;
}

response_builder &response_builder::append_body(std::istream &stream) {
    while (stream) {
        std::string str;
        //char buff[1000001];
        //stream.getline(buff, 1000000);
        std::getline(stream, str, '\n');
        if (stream.peek() != EOF) {
            str.push_back('\n');
        }
        append_body(str);
        //append_body(buff);
    }
    return *this;
}

std::string response_builder::build() const {
    std::stringstream response;
    response << "HTTP/1.1 " << status << " " << status_str.at(status) << std::endl;
    for (headers_map ::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        response << it->first << ": " << it->second << std::endl;
    }
    if (headers.find("Content-Length") == headers.end()) {
        response << "Content-Length: " << body_size << std::endl;
    }
    response << std::endl; // End of headers
    response << body;
    return response.str();
}
