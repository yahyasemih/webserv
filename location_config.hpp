//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#ifndef WEBSERV_LOCATION_CONFIG_HPP
#define WEBSERV_LOCATION_CONFIG_HPP

#include <string>
#include <vector>

class location_config {
public:
    const std::string &get_error_page() const;
    void set_error_page(const std::string &error_page);
    const std::string &get_root() const;
    void set_root(const std::string &root);
    const std::string &get_route() const;
    void set_route(const std::string &route);
    const std::string &get_client_max_body_size() const;
    void set_client_max_body_size(const std::string &client_max_body_size);
    const std::vector<std::string> &get_accepted_methods() const;
    void set_accepted_methods(const std::vector<std::string> &accepted_methods);
    const std::string &get_redirect() const;
    void set_redirect(const std::string &redirect);
    const std::string &get_index() const;
    void set_index(const std::string &index);

private:
    std::string root;
    std::string route;
    std::string error_page;
    std::string client_max_body_size;
    std::vector<std::string> accepted_methods;
    std::string redirect;
    std::string index;
};


#endif //WEBSERV_LOCATION_CONFIG_HPP
