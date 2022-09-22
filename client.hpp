//
// Created by Yahya Ez-zainabi on 9/21/22.
//

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cassert>
#include <iostream>
#include <sstream>

#include "request_builder.hpp"

class client {
public:
    explicit client(int fd, sockaddr_in local_addr, sockaddr_in remote_addr);
    client(const client &o);

    ssize_t receive();
    bool request_not_complete() const;
    request_builder get_request();
    in_addr get_local_addr() const;
    in_port_t get_local_port() const;
    in_addr get_remote_addr() const;
    in_port_t get_remote_port() const;
private:
    static const int BUFFER_SIZE = 1024;

    char buffer[BUFFER_SIZE + 1];
    int fd;
    sockaddr_in local_addr;
    sockaddr_in remote_addr;
    request_builder req_builder;
    std::stringstream content;
    bool header_started;
    bool header_completed;
    bool body_completed;

    void process_received_data();
    void reset();
};


#endif //WEBSERV_CLIENT_HPP
