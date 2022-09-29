#ifndef WEBSERV_DIRECTORY_INDEX_BUILDER_HPP
#define WEBSERV_DIRECTORY_INDEX_BUILDER_HPP

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include "constants.hpp"
#include "response_builder.hpp"

class directory_index_builder {
public:
    directory_index_builder(const std::string &directory, const std::string &root);

    void add_directory_path();
    void add_new_table_entry(const std::string &file_name, const std::string &size, const std::string &date);
    void add_parent_directory_path();
    std::string list_directory();

    static std::string get_file_last_modified_date(struct timespec &ts);
    static std::string get_file_readable_size(off_t size);

private:
    void read_file();

    std::string route;
    std::string directory_path;
    std::string template_file_path;
    std::string template_content;
};

#endif //WEBSERV_DIRECTORY_INDEX_BUILDER_HPP