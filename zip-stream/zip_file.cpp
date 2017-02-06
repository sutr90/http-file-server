#include "zip_file.h"

uint64_t zip_file::get_filesize() {
    return size;
}

std::string zip_file::generate_zip_name(dlib::file &df, const std::string &path_prefix) {
    std::string name = df.full_name();
    name.erase(0, path_prefix.length());
    std::replace(name.begin(), name.end(), '\\', '/'); // replace all 'x' to 'y'
    return name;
}