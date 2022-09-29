//
// Created by Yahya Ez-zainabi on 9/28/22.
//

#ifndef WEBSERV_LOGGER_HPP
#define WEBSERV_LOGGER_HPP

#include <arpa/inet.h>

#include <fstream>
#include <string>

#include "client.hpp"
#include "request_builder.hpp"
#include "response_builder.hpp"

class logger {
public:
    explicit logger(const std::string &filename);

    void access(const request_builder &req_builder, const response_builder &res_builder, const client &c);
    void error(const std::string &error_message);

private:
    std::ofstream log_file;
};


#endif //WEBSERV_LOGGER_HPP
