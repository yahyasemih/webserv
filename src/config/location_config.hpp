//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#ifndef WEBSERV_LOCATION_CONFIG_HPP
#define WEBSERV_LOCATION_CONFIG_HPP

#include <set>
#include <string>
#include <vector>

class location_config {
public:
    location_config();
    const std::string &get_error_page() const;
    void set_error_page(const std::string &error_page);
    const std::string &get_root() const;
    void set_root(const std::string &root);
    const std::string &get_route() const;
    void set_route(const std::string &route);
    const std::string &get_cgi_path() const;
    void set_cgi_path(const std::string &cgi_path);
    const std::string &get_cgi_extension() const;
    void set_cgi_extension(const std::string &cgi_extension);
    size_t get_client_max_body_size() const;
    void set_client_max_body_size(size_t client_max_body_size);
    const std::set<std::string> &get_accepted_methods() const;
    void set_accepted_methods(const std::set<std::string> &accepted_methods);
    const std::string &get_redirect() const;
    void set_redirect(const std::string &redirect);
    const std::vector<std::string> &get_indexes() const;
    void set_indexes(const std::vector<std::string> &indexes);
    const std::string &get_upload_dir() const;
    void set_upload_dir(const std::string &upload_dir);
    bool is_list_directory() const;
    void set_list_directory(bool list_directory);

    bool is_cgi_route() const;

private:
    std::string root;
    std::string route;
    std::string cgi_path;
    std::string cgi_extension;
    std::string error_page;
    size_t client_max_body_size;
    std::set<std::string> accepted_methods;
    std::string redirect;
    std::vector<std::string> indexes;
    std::string upload_dir;
    bool list_directory;
};


#endif //WEBSERV_LOCATION_CONFIG_HPP
