//
// Created by Yahya Ez-zainabi on 9/18/22.
//

#ifndef WEBSERV_CONFIG_PARSER_HPP
#define WEBSERV_CONFIG_PARSER_HPP

#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <stack>
#include <sstream>
#include <string>

#include "config.hpp"
#include "constants.hpp"
#include "utilities.hpp"

class config_parser {
public:
    typedef std::vector<std::string> arg_list_t;
    typedef std::vector<arg_list_t> instruction_list_t;
    typedef std::vector<instruction_list_t> scope_list_t;
    typedef std::map<std::string, scope_list_t> scope_map_t;

    explicit config_parser(const std::string &config_file);
    void compile();
    const std::string &get_error_message() const;
    size_t get_error_line() const;
    bool is_valid() const;
    config &get_result();

private:
    bool parse_config();
    bool parse_http_config(size_t line_nbr);
    bool parse_server_config(size_t line_nbr);
    bool parse_location_config(size_t line_nbr);
    void propagate();
    std::string get_absolute_path(const std::string &path) const;

    config conf;
    scope_map_t scope_to_instructions;
    std::string config_file;
    std::string error_message;
    size_t error_line;
};

#endif //WEBSERV_CONFIG_PARSER_HPP
