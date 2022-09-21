//
// Created by Yahya Ez-zainabi on 9/21/22.
//

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

class client {
public:
    explicit client(int fd, sockaddr_in local_addr, sockaddr_in remote_addr);
    client(const client &o);

    ssize_t receive();
    bool request_not_complete() const;
    std::string get_request();
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
    std::stringstream content;
    bool request_completed;
};


#endif //WEBSERV_CLIENT_HPP
