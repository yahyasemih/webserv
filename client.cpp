//
// Created by Yahya Ez-zainabi on 9/21/22.
//

#include "client.hpp"

client::client(int fd, sockaddr_in local_addr, sockaddr_in remote_addr) : buffer(), fd(fd), local_addr(local_addr),
        remote_addr(remote_addr), request_completed(false) {
}

client::client(const client &o) : buffer(), fd(o.fd), local_addr(o.local_addr), remote_addr(o.remote_addr),
        content(o.content.str()), request_completed(o.request_completed) {
}

ssize_t client::receive() {
    ssize_t res = recv(fd, buffer, BUFFER_SIZE, 0);
    struct stat s = {};
    fstat(fd, &s);
    request_completed = s.st_size == 0;
    if (res <= 0) {
        return res;
    }
    buffer[res] = '\0';
    content << buffer;
    return res;
}

bool client::request_not_complete() const {
    return !request_completed;
}

std::string client::get_request() {
    std::string request = content.str();
    content.str("");
    return request;
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
