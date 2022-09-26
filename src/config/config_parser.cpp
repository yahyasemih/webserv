//
// Created by Yahya Ez-zainabi on 9/18/22.
//

#include "config_parser.hpp"

config_parser::config_parser(const std::string &config_file) : config_file(config_file), error_message(),
        error_line(std::numeric_limits<size_t>::max()) {
}

static bool is_valid_client_max_body_size(const std::string &client_max_body_size_str) {
    for (size_t i = 0; i < client_max_body_size_str.size() - 1; ++i) {
        if (!isdigit(client_max_body_size_str[i])) {
            return false;
        }
    }
    std::string valid_units("bkmgBKMG");
    return valid_units.find(*client_max_body_size_str.rbegin()) != std::string::npos;
}

static size_t translate_client_max_body_size(const std::string &client_max_body_size_str) {
    std::map<char, size_t> multiplier;
    multiplier.insert(std::make_pair('b', 1));
    multiplier.insert(std::make_pair('B', 1));
    multiplier.insert(std::make_pair('k', 1024));
    multiplier.insert(std::make_pair('K', 1024));
    multiplier.insert(std::make_pair('m', 1024 * 1024));
    multiplier.insert(std::make_pair('M', 1024 * 1024));
    multiplier.insert(std::make_pair('g', 1024 * 1024 * 1024));
    multiplier.insert(std::make_pair('G', 1024 * 1024 * 1024));

    size_t size = strtoull(client_max_body_size_str.c_str(), NULL, 10);
    return size * multiplier[*client_max_body_size_str.rbegin()];
}

std::string config_parser::get_absolute_path(const std::string &path) const {
    // TODO: Optimize code and usage of this one
    if (path[0] == '/') {
        return path;
    }
    std::string parent_dir;
    if (config_file[0] == '/') {
        parent_dir = config_file.substr(0, config_file.find_last_of('/')) + "/";
    } else {
        char *cwd = getcwd(NULL, 0);
        if (cwd != NULL) {
            parent_dir = cwd;
            free(cwd);
        }
        parent_dir += "/" + config_file;
        parent_dir.erase(parent_dir.find_last_of('/'));
    }
    return parent_dir + "/" + path;
}

bool config_parser::parse_config() {
    std::ifstream file(config_file.c_str());
    if (!file.is_open()) {
        error_message = "Invalid config file";
        error_line = 0;
        return false;
    }
    std::string line;
    size_t line_nbr = 0;
    std::stack<std::string> scope;
    scope.push("main");
    scope_to_instructions.insert(std::make_pair(scope.top(),scope_list_t(1)));

    while (std::getline(file, line)) {
        ++line_nbr;
        size_t idx = line.find('#');
        if (idx != std::string::npos) {
            line = line.substr(0, idx);
        }
        if (line.empty()) {
            continue;
        }
        size_t start = line.find_first_not_of("\r\t\f\v ");
        if (start == std::string::npos) {
            continue;
        }
        line = line.substr(start);
        size_t end = line.find_last_not_of("\r\t\f\v ");
        if (end == std::string::npos) {
            continue;
        }
        line.erase(end + 1);
        if (line.empty()) {
            continue;
        }
        if (*line.rbegin() == ';') { // instruction
            line.erase(line.size() - 1);
            std::vector<std::string> args;
            std::string arg;
            std::stringstream line_stream(line);
            while (line_stream >> arg) {
                args.push_back(arg);
            }
            scope_to_instructions.at(scope.top()).rbegin()->push_back(args);
        } else if (*line.rbegin() == '{') { // new scope
            std::string new_scope;
            std::stringstream scope_stream(line.erase(line.size() - 1));
            scope_stream >> new_scope;
            if (constants::VALID_SCOPES.find(new_scope) == constants::VALID_SCOPES.end()) {
                error_message = "Invalid scope '" + new_scope + "'";
                error_line = line_nbr;
                return false;
            }
            if (new_scope == "server") {
                if (scope.top() != "http") {
                    error_message = "Invalid 'server' config inside '" + scope.top() + "' scope";
                    error_line = line_nbr;
                    return false;
                }
            } else if (new_scope == "location") {
                if (scope.top() != "server") {
                    error_message = "Invalid 'location' config inside '" + scope.top() + "' scope";
                    error_line = line_nbr;
                    return false;
                }
            } else if (new_scope == "http") {
                if (scope.top() != "main") {
                    error_message = "Invalid 'http' config inside '" + scope.top() + "' scope";
                    error_line = line_nbr;
                    return false;
                }
            }
            scope.push(new_scope);
            if (scope_to_instructions.find(scope.top()) == scope_to_instructions.end()) {
                scope_to_instructions.insert(std::make_pair(scope.top(), scope_list_t()));
            }
            scope_to_instructions.at(scope.top()).push_back(instruction_list_t());
            if (new_scope == "server") {
                conf.get_http_conf().get_server_configs().push_back(server_config());
            } else if (new_scope == "location") {
                conf.get_http_conf().get_server_configs().rbegin()->get_location_configs().push_back(location_config());
            }
        } else if (line == "}") { //  end of scope
            if (scope.size() <= 1) {
                error_message = "Invalid end of scope";
                error_line = line_nbr;
                return false;
            }
            if (scope.top() == "http") {
                if (scope_to_instructions.at("http").size() > 1) {
                    error_message = "Multiple 'http' sections";
                    error_line = line_nbr;
                    return false;
                }
                http_config &http_conf = conf.get_http_conf();
                const instruction_list_t &http_instruction_list = scope_to_instructions.at("http").at(0);

                for (size_t i = 0 ; i < http_instruction_list.size(); ++i) {
                    if (http_instruction_list.at(i).at(0) == "root") {
                        if (http_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'root'";
                            error_line = line_nbr;
                            return false;
                        }
                        http_conf.set_root(http_instruction_list.at(i).at(1));
                    } else if (http_instruction_list.at(i).at(0) == "error_page") {
                        if (http_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'error_page'";
                            error_line = line_nbr;
                            return false;
                        }
                        http_conf.set_error_page(http_instruction_list.at(i).at(1));
                    } else if (http_instruction_list.at(i).at(0) == "error_log") {
                        if (http_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'error_log'";
                            error_line = line_nbr;
                            return false;
                        }
                        http_conf.set_error_log(http_instruction_list.at(i).at(1));
                    } else if (http_instruction_list.at(i).at(0) == "access_log") {
                        if (http_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'access_log'";
                            error_line = line_nbr;
                            return false;
                        }
                        http_conf.set_access_log(http_instruction_list.at(i).at(1));
                    } else if (http_instruction_list.at(i).at(0) == "client_max_body_size") {
                        if (http_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'client_max_body_size'";
                            error_line = line_nbr;
                            return false;
                        }
                        const std::string &arg = http_instruction_list.at(i).at(1);
                        if (!is_valid_client_max_body_size(arg)) {
                            error_message = "Invalid argument for 'client_max_body_size' : " + arg;
                            error_line = line_nbr;
                            return false;
                        }
                        size_t size = translate_client_max_body_size(arg);
                        http_conf.set_client_max_body_size(size);
                    } else if (http_instruction_list.at(i).at(0) == "index") {
                        std::vector<std::string> indexes(http_instruction_list.at(i).begin() + 1,
                                                         http_instruction_list.at(i).end());
                        http_conf.set_indexes(indexes);
                    } else {
                        error_message = "Invalid field '" + http_instruction_list.at(i).at(0) + "'";
                        error_line = line_nbr;
                        return false;
                    }
                }
            } else if (scope.top() == "server") {
                server_config &server_conf = *conf.get_http_conf().get_server_configs().rbegin();
                const instruction_list_t &server_instruction_list = *scope_to_instructions.at("server").rbegin();

                for (size_t i = 0 ; i < server_instruction_list.size(); ++i) {
                    if (server_instruction_list.at(i).at(0) == "listen") {
                        if (server_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'listen'";
                            error_line = line_nbr;
                            return false;
                        }
                        const std::string &host_port = server_instruction_list.at(i).at(1);
                        std::string host;
                        in_port_t port;
                        size_t idx = host_port.find(':');
                        if (idx != std::string::npos) {
                            host = host_port.substr(0, idx);
                            std::stringstream host_stream(host_port.substr(idx + 1));
                            host_stream >> port;
                        } else {
                            host = host_port;
                            port = 80;
                        }

                        server_conf.set_host(host);
                        server_conf.set_port(port);
                    } else if (server_instruction_list.at(i).at(0) == "root") {
                        if (server_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'root'";
                            error_line = line_nbr;
                            return false;
                        }
                        server_conf.set_root(server_instruction_list.at(i).at(1));
                    } else if (server_instruction_list.at(i).at(0) == "server_names") {
                        server_conf.set_server_names(std::set<std::string>(
                                server_instruction_list.at(i).begin() + 1,
                                server_instruction_list.at(i).end()));
                    } else if (server_instruction_list.at(i).at(0) == "error_page") {
                        if (server_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'error_page'";
                            error_line = line_nbr;
                            return false;
                        }
                        server_conf.set_error_page(server_instruction_list.at(i).at(1));
                    } else if (server_instruction_list.at(i).at(0) == "access_log") {
                        if (server_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'access_log'";
                            error_line = line_nbr;
                            return false;
                        }
                        server_conf.set_access_log(server_instruction_list.at(i).at(1));
                    } else if (server_instruction_list.at(i).at(0) == "error_log") {
                        if (server_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'error_log'";
                            error_line = line_nbr;
                            return false;
                        }
                        server_conf.set_error_log(server_instruction_list.at(i).at(1));
                    } else if (server_instruction_list.at(i).at(0) == "client_max_body_size") {
                        if (server_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'client_max_body_size'";
                            error_line = line_nbr;
                            return false;
                        }
                        const std::string &arg = server_instruction_list.at(i).at(1);
                        if (!is_valid_client_max_body_size(arg)) {
                            error_message = "Invalid argument for 'client_max_body_size' : " + arg;
                            error_line = line_nbr;
                            return false;
                        }
                        size_t size = translate_client_max_body_size(arg);
                        server_conf.set_client_max_body_size(size);
                    } else if (server_instruction_list.at(i).at(0) == "index") {
                        std::vector<std::string> indexes(server_instruction_list.at(i).begin() + 1,
                                                         server_instruction_list.at(i).end());
                        server_conf.set_indexes(indexes);
                    } else if (server_instruction_list.at(i).at(0) == "cgi_path") {
                        if (server_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'cgi_path'";
                            error_line = line_nbr;
                            return false;
                        }
                        server_conf.set_cgi_path(server_instruction_list.at(i).at(1));
                    } else if (server_instruction_list.at(i).at(0) == "cgi_extension") {
                        if (server_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'cgi_extension'";
                            error_line = line_nbr;
                            return false;
                        }
                        server_conf.set_cgi_extension(server_instruction_list.at(i).at(1));
                    } else {
                        error_message = "Invalid field '" + server_instruction_list.at(i).at(0) + "'";
                        error_line = line_nbr;
                        return false;
                    }
                }
            } else if (scope.top() == "location") {
                location_config &location_conf = *conf.get_http_conf().get_server_configs().rbegin()->get_location_configs().rbegin();
                const instruction_list_t &location_instruction_list = *scope_to_instructions.at("location").rbegin();

                for (size_t i = 0; i < location_instruction_list.size(); ++i) {
                    if (location_instruction_list.at(i).at(0) == "route") {
                        if (location_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'route'";
                            error_line = line_nbr;
                            return false;
                        }
                        location_conf.set_route(location_instruction_list.at(i).at(1));
                    } else if (location_instruction_list.at(i).at(0) == "root") {
                        if (location_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'root'";
                            error_line = line_nbr;
                            return false;
                        }
                        location_conf.set_root(location_instruction_list.at(i).at(1));
                    } else if (location_instruction_list.at(i).at(0) == "error_page") {
                        if (location_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'error_page'";
                            error_line = line_nbr;
                            return false;
                        }
                        location_conf.set_error_page(location_instruction_list.at(i).at(1));
                    } else if (location_instruction_list.at(i).at(0) == "client_max_body_size") {
                        if (location_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'client_max_body_size'";
                            error_line = line_nbr;
                            return false;
                        }
                        const std::string &arg = location_instruction_list.at(i).at(1);
                        if (!is_valid_client_max_body_size(arg)) {
                            error_message = "Invalid argument for 'client_max_body_size' : " + arg;
                            error_line = line_nbr;
                            return false;
                        }
                        size_t size = translate_client_max_body_size(arg);
                        location_conf.set_client_max_body_size(size);
                    } else if (location_instruction_list.at(i).at(0) == "index") {
                        std::vector<std::string> indexes(location_instruction_list.at(i).begin() + 1,
                                                         location_instruction_list.at(i).end());
                        location_conf.set_indexes(indexes);
                    } else if (location_instruction_list.at(i).at(0) == "accept") {
                        std::set<std::string> accepted_methods(
                                location_instruction_list.at(i).begin() + 1,
                                location_instruction_list.at(i).end());
                        location_conf.set_accepted_methods(accepted_methods);
                    } else if (location_instruction_list.at(i).at(0) == "redirect") {
                        if (location_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'redirect'";
                            error_line = line_nbr;
                            return false;
                        }
                        location_conf.set_redirect(location_instruction_list.at(i).at(1));
                    } else if (location_instruction_list.at(i).at(0) == "list_directory") {
                        if (location_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'list_directory'";
                            error_line = line_nbr;
                            return false;
                        }
                        location_conf.set_list_directory(location_instruction_list.at(i).at(1) == "on");
                    } else if (location_instruction_list.at(i).at(0) == "upload_dir") {
                        if (location_instruction_list.at(i).size() != 2) {
                            error_message = "Invalid number of args for 'upload_dir'";
                            error_line = line_nbr;
                            return false;
                        }
                        location_conf.set_upload_dir(location_instruction_list.at(i).at(1));
                    } else {
                        error_message = "Invalid field '" + location_instruction_list.at(i).at(0) + "'";
                        error_line = line_nbr;
                        return false;
                    }
                }
            }
            scope.pop();
        } else { // error
            error_message = "Invalid instruction '" + line + "'";
            error_line = line_nbr;
            return false;
        }
    }
    if (scope.size() != 1) {
        error_message = "Unexpected end of file";
        error_line = line_nbr;
        return false;
    }
    if (scope_to_instructions.find("http") == scope_to_instructions.end()
            || scope_to_instructions.at("http").empty()) {
        error_message = "Config does not contain 'http' section";
        error_line = line_nbr;
        return false;
    }
    if (scope_to_instructions.find("server") == scope_to_instructions.end()
        || scope_to_instructions.at("server").empty()) {
        error_message = "Config does not contain any 'server' section";
        error_line = line_nbr;
        return false;
    }
    propagate();
    return true;
}

void config_parser::propagate() {
    // Propagate from main scope to http scope
    conf.set_error_log(get_absolute_path(conf.get_error_log()));
    if (conf.get_http_conf().get_error_log().empty()) {
        conf.get_http_conf().set_error_log(conf.get_error_log());
    }
    // Propagate from http scope to server scope
    http_config &http_conf = conf.get_http_conf();
    http_conf.set_error_log(get_absolute_path(http_conf.get_error_log()));
    http_conf.set_access_log(get_absolute_path(http_conf.get_access_log()));
    http_conf.set_error_page(get_absolute_path(http_conf.get_error_page()));
    http_conf.set_root(get_absolute_path(http_conf.get_root()));
    for (size_t i = 0; i < http_conf.get_server_configs().size(); ++i) {
        server_config &server_conf = http_conf.get_server_configs().at(i);
        if (server_conf.get_root().empty()) {
            server_conf.set_root(http_conf.get_root());
        }
        if (server_conf.get_error_page().empty()) {
            server_conf.set_error_page(http_conf.get_error_page());
        }
        if (server_conf.get_access_log().empty()) {
            server_conf.set_access_log(http_conf.get_access_log());
        }
        if (server_conf.get_error_log().empty()) {
            server_conf.set_error_log(http_conf.get_error_log());
        }
        if (server_conf.get_client_max_body_size() == 0) {
            server_conf.set_client_max_body_size(http_conf.get_client_max_body_size());
        }
        if (server_conf.get_indexes().empty()) {
            server_conf.set_indexes(http_conf.get_indexes());
        }
        server_conf.set_error_log(get_absolute_path(server_conf.get_error_log()));
        server_conf.set_access_log(get_absolute_path(server_conf.get_access_log()));
        server_conf.set_error_page(get_absolute_path(server_conf.get_error_page()));
        server_conf.set_root(get_absolute_path(server_conf.get_root()));
        server_conf.set_cgi_path(get_absolute_path(server_conf.get_cgi_path()));
        // Propagate from server scope to location scope
        for (size_t j = 0; j < server_conf.get_location_configs().size(); ++j) {
            location_config &location_conf = server_conf.get_location_configs().at(j);
            if (location_conf.get_root().empty()) {
                location_conf.set_root(server_conf.get_root());
            }
            if (location_conf.get_error_page().empty()) {
                location_conf.set_error_page(server_conf.get_error_page());
            }
            if (location_conf.get_client_max_body_size() == 0) {
                location_conf.set_client_max_body_size(server_conf.get_client_max_body_size());
            }
            if (location_conf.get_indexes().empty()) {
                location_conf.set_indexes(server_conf.get_indexes());
            }
            location_conf.set_error_page(get_absolute_path(location_conf.get_error_page()));
            location_conf.set_root(get_absolute_path(location_conf.get_root()));
            location_conf.set_upload_dir(get_absolute_path(location_conf.get_upload_dir()));
        }
    }
}

void config_parser::compile() {
    if (!parse_config()) {
        scope_to_instructions.clear();
    }
}

const std::string &config_parser::get_error_message() const {
    return error_message;
}

size_t config_parser::get_error_line() const {
    return error_line;
}

bool config_parser::is_valid() const {
    return error_message.empty() && error_line == std::numeric_limits<size_t>::max();
}

config &config_parser::get_result() {
    return conf;
}
