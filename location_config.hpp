//
// Created by Yahya Ez-zainabi on 9/14/22.
//

#ifndef WEBSERV_LOCATION_CONFIG_HPP
#define WEBSERV_LOCATION_CONFIG_HPP

#include <string>
#include <vector>

class location_config {
    std::string error_page;
    std::string client_max_body_size;
    std::vector<std::string> accepted_methods;
    std::string redirect;
    std::string index;
};


#endif //WEBSERV_LOCATION_CONFIG_HPP
