#include <dlib/dir_nav.h>
#include "zip_stream.h"

int main() {
    dlib::directory dir("D:");
    dlib::file file("D:\\outbanka.txt");

    std::ofstream zip("D:\\test.zip", std::ios::out | std::ios::binary);

    stream_to_zip(dir, zip);
    stream_to_zip(file, zip);

    return 0;
}

