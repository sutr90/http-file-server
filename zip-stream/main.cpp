#include <dlib/dir_nav.h>
#include "zip_headers.h"

int main() {
    dlib::directory dir("D:\\a\\b");
    dlib::file file("D:\\testLog.log.gz");

    std::ofstream zip_stream("D:\\test_new.zip", std::ios::out | std::ios::binary);

    zip_archive zip(file);

    zip.stream(zip_stream);


//    zip_file f(file, "D:\\");
//
//    std::cout << f.zipname << std::endl;
//    std::cout << f.get_filesize() << std::endl;
//    std::cout << f.get_full_name() << std::endl;

    return 0;
}

