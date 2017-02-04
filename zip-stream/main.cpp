#include <dlib/dir_nav.h>
#include "zip_stream.h"

int main() {
    dlib::directory dir("D:");
    dlib::file file("D:\\outbanka.txt");

    std::ofstream ssss("D:\\outbanka.txt");

    stream_to_zip(dir, ssss);
    stream_to_zip(file, ssss);

    return 0;
}
