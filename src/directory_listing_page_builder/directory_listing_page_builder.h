#ifndef DIRECTORYLISTINGPAGEBUILDER_H
#define DIRECTORYLISTINGPAGEBUILDER_H

#pragma once
#include <string>

class directory_listing_page_builder
{
public:
    directory_listing_page_builder(std::string directory, const std::string &root);
    ~directory_listing_page_builder();
    void add_directory_path();
    void add_new_table_entry(const std::string &file_name, const std::string &size, const std::string &date);
    void add_parent_directory_path();
    
    std::string template_content;

private:
    void read_file();
    std::string route;
    std::string directory_name;
    std::string directory_path;
    std::string template_file_path;
};

#endif