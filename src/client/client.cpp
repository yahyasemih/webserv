//
// Created by Yahya Ez-zainabi on 9/21/22.
//

#include "client.hpp"

client::client(int fd, sockaddr_in local_addr, sockaddr_in remote_addr) : buffer(), fd(fd), local_addr(local_addr),
        remote_addr(remote_addr), last_request_ts(std::time(NULL)), header_completed(false), body_completed(false),
        is_chunked(false), current_chunk_size(0), left(0), got_size(false) {
}

client::client(const client &o) : buffer(), fd(o.fd), local_addr(o.local_addr), remote_addr(o.remote_addr),
        req_builder(o.req_builder), content(o.content.str()), last_request_ts(o.last_request_ts),
        header_completed(o.header_completed), body_completed(o.body_completed), is_chunked(o.is_chunked),
        current_chunk_size(o.current_chunk_size), left(o.left), got_size(o.got_size) {
}

ssize_t client::receive() {
    bool just_ended = false;
    const size_t buffer_size = header_completed ? constants::BUFFER_SIZE : 1;
    ssize_t res = recv(fd, buffer, buffer_size, 0);
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
            just_ended = true;
        }
    }
    if (!header_completed) {
        return res;
    }
    content.clear();
    content.str("");
    if (just_ended) {
        if (!is_chunked && req_builder.get_header("Content-Length").empty()) {
            body_completed = true;
        }
        if (req_builder.get_header("Content-Length") == "0") {
            body_completed = true;
        }
        return res;
    }
    if (body_completed) {
        return res;
    }

    if (is_chunked) {
        chunk_content += buffer;
        if (!handle_chunk()) {
            return res;
        }
    } else {
        req_builder.append_body(buffer, res);
        size_t body_length = std::strtoul(req_builder.get_header("Content-Length").c_str(), NULL, 10);
        if (req_builder.get_body().size() == body_length) {
            body_completed = true;
        }
    }
    return res;
}

bool client::handle_chunk() {
    if (left > 0) {
        std::string read;
        read = chunk_content.substr(0, left);
        update_left(read.size());
        if (read.size() < chunk_content.size())
            chunk_content = chunk_content.substr(read.size() + 1, chunk_content.size());
        else
            chunk_content.clear();
        if (current_chunk_size == 0 && left == 0) {
            req_builder.append_body(read.c_str(), read.size() - 2);
            body_completed = true;
            return false;
        }
        req_builder.append_body(read.c_str(), read.size());
        if (is_chunk_done()) {
            reset_chunk_values();
        }
    }

    while (!chunk_content.empty()) {
        std::string read;
        if (!got_size)
            get_chunk_size();
        if (!got_size)
            return false;
        if (current_chunk_size == 0 && left == 0) {
            body_completed = true;
            return false;
        }
        if (!this->chunk_content.empty()) {
            read = this->chunk_content.substr(0, this->left);
            update_left(read.size());
            chunk_content = this->chunk_content.substr(read.size(), chunk_content.size());
            req_builder.append_body(read.c_str(), read.size());
            if (is_chunk_done())
                reset_chunk_values();
        }
    }

    return true;
}

bool client::is_chunk_done() const {
    return this->left == 0;
}

void client::reset_chunk_values() {
    this->chunk_content.clear();
    this->current_chunk_size = 0;
    this->left = 0;
    this->got_size = false;
}

void client::update_left(size_t bytes_read) {
    if (this->left >= bytes_read) {
        this->left -= bytes_read;
    } else {
        this->left = 0;
    }
}

void client::get_chunk_size() {
    if (!this->chunk_content.empty()) {
        std::string hex_size;
        std::string to_find = "\r\n";
        size_t idx = chunk_content.find(to_find);
        if (idx == std::string::npos) {
            return;
        }
        this->got_size = true;
        hex_size = chunk_content.substr(0, idx);
        this->chunk_content = chunk_content.substr(idx + to_find.size(), chunk_content.size());
        this->current_chunk_size = std::strtoul(hex_size.c_str(), NULL, 16);
        this->left = this->current_chunk_size + 2;
    }
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

std::string url_decode(const std::string &url) {
    std::string decoded_url;

    for (size_t i = 0; i < url.size(); i++) {
        if (url[i] == '%' && url.size() - i > 2 && utilities::is_valid_hex(url[i + 1], url[i + 2])) {
            decoded_url += utilities::hex_to_char(url[i + 1], url[i + 2]);
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
    is_chunked = false;
    reset_chunk_values();
}

void client::process_request_line() {
    std::string request_line;

    std::getline(content, request_line);
    if (*request_line.rbegin() == '\r') {
        request_line.erase(request_line.size() - 1);
    }
    if (request_line.empty()) {
        header_completed = true;
        body_completed = true;
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

    if (req_builder.is_bad_request()) {
        header_completed = true;
        body_completed = true;
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
        if (!is_chunked && key == "Transfer-Encoding" && value == "chunked") {
            is_chunked = true;
        }
        req_builder.set_header(key, value);
    }
}

std::time_t client::get_last_request_ts() const {
    return last_request_ts;
}

void client::reset_last_request_ts() {
    last_request_ts = std::time(NULL);
}
