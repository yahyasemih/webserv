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

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "client.hpp"
#include "config.hpp"
#include "config_parser.hpp"
#include "constants.hpp"
#include "directory_index_builder.hpp"
#include "logger.hpp"
#include "response_builder.hpp"
#include "utilities.hpp"

class server {
public:
    explicit server(const std::string &config_file);
    ~server();

    void start();
    void stop();
private:
    config conf;
    std::map<int, client> clients;
    std::vector<pollfd> poll_fds;
    bool is_running;

    void create_sockets();
    void accept_connection(pollfd &pf);
    void handle_request(pollfd &pf);
    void serve_response(pollfd &pf);
    void clean_fds();
    const server_config &get_matching_server(const std::string &ip, const std::string &hostname, in_port_t port);
    static const location_config &get_matching_location(const request_builder &req_builder,
            const server_config &server_conf);
    static void process_request(request_builder &req_builder, response_builder &res_builder, std::string &file,
            const server_config &server_conf, const location_config &location_conf, const client &c);
    static void run_static(request_builder &req_builder, response_builder &res_builder, const std::string &file,
            const location_config &location_conf);
    static void run_cgi(request_builder &req_builder, response_builder &res_builder, const std::string &file,
            const server_config &server_conf, const location_config &location_conf, const client &c);
    static void handle_put_delete(request_builder &req_builder, response_builder &res_builder, std::string &file,
            const location_config &location_conf);
};

#endif //WEBSERV_SERVER_HPP
