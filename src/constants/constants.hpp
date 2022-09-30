//
// Created by Yahya Ez-zainabi on 9/24/22.
//

#ifndef WEBSERV_CONSTANTS_HPP
#define WEBSERV_CONSTANTS_HPP

#include <sys/types.h>

#include <ctime>
#include <map>
#include <set>
#include <string>

class constants {
public:
    typedef const std::map<int, std::string> status_map;
    typedef const std::map<std::string, std::string> mime_types_map;

    static const std::string SERVER_NAME_VERSION;
    static const std::string HTTP_VERSION;
    static const std::string CGI_VERSION;
    static status_map STATUS_STR;
    static mime_types_map MIME_TYPES;
    static const std::set<std::string> VALID_SCOPES;
    static const std::set<std::string> VALID_METHODS;
    static const size_t BUFFER_SIZE = 256 * 1024; // 256 KB buffer
    static const std::time_t CLIENT_TIMEOUT_SEC = 60;
};

#endif //WEBSERV_CONSTANTS_HPP
