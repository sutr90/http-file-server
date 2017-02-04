//
// Created by VF on 4. 2. 2017.
//

#include "zip_headers.h"

void zip_archive::add(dlib::file &file) {
    files.emplace_back(local_file_header(file));
}
