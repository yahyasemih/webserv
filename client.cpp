//
// Created by Yahya Ez-zainabi on 9/21/22.
//

#include "client.hpp"

client::client(int fd, sockaddr_in local_addr, sockaddr_in remote_addr) : buffer(), fd(fd), local_addr(local_addr),
        remote_addr(remote_addr), header_started(false), header_completed(false), body_completed(false) {
}

client::client(const client &o) : buffer(), fd(o.fd), local_addr(o.local_addr), remote_addr(o.remote_addr),
        content(o.content.str()), header_started(o.header_started), header_completed(o.header_completed),
        body_completed(o.body_completed) {
}

ssize_t client::receive() {
    ssize_t res = recv(fd, buffer, BUFFER_SIZE, 0);
    if (res <= 0) {
        return res;
    }
    buffer[res] = '\0';
    content << buffer;
    process_received_data();
    return res;
}

bool client::request_not_complete() const {
    return !header_completed || !body_completed;
}

request_builder client::get_request() {
    reset();
    return req_builder;
}

in_addr client::get_local_addr() const {
    return local_addr.sin_addr;
}

in_port_t client::get_local_port() const {
    return ntohs(local_addr.sin_port);
}

in_addr client::get_remote_addr() const {
    return remote_addr.sin_addr;
}

in_port_t client::get_remote_port() const {
    return ntohs(remote_addr.sin_port);
}

void client::process_received_data() {
    std::string request_line;

    if (!header_started) {
        if (std::getline(content, request_line) && *request_line.rbegin() == '\r') {
            request_line.erase(request_line.size() - 1);
            std::stringstream request_line_stream(request_line);
            std::string method;
            std::string path;
            std::string query_string;
            std::string http_version;

            request_line_stream >> method;
            request_line_stream >> path;
            request_line_stream >> http_version;
            size_t idx = path.find('?');
            if (idx != std::string::npos) {
                query_string = path.c_str() + idx + 1;
                path.erase(idx);
            }
            req_builder.set_method(method)
                    .set_path(path)
                    .set_query_string(query_string)
                    .set_http_version(http_version);
            header_started = true;
        } else {
            content << request_line;
            return;
        }
    }
    while (!header_completed && std::getline(content, request_line) && *request_line.rbegin() == '\r') {
        if (request_line == "\r") {
            header_completed = true;
            break;
        }
        request_line.erase(request_line.size() - 1);
        std::string key;
        std::string value;
        size_t idx = request_line.find(':');
        if (idx == std::string::npos || request_line.size() - idx < 2) {
            continue; // ignoring invalid header
        }
        key = request_line.substr(0, idx);
        value = request_line.substr(idx + 2);
        req_builder.set_header(key, value);
    }
    if (!header_completed) {
        content.clear();
        content.str(request_line);
        return;
    }
    // at this point headers are done, processing body
    // TODO: handle chunked body
    if (req_builder.get_header("Content-Length").empty() && req_builder.get_header("Transfer-Encoding").empty()) {
        body_completed = true;
        return;
    }

    while (std::getline(content, request_line)) {
        req_builder.set_body(request_line);
        if (content.peek() != EOF) {
            req_builder.set_body("\n");
        }
    }
    size_t body_length = std::strtoul(req_builder.get_header("Content-Length").c_str(), NULL, 10);
    if (req_builder.get_body().size() == body_length) {
        body_completed = true;
    }
}

void client::reset() {
    assert(header_completed && body_completed == true);
    content.clear();
    content.str("");
    header_started = false;
    header_completed = false;
    body_completed = false;
}
