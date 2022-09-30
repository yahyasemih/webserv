//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server.hpp"

extern char **environ;

server::server(const std::string &config_file) : is_running() {
    config_parser parser(config_file);
    parser.compile();
    if (!parser.is_valid()) {
        std::stringstream error_msg;
        error_msg << parser.get_error_message();
        if (parser.get_error_line() > 0) {
            error_msg << " at line : ";
            error_msg << parser.get_error_line();
        }
        throw std::runtime_error(error_msg.str());
    } else {
        conf = parser.get_result();
    }

    create_sockets();
}

void server::create_sockets() {
    logger error_logger(conf.get_error_log());
    protoent *protocol;
    int socket_fd;
    int options = 1;
    protocol = getprotobyname("tcp");
    if (!protocol) {
        error_logger.error("Error while getting protocol TCP");
        throw std::runtime_error("Error while getting protocol TCP");
    }

    std::set<address_port> already_opened;

    for (size_t i = 0; i < conf.get_http_conf().get_server_configs().size(); ++i) {
        const server_config &server_conf = conf.get_http_conf().get_server_configs().at(i);
        std::set<address_port>::const_iterator it = server_conf.get_addresses().begin();

        for (; it != server_conf.get_addresses().end(); ++it) {
            if (already_opened.find(*it) != already_opened.end()) {
                continue;
            }
            socket_fd = socket(AF_INET, SOCK_STREAM, protocol->p_proto);
            if (socket_fd < 0) {
                error_logger.error("Socket creation failed");
                throw std::runtime_error("Socket creation failed");
            }
            if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options))) {
                error_logger.error("Setting socket option 'SO_REUSEADDR' failed");
                throw std::runtime_error("Setting socket option 'SO_REUSEADDR' failed");
            }
            if (setsockopt(socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &options, sizeof(options))) {
                error_logger.error("Setting socket option 'SO_NOSIGPIPE' failed");
                throw std::runtime_error("Setting socket option 'SO_NOSIGPIPE' failed");
            }
            fcntl(socket_fd, F_SETFL, O_NONBLOCK);
            sockaddr_in socket_addr = {};
            socket_addr.sin_family = AF_INET;
            const std::string &ip = it->get_ip();
            // TODO (minor): Check if IP is a valid IP
            in_port_t port = it->get_port();
            socket_addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socket_addr.sin_port = htons(port);
            if (bind(socket_fd, (sockaddr *)&socket_addr, sizeof(socket_addr)) < 0) {
                error_logger.error("Binding failed");
                throw std::runtime_error("Binding failed");
            }
            if (listen(socket_fd, 256) < 0) {
                error_logger.error("Listening failed");
                throw std::runtime_error("Listening failed");
            }
            pollfd pf = {};
            pf.fd = socket_fd;
            pf.events = POLLSTANDARD;
            poll_fds.push_back(pf);
            std::cout << "Successfully listening on " << ip << ":" << port << std::endl;
            already_opened.insert(*it);
        }
    }
}

server::~server() {
    stop();
}

void server::accept_connection(pollfd &pf) {
    logger error_logger(conf.get_error_log());
    sockaddr_in socket_addr = {};
    socklen_t addr_len = sizeof(socket_addr);
    if (getsockname(pf.fd, (sockaddr *)&socket_addr, &addr_len)) {
        error_logger.error("Get socket name failed");
    }
    sockaddr_in socket_remote_addr = {};
    int new_socket = accept(pf.fd, (sockaddr *)&socket_remote_addr, &addr_len);
    if (new_socket < 0) {
        error_logger.error("Could not accept connection");
    } else {
        fcntl(new_socket, F_SETFL, O_NONBLOCK);
        pollfd new_pf = {};
        new_pf.fd = new_socket;
        new_pf.events = POLLIN | POLLOUT;
        poll_fds.push_back(new_pf);
        clients.insert(std::make_pair(new_socket, client(new_socket, socket_addr, socket_remote_addr)));
    }
}

void server::start() {
    is_running = true;
    logger error_logger(conf.get_error_log());

    while (is_running) {
        try {
            int r = poll(poll_fds.data(), poll_fds.size(), 0);
            if (r > 0) {
                for (size_t index = 0; index < poll_fds.size(); ++index) {
                    pollfd &pf = poll_fds.at(index);
                    if (pf.revents & POLLRDNORM) {
                        accept_connection(pf);
                    } else if (pf.revents & POLLHUP) {
                        clean_single_fd(pf);
                    } else if (pf.revents & POLLIN) {
                        handle_request(pf);
                    } else if (pf.revents & POLLOUT) {
                        serve_response(pf);
                    }
                }
            }
            check_idle_clients();
            clean_fds();
        } catch (const std::exception &e) {
            error_logger.error("Exception: " + std::string(e.what() ? e.what() : ""));
            stop();
        }
    }
}

void server::clean_single_fd(pollfd &pf) {
    clients.erase(pf.fd);
    close(pf.fd);
    std::cout << "connection to fd " << pf.fd << " closed" << std::endl;
    pf.fd = -1;
}

void server::check_idle_clients() {
    std::time_t current_time = time(NULL);

    for (size_t index = 0; index < poll_fds.size(); ++index) {
        pollfd &pf = poll_fds.at(index);
        std::map<int, client>::iterator it = clients.find(pf.fd);

        if (it != clients.end()) {
            if (current_time - it->second.get_last_request_ts() > constants::CLIENT_TIMEOUT_SEC) {
                clean_single_fd(pf);
            }
        }
    }
}

void server::stop() {
    is_running = false;
    std::cout << "Closing server" << std::endl;
    for (size_t i = 0; i < poll_fds.size(); ++i) {
        shutdown(poll_fds[i].fd, 2);
        close(poll_fds[i].fd);
    }
}

const server_config &server::get_matching_server(const std::string &ip, const std::string &hostname, in_port_t port) {
    const server_config *server_conf_ptr = NULL;
    for (size_t i = 0; i < conf.get_http_conf().get_server_configs().size(); ++i) {
        const server_config &server_conf = conf.get_http_conf().get_server_configs().at(i);
        const std::set<std::string> &server_names = server_conf.get_server_names();
        address_port addr(ip, port);

        if (server_names.find(hostname) != server_names.end()
                && server_conf.get_addresses().find(addr) != server_conf.get_addresses().end()) {
            return server_conf;
        }

        if (server_conf.get_addresses().find(addr) != server_conf.get_addresses().end() && server_conf_ptr == NULL) {
            server_conf_ptr = &server_conf;
        }
    }

    if (server_conf_ptr == NULL) {
        throw std::runtime_error("No configuration match the {ip, hostname, port} provided");
    }
    return *server_conf_ptr;
}

const location_config &server::get_matching_location(const request_builder &req_builder,
         const server_config &server_conf) {
    size_t idx = 0;

    for (size_t i = 0; i < server_conf.get_location_configs().size(); ++i) {
        const location_config &location_conf = server_conf.get_location_configs().at(i);
        const std::set<std::string> &accepted_methods = location_conf.get_accepted_methods();
        if (accepted_methods.find(req_builder.get_method()) == accepted_methods.end()) {
            if (req_builder.get_uri().find(location_conf.get_route()) == 0) {
                idx = i;
            }
            continue;
        }
        if (req_builder.get_uri().find(location_conf.get_route()) == 0) {
            idx = i;
            break;
        }
    }
    return server_conf.get_location_configs().at(idx);
}

void server::handle_request(pollfd &pf) {
    logger error_logger(conf.get_error_log());
    client &c = clients.at(pf.fd);
    c.reset_last_request_ts();
    ssize_t res = c.receive();

    if (res <= 0) {
        std::ostringstream error_msg;
        if (res < 0) {
            error_msg << "error occurred, dropping connection with fd " << pf.fd;
        } else {
            error_msg << "client with fd " << pf.fd;
            error_msg << " closed its half side of the connection";
        }
        clean_single_fd(pf);
        error_logger.error(error_msg.str());
        return;
    }

    if (c.request_not_complete()) {
        return;
    }

    request_builder req_builder(c.get_request());
    response_builder res_builder;
    std::string &response = c.get_response();

    if (req_builder.is_bad_request()) {
        utilities::on_error(400, conf.get_http_conf().get_error_page(), res_builder);
        response = res_builder.build();
        pf.events = POLLOUT; // client ready to serve response, listening again to write events
        return;
    }

    std::string ip = inet_ntoa(c.get_local_addr());
    std::string hostname;
    in_port_t port = c.get_local_port();
    std::string &header_host = req_builder.get_header("address_port");

    if (!header_host.empty()) {
        size_t column_idx = header_host.find(':');
        if (column_idx == std::string::npos) {
            hostname = header_host;
        } else {
            hostname = header_host.substr(0, column_idx);
        }
    } else {
        hostname = ip;
    }

    std::string file;
    const server_config &server_conf = get_matching_server(ip, hostname, port);
    const location_config &location_conf = get_matching_location(req_builder, server_conf);
    logger access_logger(server_conf.get_access_log());

    file = utilities::get_valid_path(location_conf.get_root(),
            req_builder.get_path().c_str() + location_conf.get_route().size() - 1);
    size_t body_limit = location_conf.get_client_max_body_size();
    const std::set<std::string> &accepted_methods = location_conf.get_accepted_methods();
    bool redirect = !location_conf.get_redirect().empty() && location_conf.get_redirect() != "no";

    if (redirect) {
        res_builder.set_status(301)
                .set_header("Location", location_conf.get_redirect());
        if (!req_builder.get_body().empty()) {
            res_builder.set_body(req_builder.get_body().data());
        }
        if (!req_builder.get_header("Cookie").empty()) {
            res_builder.set_header("Cookie", req_builder.get_header("Cookie"));
        }
        if (!req_builder.get_header("Set-Cookie").empty()) {
            res_builder.set_header("Set-Cookie", req_builder.get_header("Set-Cookie"));
        }
    } else if (accepted_methods.find(req_builder.get_method()) == accepted_methods.end()) {
        utilities::on_error(405, location_conf.get_error_page(), res_builder);
    } else if (body_limit > 0 && body_limit < req_builder.get_body().size()) {
        utilities::on_error(413, location_conf.get_error_page(), res_builder);
    } else {
        process_request(req_builder, res_builder, file, server_conf, location_conf, c);
    }
    access_logger.access(req_builder, res_builder, c);
    response = res_builder.build();
    pf.events = POLLOUT; // client ready to serve response, listening again to write events
}

void server::serve_response(pollfd &pf) {
    if (clients.find(pf.fd) == clients.end()) {
        return;
    }
    client &c = clients.at(pf.fd);
    std::string &response = c.get_response();
    if (response.empty()) {
        pf.events = POLLIN; // client has no more response to send, listening to read events only
        return;
    }

    c.reset_last_request_ts();
    ssize_t res = send(pf.fd, response.c_str(), response.size(), 0);
    if (res < 0) {
        logger error_logger(conf.get_error_log());
        std::ostringstream error_msg;
        error_msg << "error occurred, dropping connection with fd " << pf.fd;
        clean_single_fd(pf);
        error_logger.error(error_msg.str());
        return;
    }
    response = response.substr(res);
}

void server::handle_put_delete(request_builder &req_builder, response_builder &res_builder, std::string &file,
        const location_config &location_conf) {
    if (req_builder.get_method() == "PUT") {
        std::string new_file;
        if (!location_conf.get_upload_dir().empty()) {
            new_file = location_conf.get_upload_dir() + "/" + utilities::get_file_basename(file);
        } else {
            new_file = file;
        }
        if (!access(new_file.c_str(), F_OK)) {
            utilities::on_error(409, location_conf.get_error_page(), res_builder);
            return;
        }
        std::ofstream put_file(new_file.c_str(), std::ios_base::binary);
        put_file.write(req_builder.get_body().data(), static_cast<std::streamsize>(req_builder.get_body().size()));
        res_builder.set_status(204)
                .set_header("Content-Location", utilities::get_file_basename(file));
    } else if (req_builder.get_method() == "DELETE") {
        if (access(file.c_str(), F_OK)) {
            utilities::on_error(404, location_conf.get_error_page(), res_builder);
            return;
        }
        if (!req_builder.get_body().empty()) {
            utilities::on_error(415, location_conf.get_error_page(), res_builder);
            return;
        }
        if (remove(file.c_str())) {
            utilities::on_error(409, location_conf.get_error_page(), res_builder);
            return;
        }
        res_builder.set_status(204);
    }
}

void server::process_request(request_builder &req_builder, response_builder &res_builder, std::string &file,
        const server_config &server_conf, const location_config &location_conf, const client &c) {
    if ((!server_conf.is_cgi_route() || utilities::get_file_extension(file) != server_conf.get_cgi_extension())
            && req_builder.get_method() != "GET" && req_builder.get_method() != "POST") {
        handle_put_delete(req_builder, res_builder, file, location_conf);
        return;
    }
    struct stat s = {};
    int ret = stat(file.c_str(), &s);

    if (ret != 0) {
        utilities::on_error(404, location_conf.get_error_page(), res_builder);
        return;
    }
    if (access(file.c_str(), R_OK)) {
        utilities::on_error(403, location_conf.get_error_page(), res_builder);
        return;
    }
    if (s.st_mode & S_IFDIR) {
        if (*file.rbegin() != '/') {
            file += "/";
        }
        size_t i = 0;

        while (i < location_conf.get_indexes().size()) {
            std::string index_to_check = file + location_conf.get_indexes().at(i);
            if (!access(index_to_check.c_str(), F_OK | R_OK)) {
                break;
            }
            ++i;
        }
        if (i == location_conf.get_indexes().size()) { // Fallback to directory listing
            if (location_conf.is_list_directory()) {
                directory_index_builder index_page(file, location_conf.get_root());
                res_builder.set_status(200)
                        .set_body(index_page.list_directory());
                return;
            } else {
                utilities::on_error(403, location_conf.get_error_page(), res_builder);
                return;
            }
        } else {
            file += location_conf.get_indexes().at(i);
        }
    }
    if (server_conf.is_cgi_route() && utilities::get_file_extension(file) == server_conf.get_cgi_extension()) {
        run_cgi(req_builder, res_builder, file, server_conf, location_conf, c);
    } else {
        run_static(req_builder, res_builder, file, location_conf);
    }
}

void server::run_static(request_builder &req_builder, response_builder &res_builder, const std::string &file,
        const location_config &location_conf) {
    if (access(file.c_str(), F_OK)) {
        utilities::on_error(404, location_conf.get_error_page(), res_builder);
        return;
    }
    if (access(file.c_str(), R_OK)) {
        utilities::on_error(403, location_conf.get_error_page(), res_builder);
        return;
    }
    std::ifstream f(file.c_str());
    res_builder.set_status(200)
            .set_header("Server", constants::SERVER_NAME_VERSION)
            .set_header("Content-Type", utilities::get_mime_type(file))
            .append_body(f);
    if (!req_builder.get_header("Range").empty()) {
        utilities::truncate_body(req_builder, res_builder, f);
    }
}

void server::run_cgi(request_builder &req_builder, response_builder &res_builder, const std::string &file,
        const server_config &server_conf, const location_config &location_conf, const client &c) {
    int child_to_parent[2];
    int parent_to_child[2];
    if (pipe(child_to_parent) < 0) {
        res_builder.set_status(502);
        return;
    }
    if (pipe(parent_to_child) < 0) {
        close(child_to_parent[0]);
        close(child_to_parent[1]);
        res_builder.set_status(502);
        return;
    }
    std::ostringstream size_stream;
    size_stream << req_builder.get_body().size();
    req_builder.set_header("Content-Length", size_stream.str());
    int pid = fork();
    if (pid < 0) {
        close(child_to_parent[0]);
        close(child_to_parent[1]);
        close(parent_to_child[0]);
        close(parent_to_child[1]);
        res_builder.set_status(502);
    } else if (pid == 0) {
        chdir(location_conf.get_root().c_str());
        utilities::set_environment_variables(req_builder, file, location_conf, c);

        close(child_to_parent[0]);
        dup2(child_to_parent[1], 1);
        close(child_to_parent[1]);

        close(parent_to_child[1]);
        dup2(parent_to_child[0], 0);
        close(parent_to_child[0]);
        close(2);

        const char *args[3] = {server_conf.get_cgi_path().c_str(), file.c_str(), NULL};
        int ret = execve(args[0], const_cast<char **>(args), environ);
        if (ret < 0) {
            std::string error_res = "Status: 502 Bad Gateway\r\n";
            error_res += "Content-Length: 0\r\n\r\n";
            write(1, error_res.c_str(), error_res.size());
        }
        exit(1);
    } else {
        std::stringstream strm;
        char buffer[constants::BUFFER_SIZE + 1];
        close(child_to_parent[1]);
        close(parent_to_child[0]);

        write(parent_to_child[1], req_builder.get_body().data(), req_builder.get_body().size());
        close(parent_to_child[1]);
        ssize_t r;
        while ((r = read(child_to_parent[0], buffer, constants::BUFFER_SIZE)) != 0) {
            buffer[r] = '\0';
            strm << buffer;
        }
        close(child_to_parent[0]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            res_builder.set_status(200);
        } else {
            res_builder.set_status(500);
        }
        std::string line;
        while (std::getline(strm, line) && !line.empty() && line != "\r") {
            line.erase(line.size() - 1);
            size_t idx = line.find(':');
            if (idx == std::string::npos) {
                continue; // invalid header
            }
            std::string key = line.substr(0, idx);
            std::string value = line.substr(idx + 2);
            if (key == "Status") {
                int cg_status = static_cast<int>(std::strtol(value.c_str(), NULL, 10));
                if (constants::STATUS_STR.find(cg_status) != constants::STATUS_STR.end()) {
                    res_builder.set_status(cg_status);
                }
            }
            res_builder.set_header(key, value);
        }
        const std::string &str = strm.str();
        size_t pos = strm.tellg();
        res_builder.set_body(str.c_str() + pos);
    }
}

void server::clean_fds() {
    // Remove any invalid file descriptor, previously closed and marked with -1
    for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end();) {
        if (it->fd == -1) {
            it = poll_fds.erase(it);
        } else {
            ++it;
        }
    }
}
