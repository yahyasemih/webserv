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
#include "response_builder.hpp"
#include "directory_listing_page_builder.hpp"

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

    void accept_connection(pollfd &pf);
    void handle_request(pollfd &pf);
    void clean_fds();
    const server_config &get_matching_server(const std::string &ip, const std::string &host, in_port_t port);
    static const location_config &get_matching_location(const request_builder &req_builder,
            const server_config &server_conf);
    static void process_request(request_builder &req_builder, response_builder &res_builder, std::string &file,
            const server_config &server_conf, const location_config &location_conf);
    static void run_static(request_builder &req_builder, response_builder &res_builder, const std::string &file,
            const location_config &location_conf);
    static void run_cgi(request_builder &req_builder, response_builder &res_builder, const std::string &file,
            const server_config &server_conf, const location_config &location_conf);
    static std::string get_mime_type(const std::string &file);
    static std::string get_file_extension(const std::string &file);
    static std::string get_file_basename(const std::string &file);
    static std::string get_valid_path(const std::string &root, std::string path);
    static std::string create_error_page(int status);
    static void on_error(int status, const location_config &location_conf, response_builder &res_builder);
    static void truncate_body(request_builder &req_builder, response_builder &res_builder, std::ifstream &f);
    static void list_directory(std::string &path, response_builder &res_builder, const std::string &root);
    void serve_response(pollfd &pf);
    static std::string get_file_readable_size(off_t size);
    static std::string get_file_last_modified_date(struct timespec &ts);
};

#endif //WEBSERV_SERVER_HPP
