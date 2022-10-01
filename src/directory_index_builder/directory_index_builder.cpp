#include "directory_index_builder.hpp"

directory_index_builder::directory_index_builder(const std::string &directory_path,
        const std::string &uri) {
    this->template_file_path = "src/directory_index_builder/directory_index_template.html";
    this->directory_path = directory_path;
    this->read_file();
    this->uri = uri;
    if (*this->uri.rbegin() != '/') {
        this->uri += "/";
    }
}

std::string directory_index_builder::list_directory() {
    DIR *directory = opendir(this->directory_path.c_str());
    if (directory == NULL) {
        return "";
    }

    struct dirent *dirent_struct;
    // Add directory path to the head of the page.
    this->add_directory_path();
    // Add parent directory path to [parent directory]
    this->add_parent_directory_path();

    while ((dirent_struct = readdir(directory))) {
        if (strcmp(dirent_struct->d_name, "..") == 0 || strcmp(dirent_struct->d_name, ".") == 0)
            continue;
        struct stat file_stat = {};
        std::string full_path = this->directory_path + dirent_struct->d_name;
        std::string size;
        // if we can't get the required information about the file, we will display its name and leave the other
        // two columns empty
        if (stat(full_path.c_str(), &file_stat) == -1) {
            add_new_table_entry(dirent_struct->d_name, "-", "-");
            continue;
        }
        // We don't need size for a directory, we only display size for files.
        if (file_stat.st_mode & S_IFDIR)
            size = "-";
        else
            size = utilities::get_file_readable_size(file_stat.st_size);
        // Add file information to page.
        add_new_table_entry(dirent_struct->d_name, size,
                utilities::get_file_last_modified_date(file_stat.st_mtimespec));
    }
    closedir(directory);
    return template_content;
}

void directory_index_builder::read_file() {
    std::ifstream file(this->template_file_path.c_str());
    std::string line;

    while (std::getline(file, line)) {
        this->template_content += line;
        if (file.good()) {
            this->template_content += "\n";
        }
    }
}

void directory_index_builder::add_directory_path() {
    std::string to_replace = "{directory_path}";
    size_t index = this->template_content.find(to_replace);
    while (index != std::string::npos) {
        this->template_content.replace(index, to_replace.size(), this->uri);
        index = this->template_content.find(to_replace);
    }
}

void directory_index_builder::add_parent_directory_path() {
    std::string parent;
    std::string to_replace = "{parent_link}";
    if (this->uri == "/")
        parent = "/";
    else {
        // remove the last directory from uri to get to the parent directory.
        std::string tmp(this->uri);
        tmp.erase(tmp.rfind('/'));
        parent = tmp.substr(0, tmp.rfind('/') + 1);
    }
    size_t index = this->template_content.find(to_replace);
    if (index != std::string::npos) {
        this->template_content.replace(index, to_replace.size(), parent);
    }
}

void directory_index_builder::add_new_table_entry(const std::string &file_name, const std::string &size,
        const std::string &date) {
    std::string to_find = "</table>";
    size_t index = this->template_content.find(to_find);
    if (index == std::string::npos || index + to_find.size() >= this->template_content.size()) {
        return;
    }
    std::string pre = this->template_content.substr(0, index);
    std::string post = this->template_content.substr(index, this->template_content.size());
    std::string url = this->uri;
    if (url[0] != '/') {
        url = "/" + url;
    }
    if (*url.rbegin() != '/') {
        url += "/";
    }

    pre += "\n<tr>\n"
        "<td valign=\"top\">&nbsp;</td>"
        "<td><a href='" + url + file_name + "'>" + file_name + "</a></td>\n"
        "<td align=\"right\">" + size + "</td>\n"
        "<td align=\"right\">" + date + "</td>\n"
        "<td>&nbsp;</td>"
        "</tr>\n";
    pre += post;
    this->template_content = pre;
}
