#include "directory_listing_page_builder.h"
#include <fstream>
#include <iostream>

directory_listing_page_builder::directory_listing_page_builder(std::string directory, const std::string &root)
{
    this->template_file_path = "/Users/mobounya/Desktop/webserv/src/server/directory_listing_template.html";
    this->directory_path = directory;
    directory.erase(directory.end() - 1);
    this->directory_name = directory.substr(directory.rfind("/") + 1, directory.size());
    this->template_content.resize(200);
    this->read_file();
    this->route = this->directory_path.substr(root.size(), this->directory_path.size());
}

directory_listing_page_builder::~directory_listing_page_builder()
{

}

void directory_listing_page_builder::read_file() {
    std::fstream file;
    std::string line;

    file.open(this->template_file_path, std::ios::in);
    while (std::getline(file, line)) {
        this->template_content += line;
        if (file.good()) {
            this->template_content += "\n";
        }
    }
}

void directory_listing_page_builder::add_directory_path() {
    std::string to_replace = "{directory_path}";
    size_t index = this->template_content.find(to_replace);
    this->template_content.replace(index, to_replace.size(), this->directory_path);
}

void directory_listing_page_builder::add_parent_directory_path() {
    std::string parent;
    std::string to_replace = "{parent_link}";
    if (this->route == "/")
        parent = "/";
    else {
        // remove the last directory from route to get to the parent directory.
        std::string tmp(this->route);
        tmp.erase(tmp.rfind("/"));
        parent = tmp.substr(0, tmp.rfind("/") + 1);
    }
    size_t index = this->template_content.find(to_replace);
    this->template_content.replace(index, to_replace.size(), parent);
}

void directory_listing_page_builder::add_new_table_entry(const std::string &file_name, const std::string &size, const std::string &date) {
    size_t index = this->template_content.find("<tbody id='tbody'>");
    std::string pre = this->template_content.substr(0, index + 18);
    std::string post = this->template_content.substr(index + 18, this->template_content.length());
    pre += "\n<tr>\n"
    "<td><a href='" + this->route + file_name + "'>" + file_name + "</a></td>\n"
    "<td>" + size + "</td>\n"
    "<td>" + date + "</td>\n"
    "</tr>\n";
    pre += post;
    this->template_content = pre;
}
