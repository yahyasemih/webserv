//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server.hpp"

const int server::BUFFER_SIZE = 1024;

server::server(const std::string &config_file) : conf(config_file) {
    protoent *protocol;
    int options = 1;
    protocol = getprotobyname("tcp");
    if (protocol == nullptr) {
        throw std::runtime_error("Error while getting protocol TCP");
    }
    socket_fds.resize(conf.get_http_conf().get_server_configs().size());
    socket_addrs.resize(conf.get_http_conf().get_server_configs().size());
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
        socket_addrs[i].sin_family = AF_INET;
        const std::string &host = conf.get_http_conf().get_server_configs().at(i).get_host();
        short port = conf.get_http_conf().get_server_configs().at(i).get_port();
        socket_addrs[i].sin_addr.s_addr = inet_addr(host.c_str());
        socket_addrs[i].sin_port = htons(port);
        if (bind(socket_fds[i], (sockaddr *)&socket_addrs[i], sizeof(socket_addrs[i])) < 0) {
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

void server::accept_connection(size_t index) {
    int addr_len = sizeof(socket_addrs[index]);
    int new_socket = accept(socket_fds[index],
                            reinterpret_cast<sockaddr *>(&socket_addrs[index]),
                            reinterpret_cast<socklen_t *>(&addr_len));
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

server::~server() {
    stop();
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
                        size_t res = read(pf.fd, buff, BUFFER_SIZE);
                        buff[res] = '\0';
                        std::cout << "got '" << buff << "'" << std::endl;
                        response_builder res_builder;
                        std::ifstream f("index.html");
                        res_builder.set_status(200)
                                .set_header("Date", "Mon, 27 Jul 2009 12:28:53 GMT")
                                .set_header("Server", "Apache/2.2.14 (Win32)")
                                .set_header("Last-Modified", "Wed, 22 Jul 2009 19:15:56 GMT")
                                .set_header("Content-Type", "text/html")
                                .set_header("Connection", "closed")
                                .append_body(f);
                        std::string response = res_builder.build();
                        write(pf.fd, response.c_str(), response.size());
                    }
                }
            }
        } catch (const std::exception &e) {
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
