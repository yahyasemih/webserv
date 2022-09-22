//
// Created by Yahya Ez-zainabi on 9/13/22.
//

#include "server.hpp"

static std::map<std::string, std::string> mime_types() {
    std::map<std::string, std::string> types;

    types.insert(std::make_pair(".aac", "audio/aac"));
    types.insert(std::make_pair(".abw", "application/x-abiword"));
    types.insert(std::make_pair(".avif", "image/avif"));
    types.insert(std::make_pair(".avi", "video/x-msvideo"));
    types.insert(std::make_pair(".azw", "application/vnd.amazon.ebook"));

    types.insert(std::make_pair(".bin", "application/octet-stream"));
    types.insert(std::make_pair(".bmp", "image/bmp"));
    types.insert(std::make_pair(".bz", "application/x-bzip"));
    types.insert(std::make_pair(".bz2", "application/x-bzip2"));

    types.insert(std::make_pair(".cda", "application/x-cdf"));
    types.insert(std::make_pair(".csh", "application/x-csh"));
    types.insert(std::make_pair(".css", "text/css"));
    types.insert(std::make_pair(".csv", "text/csv"));

    types.insert(std::make_pair(".doc", "application/msword"));
    types.insert(std::make_pair(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));

    types.insert(std::make_pair(".eot", "application/vnd.ms-fontobject"));
    types.insert(std::make_pair(".epub", "application/epub+zip"));

    types.insert(std::make_pair(".gif", "image/gif"));
    types.insert(std::make_pair(".gz", "application/gzip"));

    types.insert(std::make_pair(".htm", "text/html"));
    types.insert(std::make_pair(".html", "text/html"));

    types.insert(std::make_pair(".ico", "image/vnd.microsoft.icon"));
    types.insert(std::make_pair(".ics", "text/calendar"));

    types.insert(std::make_pair(".jar", "application/java-archive"));
    types.insert(std::make_pair(".jpeg", "image/jpeg"));
    types.insert(std::make_pair(".jpg", "image/jpeg"));
    types.insert(std::make_pair(".js", "text/javascript"));
    types.insert(std::make_pair(".json", "application/json"));
    types.insert(std::make_pair(".jsonld", "application/ld+json"));

    types.insert(std::make_pair(".mid", "audio/midi"));
    types.insert(std::make_pair(".midi", "audio/x-midi"));
    types.insert(std::make_pair(".mjs", "text/javascript"));
    types.insert(std::make_pair(".mp3", "audio/mpeg"));
    types.insert(std::make_pair(".mp4", "video/mp4"));
    types.insert(std::make_pair(".mpeg", "video/mpeg"));
    types.insert(std::make_pair(".mpkg", "application/vnd.apple.installer+xml"));

    types.insert(std::make_pair(".odp", "application/vnd.oasis.opendocument.presentation"));
    types.insert(std::make_pair(".ods", "application/application/vnd.oasis.opendocument.spreadsheet"));
    types.insert(std::make_pair(".odt", "application/vnd.oasis.opendocument.text"));
    types.insert(std::make_pair(".oga", "audio/ogg"));
    types.insert(std::make_pair(".ogv", "video/ogg"));
    types.insert(std::make_pair(".ogx", "application/ogg"));
    types.insert(std::make_pair(".opus", "audio/opus"));
    types.insert(std::make_pair(".otf", "font/otf"));

    types.insert(std::make_pair(".png", "image/png"));
    types.insert(std::make_pair(".pdf", "application/pdf"));
    types.insert(std::make_pair(".php", "application/x-httpd-php"));
    types.insert(std::make_pair(".ppt", "application/vnd.ms-powerpoint"));
    types.insert(std::make_pair(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));

    types.insert(std::make_pair(".rar", "application/vnd.rar"));
    types.insert(std::make_pair(".rtf", "application/rtf"));

    types.insert(std::make_pair(".sh", "application/x-sh"));
    types.insert(std::make_pair(".svg", "image/svg+xml"));

    types.insert(std::make_pair(".tar", "application/x-tar"));
    types.insert(std::make_pair(".tif", "image/tiff"));
    types.insert(std::make_pair(".tiff", "image/tiff"));
    types.insert(std::make_pair(".ts", "video/mp2t"));
    types.insert(std::make_pair(".ttf", "font/ttf"));
    types.insert(std::make_pair(".txt", "text/plain"));

    types.insert(std::make_pair(".vsd", "application/vnd.visio"));
    types.insert(std::make_pair(".wav", "audio/wav"));
    types.insert(std::make_pair(".weba", "audio/webm"));
    types.insert(std::make_pair(".webm", "audio/webm"));
    types.insert(std::make_pair(".webp", "audio/webp"));
    types.insert(std::make_pair(".woff", "font/woff"));
    types.insert(std::make_pair(".woff2", "font/woff2"));

    types.insert(std::make_pair(".xhtml", "application/xhtml+xml"));
    types.insert(std::make_pair(".xls", "application/vnd.ms-excel"));
    types.insert(std::make_pair(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
    types.insert(std::make_pair(".xml", "application/xml"));
    types.insert(std::make_pair(".xul", "application/vnd.mozilla.xul+xml"));

    types.insert(std::make_pair(".zip", "application/zip"));
    types.insert(std::make_pair(".3gp", "video/3gpp"));
    types.insert(std::make_pair(".3g2", "video/3gpp2"));

    types.insert(std::make_pair("7z", "application/x-7z-compressed"));
    types.insert(std::make_pair("", "text/html"));

    return types;
}

const std::map<std::string, std::string> server::mime_type_map = mime_types();

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
        std::cout << "result: " << std::endl;
        std::cout << "\t error_log: " << conf.get_error_log() << std::endl;
        std::cout << "\t http config: " << std::endl;
        std::cout << "\t\t root: " << conf.get_http_conf().get_root() << std::endl;
        std::cout << "\t\t error_page: " << conf.get_http_conf().get_error_page() << std::endl;
        std::cout << "\t\t access_log: " << conf.get_http_conf().get_access_log() << std::endl;
        std::cout << "\t\t error_log: " << conf.get_http_conf().get_error_log() << std::endl;
        std::cout << "\t\t client_max_body_size: " << conf.get_http_conf().get_client_max_body_size() << std::endl;
        std::cout << "\t\t index: ";
        for (size_t i = 0 ; i < conf.get_http_conf().get_indexes().size(); ++i) {
            std::cout << conf.get_http_conf().get_indexes().at(i) << " ";
        }
        std::cout << std::endl;
        std::cout << "\t\t server_configs: " << conf.get_http_conf().get_server_configs().size() << std::endl;
        for (size_t i = 0; i < conf.get_http_conf().get_server_configs().size(); ++i) {
            server_config &server_conf = conf.get_http_conf().get_server_configs().at(i);
            std::cout << "\t\t\t listen: " << server_conf.get_host() << ":" << server_conf.get_port() << std::endl;
            std::cout << "\t\t\t root: " << server_conf.get_root() << std::endl;
            std::cout << "\t\t\t error_page: " << server_conf.get_error_page() << std::endl;
            std::cout << "\t\t\t access_log: " << server_conf.get_access_log() << std::endl;
            std::cout << "\t\t\t error_log: " << server_conf.get_error_log() << std::endl;
            std::cout << "\t\t\t client_max_body_size: " << server_conf.get_client_max_body_size() << std::endl;
            std::cout << "\t\t\t index: ";
            for (size_t i = 0 ; i < server_conf.get_indexes().size(); ++i) {
                std::cout << server_conf.get_indexes().at(i) << " ";
            }
            std::cout << std::endl;
            std::cout << "\t\t\t server_names: ";
            std::set<std::string>::iterator it = server_conf.get_server_names().begin();
            for (; it != server_conf.get_server_names().end(); ++it) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
            std::cout << "\t\t\t locations: " << server_conf.get_location_configs().size() << std::endl;
            for (size_t j = 0; j < server_conf.get_location_configs().size(); ++j) {
                location_config &location_conf = server_conf.get_location_configs().at(j);
                std::cout << "\t\t\t\t route: " << location_conf.get_route() << std::endl;
                std::cout << "\t\t\t\t root: " << location_conf.get_root() << std::endl;
                std::cout << "\t\t\t\t error_page: " << location_conf.get_error_page() << std::endl;
                std::cout << "\t\t\t\t client_max_body_size: " << location_conf.get_client_max_body_size() << std::endl;
                std::cout << "\t\t\t\t index: ";
                for (size_t i = 0 ; i < location_conf.get_indexes().size(); ++i) {
                    std::cout << location_conf.get_indexes().at(i) << " ";
                }
                std::cout << std::endl;
                std::cout << "\t\t\t\t redirect: " << location_conf.get_redirect() << std::endl;
                std::cout << "\t\t\t\t upload_dir: " << location_conf.get_upload_dir() << std::endl;
                std::cout << "\t\t\t\t list_directory: " << location_conf.is_list_directory() << std::endl;
                std::cout << "\t\t\t\t accept: ";
                std::set<std::string>::iterator it = location_conf.get_accepted_methods().begin();
                for (; it != location_conf.get_accepted_methods().end(); ++it) {
                    std::cout << *it << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    protoent *protocol;
    int options = 1;
    protocol = getprotobyname("tcp");
    if (!protocol) {
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
        in_port_t port = conf.get_http_conf().get_server_configs().at(i).get_port();
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
    sockaddr_in socket_remote_addr = {};
    int new_socket = accept(socket_fds[index], (sockaddr *)&socket_remote_addr, &addr_len);
    if (new_socket < 0) {
        perror("Could not accept connection : ");
    } else {
        fcntl(new_socket, F_SETFL, O_NONBLOCK);
        pollfd pf = {};
        pf.fd = new_socket;
        pf.events = POLLIN;
        poll_fds.push_back(pf);
        clients.insert(std::make_pair(new_socket, client(new_socket, socket_addr, socket_remote_addr)));
    }
}

void server::start() {
    is_running = true;
    while (is_running) {
        try {
            int r = poll(poll_fds.data(), poll_fds.size(), 0);
            if (r > 0) {
                for (size_t index = 0; index < poll_fds.size(); ++index) {
                    pollfd &pf = poll_fds.at(index);
                    if (pf.revents & POLLRDNORM) {
                        accept_connection(index);
                    } else if (pf.revents & POLLHUP) {
                        std::cout << "connection to fd " << pf.fd << " closed" << std::endl;
                        clients.erase(pf.fd);
                        close(pf.fd);
                        pf.fd = -1;
                    } else if (pf.revents & POLLIN) {
                        handle_request(pf);
                    }
                }
                clean_fds();
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

const server_config &server::get_matching_server(const std::string &ip, const std::string &host, in_port_t port) {
    const server_config *server_conf_ptr = NULL;
    for (size_t i = 0; i < conf.get_http_conf().get_server_configs().size(); ++i) {
        const server_config &server_conf = conf.get_http_conf().get_server_configs().at(i);
        const std::set<std::string> &server_names = server_conf.get_server_names();
        if (server_names.find(host) != server_names.end() && server_conf.get_port() == port) {
            return server_conf;
        }
        if (server_conf.get_port() == port && server_conf.get_host() == ip && server_conf_ptr == NULL) {
            server_conf_ptr = &server_conf;
        }
    }
    if (server_conf_ptr == NULL) {
        throw std::runtime_error("No configuration match the {ip, host, port} provided");
    }
    return *server_conf_ptr;
}

static bool ends_with(const std::string &value, const std::string &ending)
{
    if (ending.size() > value.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

static std::string get_valid_path(const std::string &root, std::string path) {
    std::string res;
    size_t idx;

    while ((idx = path.find('/')) != std::string::npos) {
        res += "/";
        path = path.substr(idx + 1);
        if (path.find("../") == 0) {
            res = res.substr(0, res.find_last_of('/', res.size() - 2));
        } else if (path.find("./") == 0) {
            res.resize(res.size() - 1);
        } else {
            res += path.substr(0, path.find('/'));
        }
    }

    if (ends_with(res, "/..")) {
        res.resize(res.size() - 2);
    }
    return root + res;
}

static std::string get_file_extension(const std::string &file) {
    std::string ext;
    size_t idx = file.find_last_of('.');
    if (idx == std::string::npos) {
        ext = "";
    } else {
        ext = file.substr(idx);
    }
    return ext;
}

void server::handle_request(pollfd &pf) {
    client &c = clients.at(pf.fd);
    ssize_t res = c.receive();

    if (res <= 0) {
        if (res < 0) {
            std::cout << "error occurred, dropping connection with fd " << pf.fd << std::endl;
        } else {
            std::cout << "client with fd " << pf.fd;
            std::cout << " closed its half side of the connection" << std::endl;
        }
        clients.erase(pf.fd);
        close(pf.fd);
        pf.fd = -1;
        return;
    }

    if (c.request_not_complete()) {
        return;
    }

    request_builder req_builder(c.get_request());
    std::string ip = inet_ntoa(c.get_local_addr());
    std::string host;
    in_port_t port = c.get_local_port();
    std::string &header_host = req_builder.get_header("Host");
    if (!header_host.empty()) {
        size_t column_idx = header_host.find(':');
        if (column_idx == std::string::npos) {
            host = header_host;
        } else {
            host = header_host.substr(0, column_idx);
        }
    } else {
        host = ip;
    }
    response_builder res_builder;
    std::string file;
    const server_config &conf = get_matching_server(ip, host, port);
    file = get_valid_path(conf.get_root(), req_builder.get_path());
    struct stat s = {};
    stat(file.c_str(), &s);
    if (s.st_mode & S_IFDIR) {
        if (*file.rbegin() != '/') {
            file += "/";
        }
        file += conf.get_indexes().at(0);
    }
    std::ifstream f(file.c_str());
    if (f.is_open()) {
        res_builder.set_status(200);
    } else {
        std::ifstream error_file(conf.get_error_page().c_str());
        res_builder.set_status(404)
                .append_body(error_file);
    }
    res_builder.set_header("Server", "yez-zain-server/1.0.0 (UNIX)")
            .set_header("Content-Type", get_mime_type(file))
            .set_header("Connection", "keep-alive");
    if (f.is_open()) {
        res_builder.append_body(f);
    }
    std::string response = res_builder.build();
    while ((res = send(pf.fd, response.c_str(), response.size(), 0)) < static_cast<ssize_t>(response.size())
            && res >= 0) {
        response = response.substr(res);
    }
    if (res < 0) {
        std::cout << "error occurred, dropping connection with fd " << pf.fd << std::endl;
        clients.erase(pf.fd);
        close(pf.fd);
        pf.fd = -1;
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

std::string server::get_mime_type(const std::string &file) {
    std::map<std::string, std::string>::const_iterator it = mime_type_map.find(get_file_extension(file));
    return it == mime_type_map.end() ? mime_type_map.at("") : it->second;
}
