//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#ifndef WEBSERV_SERVER_CONFIG_HPP
#define WEBSERV_SERVER_CONFIG_HPP

#include <sys/types.h>

#include <set>
#include <string>
#include <vector>

#include "location_config.hpp"
#include "address_port.hpp"

class server_config {
public:
    server_config();

    void add_address(const std::string &ip, in_port_t port);
    const std::set<address_port> &get_addresses() const;
    const std::string &get_root() const;
    void set_root(const std::string &root);
    const std::set<std::string> &get_server_names() const;
    void set_server_names(const std::set<std::string> &server_names);
    std::vector<location_config> &get_location_configs();
    const std::vector<location_config> &get_location_configs() const;
    void set_location_configs(const std::vector<location_config> &location_configs);
    const std::string &get_error_page() const;
    void set_error_page(const std::string &error_page);
    const std::string &get_access_log() const;
    void set_access_log(const std::string &access_log);
    const std::string &get_error_log() const;
    void set_error_log(const std::string &error_log);
    size_t get_client_max_body_size() const;
    void set_client_max_body_size(size_t client_max_body_size);
    const std::vector<std::string> &get_indexes() const;
    void set_indexes(const std::vector<std::string> &indexes);
    const std::string &get_cgi_path() const;
    void set_cgi_path(const std::string &cgi_path);
    const std::string &get_cgi_extension() const;
    void set_cgi_extension(const std::string &cgi_extension);
    bool is_cgi_route() const;

private:
    std::set<address_port> addresses;
    std::string root;
    std::set<std::string> server_names;
    std::vector<location_config> location_configs;
    std::string error_page;
    std::string access_log;
    std::string error_log;
    size_t client_max_body_size;
    std::vector<std::string> indexes;
    std::string cgi_path;
    std::string cgi_extension;
};

#endif //WEBSERV_SERVER_CONFIG_HPP
