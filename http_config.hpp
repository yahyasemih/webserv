//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#ifndef WEBSERV_HTTP_CONFIG_HPP
#define WEBSERV_HTTP_CONFIG_HPP


#include "server_config.hpp"

class http_config {
public:
    explicit http_config(const std::string &conf);
    const std::vector<server_config> &get_server_configs() const;
    void set_server_configs(const std::vector<server_config> &server_configs);
    const std::string &get_root() const;
    void set_root(const std::string &root);
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
    std::vector<server_config> server_configs; // could be a std::map "host:port" -> server_config
    std::string root;
    std::string error_page;
    std::string access_log;
    std::string error_log;
    std::string client_max_body_size;
    std::string index;
};

#endif //WEBSERV_HTTP_CONFIG_HPP
