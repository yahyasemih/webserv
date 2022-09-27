#ifndef DIRECTORYLISTINGPAGEBUILDER_H
#define DIRECTORYLISTINGPAGEBUILDER_H

#pragma once

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#include <string>
#include <fstream>
#include <iostream>

#include <response_builder.hpp>
#include <constants.hpp>

class directory_listing_page_builder
{
public:
    directory_listing_page_builder(std::string directory, const std::string &root);
    ~directory_listing_page_builder();
    void add_directory_path();
    void add_new_table_entry(const std::string &file_name, const std::string &size, const std::string &date);
    void add_parent_directory_path();
    std::string list_directory();
    static std::string get_file_last_modified_date(struct timespec &ts);
    static std::string get_file_readable_size(off_t size);
    std::string template_content;

private:
    void read_file();
    std::string route;
    std::string directory_name;
    std::string directory_path;
    std::string template_file_path;
};

#endif