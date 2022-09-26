//
// Created by Yahya Ez-zainabi on 9/21/22.
//

#include "client.hpp"

client::client(int fd, sockaddr_in local_addr, sockaddr_in remote_addr) : buffer(), fd(fd), local_addr(local_addr),
        remote_addr(remote_addr), header_completed(false), body_completed(false) {
}

client::client(const client &o) : buffer(), fd(o.fd), local_addr(o.local_addr), remote_addr(o.remote_addr),
        req_builder(o.req_builder), content(o.content.str()), header_completed(o.header_completed),
        body_completed(o.body_completed) {
}

ssize_t client::receive() {
    ssize_t res = recv(fd, buffer, constants::BUFFER_SIZE, 0);
    if (res <= 0) {
        return res;
    }
    buffer[res] = '\0';
    if (!header_completed) {
        content << buffer;
        size_t idx = content.str().find("\r\n\r\n");
        if (idx != std::string::npos) {
            process_request_line();
            process_header_lines();
            idx += 4;
            ssize_t body_start_idx = static_cast<ssize_t>(idx) % constants::BUFFER_SIZE;
            if (body_start_idx <= res) {
                req_builder.append_body(buffer + body_start_idx, res - body_start_idx);
            } else {
                req_builder.append_body(content.str().c_str() + idx, content.str().size());
                content.clear();
                content.str("");
            }
            // TODO: handle chunked body
            if (req_builder.get_header("Content-Length").empty()
                    && req_builder.get_header("Transfer-Encoding").empty()) {
                body_completed = true;
                return res;
            }
        }
    } else {
        req_builder.append_body(buffer, res);
    }
    size_t body_length = std::strtoul(req_builder.get_header("Content-Length").c_str(), NULL, 10);
    if (req_builder.get_body().size() == body_length) {
        body_completed = true;
    }
    return res;
}

bool client::request_not_complete() const {
    return !header_completed || !body_completed;
}

request_builder client::get_request() {
    reset();
    request_builder res = req_builder;
    req_builder.reset();
    return res;
}

std::string &client::get_response() {
    return response;
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

static char hex_to_char(char c1, char c2) {
    char c;

    if (isdigit(c1)) {
        c = static_cast<char>(((c1 - '0') * 16));
    } else {
        c = static_cast<char>(((tolower(c1) - 'a') * 16));
    }
    if (isdigit(c2)) {
        c = static_cast<char>((c + (c2 - '0')));
    } else {
        c = static_cast<char>((c + (tolower(c2) - 'a')));
    }
    return c;
}

static bool is_valid_hex(char c1, char c2) {
    if (!isalnum(c1) || tolower(c1) > 'f') {
        return false;
    }
    if (!isalnum(c2) || tolower(c2) > 'f') {
        return false;
    }
    return true;
}

std::string url_decode(const std::string &url) {
    std::string decoded_url;

    for (size_t i = 0; i < url.size(); i++) {
        if (url[i] == '%' && url.size() - i > 2 && is_valid_hex(url[i + 1], url[i + 2])) {
            decoded_url += hex_to_char(url[i + 1], url[i + 2]);
            i += 2;
        } else if (url[i] == '+') {
            decoded_url += ' ';
        } else {
            decoded_url += url[i];
        }
    }
    return decoded_url;
}

void client::reset() {
    assert(header_completed && body_completed);
    content.clear();
    content.str("");
    header_completed = false;
    body_completed = false;
}

void client::process_request_line() {
    std::string request_line;

    std::getline(content, request_line);
    if (*request_line.rbegin() == '\r') {
        request_line.erase(request_line.size() - 1);
    }
    if (request_line.empty()) {
        // TODO: Bad Request
        return;
    }

    std::stringstream request_line_stream(request_line);
    std::string method;
    std::string path;
    std::string query_string;
    std::string http_version;

    request_line_stream >> method;
    request_line_stream >> path;
    request_line_stream >> http_version;
    req_builder.set_uri(path);
    size_t idx = path.find('?');
    if (idx != std::string::npos) {
        query_string = path.c_str() + idx + 1;
        path.erase(idx);
    }
    req_builder.set_method(method)
            .set_path(url_decode(path))
            .set_query_string(query_string)
            .set_http_version(http_version);
    if (req_builder.get_http_version() != constants::HTTP_VERSION) {
        // TODO: Bad Request
        return;
    }
}

void client::process_header_lines() {
    std::string header_line;
    std::string key;
    std::string value;
    size_t idx;

    while (std::getline(content, header_line) && *header_line.rbegin() == '\r') {
        if (header_line == "\r") {
            header_completed = true;
            break;
        }
        header_line.erase(header_line.size() - 1);
        idx = header_line.find(':');
        if (idx == std::string::npos || header_line.size() - idx < 2) {
            continue; // ignoring invalid header
        }
        key = header_line.substr(0, idx);
        value = header_line.substr(idx + 2);
        req_builder.set_header(key, value);
    }
    content.clear();
}
