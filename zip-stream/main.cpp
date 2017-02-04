#include <dlib/dir_nav.h>
#include "zip_headers.h"

int main() {
    dlib::file file("D:\\a.txt");
    dlib::file file2("D:\\b.txt");

    std::ofstream zip_stream("D:\\test_new.zip", std::ios::out | std::ios::binary);

    zip_archive zip;
    zip.add(file);
    zip.add(file2);

    zip.stream(zip_stream);

    return 0;
}

