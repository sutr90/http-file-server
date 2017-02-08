#include "utils.h"

#include <dlib/logger.h>

const dlib::logger logan("L.dir_utils");

bool is_path_file(std::string &path) {
    logan << dlib::LTRACE << "is_path_file";
    struct stat s;
    if (stat(path.c_str(), &s) == 0) {
        if (s.st_mode & S_IFREG) {
            return true;
        } else {
            return false;
        }
    } else {
        logan << dlib::LERROR << "Error when getting stat() of " << path;
        throw std::exception();
    }
}