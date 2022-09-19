//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <netdb.h>
#include <poll.h>
#include <string>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

#include "config.hpp"
#include "config_parser.hpp"
#include "request_parser.hpp"
#include "response_builder.hpp"

class server {
public:
    explicit server(const std::string &config_file);
    ~server();

    void start();
    void stop();
private:
    static const int BUFFER_SIZE;

    config conf;
    std::vector<int> socket_fds;
    std::vector<pollfd> poll_fds;
    bool is_running;

    void accept_connection(size_t index);
    const server_config &get_matching_server(const std::string &host, short port);
};

#endif //WEBSERV_SERVER_HPP
