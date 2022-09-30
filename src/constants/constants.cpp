//
// Created by Yahya Ez-zainabi on 9/24/22.
//

#include "constants.hpp"

const std::string constants::SERVER_NAME_VERSION = "leet-server/1.0.0 (UNIX)";

const std::string constants::HTTP_VERSION = "HTTP/1.1";

const std::string constants::CGI_VERSION = "CGI/1.1";

static std::map<int, std::string> init_status_map() {
    std::map<int, std::string> map;

    // Informational 1xx
    map.insert(std::make_pair(100, "Continue"));
    map.insert(std::make_pair(101, "Switching Protocols"));
    // Successful 2xx
    map.insert(std::make_pair(200, "OK"));
    map.insert(std::make_pair(201, "Created"));
    map.insert(std::make_pair(202, "Accepted"));
    map.insert(std::make_pair(203, "Non-Authoritative Information"));
    map.insert(std::make_pair(204, "No Content"));
    map.insert(std::make_pair(205, "Reset Content"));
    map.insert(std::make_pair(206, "Partial Content"));
    // Redirection 3xx
    map.insert(std::make_pair(300, "Multiple Choices"));
    map.insert(std::make_pair(301, "Moved Permanently"));
    map.insert(std::make_pair(302, "Found"));
    map.insert(std::make_pair(303, "See Other"));
    map.insert(std::make_pair(304, "Not Modified"));
    map.insert(std::make_pair(305, "Use Proxy"));
    map.insert(std::make_pair(306, "(Unused)")); // Unused
    map.insert(std::make_pair(307, "Temporary Redirect"));
    map.insert(std::make_pair(308, "Permanent Redirect"));
    // Client Error 4xx
    map.insert(std::make_pair(400, "Bad Request"));
    map.insert(std::make_pair(401, "Unauthorized"));
    map.insert(std::make_pair(402, "Payment Required"));
    map.insert(std::make_pair(403, "Forbidden"));
    map.insert(std::make_pair(404, "Not Found"));
    map.insert(std::make_pair(405, "Method Not Allowed"));
    map.insert(std::make_pair(406, "Not Acceptable"));
    map.insert(std::make_pair(407, "Proxy Authentication Required"));
    map.insert(std::make_pair(408, "Request Timeout"));
    map.insert(std::make_pair(409, "Conflict"));
    map.insert(std::make_pair(410, "Gone"));
    map.insert(std::make_pair(411, "Length Required"));
    map.insert(std::make_pair(412, "Precondition Failed"));
    map.insert(std::make_pair(413, "Request Entity Too Large"));
    map.insert(std::make_pair(414, "Request-URI Too Long"));
    map.insert(std::make_pair(415, "Unsupported Media Type"));
    map.insert(std::make_pair(416, "Requested Range Not Satisfiable"));
    map.insert(std::make_pair(417, "Expectation Failed"));
    map.insert(std::make_pair(418, "I'm a teapot"));
    map.insert(std::make_pair(422, "Unprocessable Entity"));
    map.insert(std::make_pair(425, "Too Early"));
    map.insert(std::make_pair(426, "Upgrade Required"));
    map.insert(std::make_pair(428, "Precondition Required"));
    map.insert(std::make_pair(429, "Too Many Requests"));
    map.insert(std::make_pair(431, "Request Header Fields Too Large"));
    map.insert(std::make_pair(451, "Unavailable For Legal Reasons"));
    // Server Error 5xx
    map.insert(std::make_pair(500, "Internal Server Error"));
    map.insert(std::make_pair(501, "Not Implemented"));
    map.insert(std::make_pair(502, "Bad Gateway"));
    map.insert(std::make_pair(503, "Service Unavailable"));
    map.insert(std::make_pair(504, "Gateway Timeout"));
    map.insert(std::make_pair(505, "HTTP Version Not Supported"));
    map.insert(std::make_pair(506, "Variant Also Negotiates"));
    map.insert(std::make_pair(507, "Insufficient Storage"));
    map.insert(std::make_pair(508, "Loop Detected"));
    map.insert(std::make_pair(510, "Not Extended"));
    map.insert(std::make_pair(511, "Network Authentication Required"));

    return map;
}

static std::map<std::string, std::string> init_mime_types() {
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

static std::set<std::string> init_valid_scopes() {
    std::set<std::string> valid_scopes;

    valid_scopes.insert("main");
    valid_scopes.insert("http");
    valid_scopes.insert("server");
    valid_scopes.insert("location");

    return valid_scopes;
}

static std::set<std::string> init_valid_methods() {
    std::set<std::string> valid_methods;

    valid_methods.insert("GET");
    valid_methods.insert("POST");
    valid_methods.insert("DELETE");
    valid_methods.insert("PUT");

    return valid_methods;
}

const constants::status_map constants::STATUS_STR = init_status_map();

const constants::mime_types_map constants::MIME_TYPES = init_mime_types();

const std::set<std::string> constants::VALID_SCOPES = init_valid_scopes();

const std::set<std::string> constants::VALID_METHODS = init_valid_methods();

const size_t constants::BUFFER_SIZE;

const std::time_t constants::CLIENT_TIMEOUT_SEC;
