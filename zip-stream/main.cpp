#include <dlib/dir_nav.h>
#include "zip_headers.h"

int main() {
    dlib::file file("D:\\a\\a.txt");
    dlib::directory dir("D:\\a");
    dlib::file file2("D:\\b.txt");

    std::ofstream zip_stream("D:\\test_new.zip", std::ios::out | std::ios::binary);

    zip_file f(file, "D:\\");
    zip_file f2(file2, "D:\\");

    zip_archive zip(dir);
//    zip.add(f);
//    zip.add(f2);

    zip.stream(zip_stream);


//    zip_file f(file, "D:\\");
//
//    std::cout << f.zipname << std::endl;
//    std::cout << f.get_filesize() << std::endl;
//    std::cout << f.get_full_name() << std::endl;

    return 0;
}

