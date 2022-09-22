//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <sys/file.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#include "client.hpp"
#include "config.hpp"
#include "config_parser.hpp"
#include "response_builder.hpp"

class server {
public:
    explicit server(const std::string &config_file);
    ~server();

    void start();
    void stop();
private:
    static const std::map<std::string, std::string> mime_type_map;
    config conf;
    std::vector<int> socket_fds;
    std::map<int, client> clients;
    std::vector<pollfd> poll_fds;
    bool is_running;

    void accept_connection(size_t index);
    void handle_request(pollfd &pf);
    void clean_fds();
    const server_config &get_matching_server(const std::string &ip, const std::string &host, in_port_t port);

    static std::string get_mime_type(const std::string &file);
};

#endif //WEBSERV_SERVER_HPP
