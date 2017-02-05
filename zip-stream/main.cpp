#include <dlib/dir_nav.h>
#include "zip_headers.h"
#include "zip_file.h"

int main() {
    dlib::file file("D:\\a\\a.txt");
//    dlib::directory file("D:\\a");
//    dlib::file file2("D:\\b.txt");

//    std::ofstream zip_stream("D:\\test_new.zip", std::ios::out | std::ios::binary);
//
//    zip_archive zip;
//    zip.add(file);
//    zip.add(file2);
//
//    zip.stream(zip_stream);


    zip_file f(file, "D:\\");

    std::cout << f.zipname << std::endl;
    std::cout << f.get_filesize() << std::endl;
    std::cout << f.get_full_name() << std::endl;

    return 0;
}

