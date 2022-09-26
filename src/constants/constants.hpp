//
// Created by Yahya Ez-zainabi on 9/24/22.
//

#ifndef WEBSERV_CONSTANTS_HPP
#define WEBSERV_CONSTANTS_HPP

#include <sys/types.h>

#include <map>
#include <set>
#include <string>

class constants {
public:
    typedef const std::map<in_port_t, std::string> status_map;
    typedef const std::map<std::string, std::string> mime_types_map;

    static const std::string SERVER_NAME;
    static const std::string HTTP_VERSION;
    static status_map STATUS_STR;
    static mime_types_map MIME_TYPES;
    static const std::set<std::string> VALID_SCOPES;
    static const size_t BUFFER_SIZE = 256 * 1024; // 256 KB buffer
};

#endif //WEBSERV_CONSTANTS_HPP
