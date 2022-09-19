//
// Created by Yahya EZ-ZAINABI on 17/9/2022.
//

#include "request_parser.hpp"

request_parser::request_parser(const std::string &request) {
    std::cout << "-----> request: '" << request << "'" << std::endl;
    std::stringstream s_steam(request);
    std::string request_line;

    std::getline(s_steam, request_line);
    std::stringstream request_line_stream(request_line);
    request_line_stream >> method;
    request_line_stream >> path;
    request_line_stream >> http_version;

    if (method.empty() || path.empty() || http_version.empty()) {
        throw std::invalid_argument("Invalid HTTP request");
    }
    while (std::getline(s_steam, request_line) && !request_line.empty() && request_line != "\r") {
        if (*request_line.rbegin() == '\r') {
            request_line.resize(request_line.size() - 1);
        }
        std::stringstream header_stream(request_line);
        std::string key;
        std::string value;

        std::getline(header_stream, key, ':');
        std::getline(header_stream, value, '\n');
        value.erase(0, 1);
        if (key.empty() || key.size() == request_line.size()) {
            throw std::invalid_argument("Invalid HTTP header");
        }
        headers.insert(std::make_pair(key, value));
    }
    while (std::getline(s_steam, request_line)) {
        body.append(request_line);
        if (s_steam.peek() != EOF) {
            request_line.push_back('\n');
        }
    }
}

const std::string &request_parser::get_method() const {
    return method;
}

const std::string &request_parser::get_path() const {
    return path;
}

const std::string &request_parser::get_http_version() const {
    return http_version;
}

const std::string &request_parser::get_body() const {
    return body;
}

const std::map<std::string, std::string> &request_parser::get_headers() const {
    return headers;
}
