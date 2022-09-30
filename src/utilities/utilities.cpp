//
// Created by Yahya Ez-zainabi on 9/29/22.
//

#include "utilities.hpp"

static bool ends_with(const std::string &value, const std::string &ending) {
    if (ending.size() > value.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string utilities::get_valid_path(const std::string &root, std::string path) {
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

std::string utilities::get_file_extension(const std::string &file) {
    std::string ext;
    size_t idx = file.find_last_of('.');
    if (idx == std::string::npos) {
        ext = "";
    } else {
        ext = file.substr(idx);
    }
    return ext;
}

std::string utilities::get_file_basename(const std::string &file) {
    std::string ext;
    size_t idx = file.find_last_of('/');
    if (idx == std::string::npos) {
        ext = file;
    } else {
        ext = file.substr(idx + 1);
    }
    return ext;
}

static int update_header_key(char c) {
    if (c == '-') {
        return '_';
    }
    return toupper(c);
}

void utilities::set_environment_variables(request_builder &req_builder, const std::string &file,
        const location_config &location_conf, const client &c) {
    std::stringstream port_stream;
    std::string server_port;
    std::string remote_port;

    port_stream << c.get_local_port();
    server_port = port_stream.str();

    port_stream.clear();
    port_stream.str("");
    port_stream << c.get_remote_port();
    remote_port = port_stream.str();

    setenv("GATEWAY_INTERFACE", constants::CGI_VERSION.c_str(), 1);
    setenv("SERVER_PROTOCOL", constants::HTTP_VERSION.c_str(), 1);
    setenv("REDIRECT_STATUS", "200", 1);

    setenv("PATH_INFO", req_builder.get_path().c_str(), 1);
    setenv("PATH_TRANSLATED", (location_conf.get_root() + req_builder.get_path()).c_str(), 1);

    setenv("REQUEST_URI", req_builder.get_uri().c_str(), 1);
    setenv("REQUEST_METHOD", req_builder.get_method().c_str(), 1);
    if (!req_builder.get_query_string().empty()) {
        setenv("QUERY_STRING", req_builder.get_query_string().c_str(), 1);
    }

    setenv("SCRIPT_FILENAME", file.c_str(), 1);
    setenv("SCRIPT_NAME", get_file_basename(file).c_str(), 1);

    setenv("CONTENT_TYPE", req_builder.get_header("Content-Type").c_str(), 1);
    setenv("CONTENT_LENGTH", req_builder.get_header("Content-Length").c_str(), 1);

    setenv("DOCUMENT_ROOT", location_conf.get_root().c_str(), 1);

    if (!req_builder.get_header("Host").empty()) {
        setenv("SERVER_NAME", req_builder.get_header("Host").c_str(), 1);
    }
    setenv("SERVER_ADDR", inet_ntoa(c.get_local_addr()), 1);
    setenv("SERVER_PORT", server_port.c_str(), 1);
    setenv("SERVER_SOFTWARE", constants::SERVER_NAME_VERSION.c_str(), 1);

    setenv("REMOTE_ADDR", inet_ntoa(c.get_remote_addr()), 1);
    setenv("REMOTE_PORT", remote_port.c_str(), 1);

    const std::map<std::string, std::string> &headers = req_builder.get_headers();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        std::string key = "HTTP_";
        std::transform(it->first.begin(), it->first.end(), std::back_inserter(key), update_header_key);
        setenv(key.c_str(), it->second.c_str(), 1);
    }
}

std::string utilities::get_mime_type(const std::string &file) {
    constants::mime_types_map::const_iterator it = constants::MIME_TYPES.find(get_file_extension(file));
    return it == constants::MIME_TYPES.end() ? constants::MIME_TYPES.at("") : it->second;
}

std::string utilities::create_error_page(int status) {
    std::stringstream str_stream;
    str_stream << status << " " << constants::STATUS_STR.at(status);

    return "<html>\r\n"
               "\t<head>\r\n"
                    "\t\t<title>" + str_stream.str() + "</title>\r\n"
               "\t</head>\r\n"
               "\t<body>\r\n"
                    "\t\t<center><h1>" + str_stream.str() + "</h1></center>\r\n"
                    "\t\t<hr>\r\n"
                    "\t\t<center>" + constants::SERVER_NAME_VERSION + "</center>\r\n"
               "\t</body>\r\n"
           "</html>";
}

void utilities::on_error(int status, const std::string &error_page, response_builder &res_builder) {
    if (access(error_page.c_str(), F_OK | R_OK)) {
        res_builder.set_status(status)
                .set_body(create_error_page(status));
        return;
    }
    std::ifstream error_file(error_page.c_str());
    res_builder.set_status(status)
            .append_body(error_file);
}

void utilities::truncate_body(request_builder &req_builder, response_builder &res_builder, std::ifstream &f) {
    std::string range = req_builder.get_header("Range");
    if (range.find('=') != std::string::npos) {
        range = range.substr(range.find('=') + 1);
    }
    f.clear();
    f.seekg(0, std::ios_base::end);
    long long file_size = f.tellg();
    long long first_byte;
    long long last_byte = file_size - 1;
    bool first_missing = range[0] == '-';
    bool last_missing = *range.rbegin() == '-';

    if (!first_missing && !last_missing) {
        size_t idx = range.find('-');
        first_byte = strtoll(range.c_str(), NULL, 10);
        if (idx != std::string::npos) {
            last_byte = strtoll(range.c_str() + idx + 1, NULL, 10);
        }
    } else if (last_missing) {
        first_byte = strtoll(range.c_str(), NULL, 10);
    } else {
        size_t idx = range.find('-');
        if (idx != std::string::npos) {
            last_byte = strtoll(range.c_str() + idx + 1, NULL, 10);
        }
        first_byte = std::max(file_size - last_byte, 0ll);
        last_byte = file_size - 1;
    }
    std::stringstream range_stream;
    range_stream << "bytes " << first_byte << "-" << last_byte << "/" << file_size;
    res_builder.set_status(206)
            .set_header("Accept-Ranges", "bytes")
            .set_header("Content-Range", range_stream.str());
    res_builder.truncate_body(first_byte, last_byte);
}