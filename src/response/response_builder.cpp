//
// Created by Yahya Ez-zainabi on 9/17/22.
//

#include "response_builder.hpp"

response_builder::response_builder() : status(200), body_size(0) {
}

int response_builder::get_status() const {
    return status;
}

response_builder &response_builder::set_status(int status) {
    this->status = status;
    return *this;
}

response_builder &response_builder::set_header(const std::string &key, const std::string &value) {
    headers.insert(std::make_pair(key, value));
    return *this;
}

const std::string &response_builder::get_body() const {
    return body;
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
        std::getline(stream, str);
        if (stream.peek() != EOF) {
            str.push_back('\n');
        }
        append_body(str);
    }
    return *this;
}

std::string response_builder::build() const {
    std::stringstream response;
    response << "HTTP/1.1 " << status << " " << constants::STATUS_STR.at(status) << std::endl;
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

response_builder &response_builder::truncate_body(size_t begin, size_t end) {
    if (begin < body.size() && end >= begin) {
        body = body.substr(begin, end - begin + 1);
        body_size = body.size();
    }
    return *this;
}
