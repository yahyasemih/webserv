//
// Created by Yahya Ez-zainabi on 9/28/22.
//

#include "logger.hpp"

logger::logger(const std::string &filename) : log_file(filename.c_str(), std::ios_base::app | std::ios_base::out) {
}

static std::string current_time() {
    time_t now = time(0);
    struct tm t_struct = {};
    char buf[80];

    t_struct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d/%b/%Y:%X %z", &t_struct);

    return buf;
}

void logger::access(const request_builder &req_builder, const response_builder &res_builder, const client &c) {
    log_file << inet_ntoa(c.get_remote_addr()) << " - " << "[" << current_time() << "] \"";
    log_file << req_builder.get_method() << " " << req_builder.get_uri() << " " << req_builder.get_http_version();
    log_file << "\" " << res_builder.get_status() << " " << res_builder.get_body().size();
    if (req_builder.get_headers().find("Referer") != req_builder.get_headers().end()) {
        log_file << " \"" << req_builder.get_header("Referer") << "\"";
    } else {
        log_file << " \"-\"";
    }
    if (req_builder.get_headers().find("User-Agent") != req_builder.get_headers().end()) {
        log_file << " \"" << req_builder.get_header("User-Agent") << "\"";
    } else {
        log_file << " \"-\"";
    }
    log_file << std::endl;
}

void logger::error(const std::string &error_message) {
    log_file << "[" << current_time() << "] [error] : " << error_message << std::endl;
}
