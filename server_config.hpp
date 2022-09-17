//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#ifndef WEBSERV_SERVER_CONFIG_HPP
#define WEBSERV_SERVER_CONFIG_HPP

#include <string>
#include <vector>

#include "location_config.hpp"

class server_config {
public:
    const std::string &get_host() const;
    void set_host(const std::string &host);
    short get_port() const;
    void set_port(short port);
    const std::vector<std::string> &get_server_names() const;
    void set_server_names(const std::vector<std::string> &server_names);
    const std::vector<location_config> &get_location_configs() const;
    void set_location_configs(const std::vector<location_config> &location_configs);
    const std::string &get_error_page() const;
    void set_error_page(const std::string &error_page);
    const std::string &get_access_log() const;
    void set_access_log(const std::string &access_log);
    const std::string &get_error_log() const;
    void set_error_log(const std::string &error_log);
    const std::string &get_client_max_body_size() const;
    void set_client_max_body_size(const std::string &client_max_body_size);
    const std::string &get_index() const;
    void set_index(const std::string &index);
private:
    std::string host;
    short port;
    std::vector<std::string> server_names;
    std::vector<location_config> location_configs;
    std::string error_page;
    std::string access_log;
    std::string error_log;
    std::string client_max_body_size;
    std::string index;
};

#endif //WEBSERV_SERVER_CONFIG_HPP
