#include <dlib/dir_nav.h>
#include "zip_headers.h"

int main() {
    dlib::file file("D:\\a.txt");
    dlib::directory dir("d:\\localtexmf");
    std::ofstream zip_stream("D:\\test_new.zip", std::ios::out | std::ios::binary);

    zip_archive zip(dir);
    std::cout << zip.get_archive_size();
    zip.stream(zip_stream);


//    zip_file f(file, "D:\\");
//
//    std::cout << f.zipname << std::endl;
//    std::cout << f.get_filesize() << std::endl;
//    std::cout << f.get_full_name() << std::endl;

    return 0;
}

