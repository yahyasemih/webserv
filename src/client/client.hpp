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
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "constants.hpp"
#include "request_builder.hpp"
#include "response_builder.hpp"
#include "utilities.hpp"

class client {
public:
    explicit client(int fd, sockaddr_in local_addr, sockaddr_in remote_addr);
    client(const client &o);

    ssize_t receive();
    bool request_not_complete() const;
    request_builder get_request();
    std::string &get_response();
    in_addr get_local_addr() const;
    in_port_t get_local_port() const;
    in_addr get_remote_addr() const;
    in_port_t get_remote_port() const;
    std::time_t get_last_request_ts() const;
    void reset_last_request_ts();

private:
    char buffer[constants::BUFFER_SIZE + 1];
    int fd;
    sockaddr_in local_addr;
    sockaddr_in remote_addr;
    request_builder req_builder;
    std::string response;
    std::stringstream content;
    std::time_t last_request_ts;
    bool header_completed;
    bool body_completed;
    bool is_chunked;

    std::string chunk_content;
    size_t current_chunk_size;
    size_t left;
    bool got_size;

    void reset_chunk_values();
    bool is_chunk_done() const;
    void update_left(size_t bytes_read);
    void get_chunk_size();
    bool handle_chunk();
    void process_request_line();
    void process_header_lines();
    void reset();
};


#endif //WEBSERV_CLIENT_HPP
