#include "directory_index_builder.hpp"

directory_index_builder::directory_index_builder(const std::string &directory_path,
                                                 const std::string &root) {
    this->template_file_path = "src/directory_index_builder/directory_index_template.html";
    this->directory_path = directory_path;
    this->read_file();
    this->route = this->directory_path.substr(root.size(), this->directory_path.size());
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
            add_new_table_entry(dirent_struct->d_name, "", "");
            continue;
        }
        // We don't need size for a directory, we only display size for files.
        if (file_stat.st_mode & S_IFDIR)
            size = "";
        else
            size = get_file_readable_size(file_stat.st_size);
        // Add file information to page.
        add_new_table_entry(dirent_struct->d_name, size, get_file_last_modified_date(file_stat.st_mtimespec));
    }
    closedir(directory);
    return template_content;
}

std::string directory_index_builder::get_file_last_modified_date(struct timespec &ts) {
    char buffer[100];
    strftime(buffer, sizeof buffer, "%D %r", gmtime(&ts.tv_sec));
    return std::string(buffer);
}

std::string directory_index_builder::get_file_readable_size(off_t size) {
    std::stringstream size_string;
    const long KiB = 1024;
    const long MiB = 1049000;
    const long GiB = 1074000000;

    if (size < KiB)
        size_string << size << " B";
    else if (size < MiB)
        size_string << (size / KiB) << " KB";
    else if (size < GiB)
        size_string << (size / MiB) << " MB";
    else
        size_string << (size / GiB) << " GB";
    return size_string.str();
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
    if (index != std::string::npos) {
        this->template_content.replace(index, to_replace.size(), this->directory_path);
    }
}

void directory_index_builder::add_parent_directory_path() {
    std::string parent;
    std::string to_replace = "{parent_link}";
    if (this->route == "/")
        parent = "/";
    else {
        // remove the last directory from route to get to the parent directory.
        std::string tmp(this->route);
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
