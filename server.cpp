//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server.hpp"

const int server::BUFFER_SIZE = 1024 * 1024;

server::server(const std::string &config_file) : conf(config_file), is_running() {
    protoent *protocol;
    int options = 1;
    protocol = getprotobyname("tcp");
    if (protocol == nullptr) {
        throw std::runtime_error("Error while getting protocol TCP");
    }
    socket_fds.resize(conf.get_http_conf().get_server_configs().size());
    for (size_t i = 0; i < socket_fds.size(); ++i) {
        socket_fds[i] = socket(AF_INET, SOCK_STREAM, protocol->p_proto);
        if (socket_fds[i] < 0) {
            perror("Socket creation failed : ");
            throw std::runtime_error("Socket creation failed");
        }
        if (setsockopt(socket_fds[i], SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options))) {
            perror("Socket options : ");
            throw std::runtime_error("Setting socket option 'SO_REUSEADDR' failed : ");
        }
        if (setsockopt(socket_fds[i], SOL_SOCKET, SO_NOSIGPIPE, &options, sizeof(options))) {
            perror("Socket options : ");
            throw std::runtime_error("Setting socket option 'SO_NOSIGPIPE' failed : ");
        }
        fcntl(socket_fds[i], F_SETFL, O_NONBLOCK);
        sockaddr_in socket_addr = {};
        socket_addr.sin_family = AF_INET;
        const std::string &host = conf.get_http_conf().get_server_configs().at(i).get_host();
        short port = conf.get_http_conf().get_server_configs().at(i).get_port();
        socket_addr.sin_addr.s_addr = inet_addr(host.c_str());
        socket_addr.sin_port = htons(port);
        if (bind(socket_fds[i], (sockaddr *)&socket_addr, sizeof(socket_addr)) < 0) {
            perror("Binding failed : ");
            throw std::runtime_error("Binding failed");
        }
        if (listen(socket_fds[i], 100) < 0) {
            perror("Listening failed : ");
            throw std::runtime_error("Listening failed");
        }
        pollfd pf = {};
        pf.fd = socket_fds[i];
        pf.events = POLLSTANDARD;
        poll_fds.push_back(pf);
        std::cout << "Server successfully initialised on " << host << ":" << port << std::endl;
    }
}

server::~server() {
    stop();
}

void server::accept_connection(size_t index) {
    sockaddr_in socket_addr = {};
    socklen_t addr_len = sizeof(socket_addr);
    if (getsockname(socket_fds[index], (sockaddr *)&socket_addr, &addr_len)) {
        perror("get socket name failed: ");
    }
    int new_socket = accept(socket_fds[index], (sockaddr *)&socket_addr, &addr_len);
    if (new_socket < 0) {
        perror("Could not accept connection : ");
    } else {
        fcntl(new_socket, F_SETFL, O_NONBLOCK);
        pollfd pf = {};
        pf.fd = new_socket;
        pf.events = POLLIN;
        poll_fds.push_back(pf);
    }
}

void server::start() {
    is_running = true;
    while (is_running) {
        try {
            int r = poll(poll_fds.data(), poll_fds.size(), 0);
            if (r > 0) {
                std::cout << "r is " << r << std::endl;
                for (size_t index = 0; index < poll_fds.size(); ++index) {
                    pollfd &pf = poll_fds.at(index);
                    if (pf.revents) {
                        std::cout << "got event from fd " << pf.fd << " : " << pf.revents << std::endl;
                    }
                    if (pf.revents & POLLRDNORM) {
                        accept_connection(index);
                    } else if (pf.revents & POLLHUP) {
                        std::cout << "connection closed" << std::endl;
                        close(pf.fd);
                        pf.fd = -1;
                    } else if (pf.revents & POLLIN) {
                        char buff[BUFFER_SIZE];
                        size_t res = recv(pf.fd, buff, BUFFER_SIZE, 0);
                        buff[res] = '\0';
                        request_parser req_parser(buff);
                        std::string host_port = req_parser.get_headers().at("Host");
                        std::stringstream host_stream(host_port);
                        std::string host;
                        short port = 80;
                        std::getline(host_stream, host, ':');
                        if (host_stream.peek() != EOF) {
                            host_stream >> port;
                        }
                        response_builder res_builder;
                        std::string file;
                        const server_config &conf = get_matching_server(host, port);
                        file = conf.get_root() + req_parser.get_path();
                        if (file.back() == '/') {
                            file += "index.html";
                        }
                        std::cout << "-----> file : " << file << std::endl;
                        std::ifstream f(file);
                        std::string mime;
                        if (file.find(".jpeg") != std::string::npos) {
                            mime = "image/jpeg";
                        } else if (file.find(".ico") != std::string::npos) {
                            mime = "image/x-icon";
                        } else {
                            mime = "text/html";
                        }
                        res_builder.set_status(200)
                                .set_header("Server", "yez-zain-server/1.0.0 (UNIX)")
                                .set_header("Content-Type", mime)
                                .set_header("Connection", "closed")
                                .append_body(f);
                        std::string response = res_builder.build();
                        send(pf.fd, response.c_str(), response.size(), 0);
                    }
                }
                // Remove any invalid file descriptor
                std::cout << "Actual number of fds : " << poll_fds.size() << std::endl;
                for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end();) {
                    if (it->fd == -1) {
                        it = poll_fds.erase(it);
                    } else {
                        ++it;
                    }
                }
                std::cout << "Number of fds after removal : " << poll_fds.size() << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
            stop();
        }
    }
}

void server::stop() {
    is_running = false;
    std::cout << "\rClosing server" << std::endl;
    for (size_t i = 0; i < socket_fds.size(); ++i) {
        shutdown(socket_fds[i], 2);
        close(socket_fds[i]);
    }
    for (size_t i = 0; i < poll_fds.size(); ++i) {
        shutdown(poll_fds[i].fd, 2);
        close(poll_fds[i].fd);
    }
}

const server_config &server::get_matching_server(const std::string &host, short port) const {
    const server_config *server_conf_ptr = nullptr;
    for (size_t i = 0; i < conf.get_http_conf().get_server_configs().size(); ++i) {
        const server_config &server_conf = conf.get_http_conf().get_server_configs().at(i);
        const std::unordered_set<std::string> &server_names = server_conf.get_server_names();
        if (server_names.find(host) != server_names.end() && server_conf.get_port() == port) {
            return server_conf;
        }
        if (server_conf.get_port() == port && server_conf.get_host() == host && server_conf_ptr == nullptr) {
            server_conf_ptr = &server_conf;
        }
    }
    return *server_conf_ptr;
}
