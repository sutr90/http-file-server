
#include <dlib/dir_nav.h>

void stream_dirfile(dlib::directory &obj, std::ostream &ostream) {
std::cout << "streaming dir\n";
}

void stream_dirfile(dlib::file &obj, std::ostream &ostream) {
    std::cout << "streaming file\n";
}
