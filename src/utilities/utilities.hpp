//
// Created by Yahya Ez-zainabi on 9/29/22.
//

#ifndef WEBSERV_UTILITIES_HPP
#define WEBSERV_UTILITIES_HPP

#include <arpa/inet.h>

#include <algorithm>
#include <fstream>
#include <string>

#include "client.hpp"
#include "constants.hpp"
#include "location_config.hpp"
#include "request_builder.hpp"
#include "response_builder.hpp"

class utilities {
public:
    static std::string get_mime_type(const std::string &file);
    static std::string get_file_extension(const std::string &file);
    static std::string get_file_basename(const std::string &file);
    static std::string get_valid_path(const std::string &root, std::string path);
    static std::string create_error_page(int status);
    static void on_error(int status, const location_config &location_conf, response_builder &res_builder);
    static void truncate_body(request_builder &req_builder, response_builder &res_builder, std::ifstream &f);
    static void set_environment_variables(request_builder &req_builder, const std::string &file,
            const location_config &location_conf, const client &c);
    static bool is_valid_client_max_body_size(const std::string &client_max_body_size_str);
    static size_t translate_client_max_body_size(const std::string &client_max_body_size_str);
};

#endif //WEBSERV_UTILITIES_HPP
