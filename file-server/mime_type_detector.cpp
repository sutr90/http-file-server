//
// Created by vf on 19.2.17.
//

#include <fstream>
#include <dlib/string.h>
#include "mime_type_detector.h"

mime_type_detector::mime_type_detector() {
    std::ifstream file("mime.types");
    std::string str;
    while (std::getline(file, str)) {
        if (dlib::trim(str)[0] == '#') {
            continue;
        }

        std::vector<std::string> parts = dlib::split(str);

        auto it = parts.begin();
        std::string mime = *it;
        for (++it; it != parts.end(); ++it) {
            ext_to_mime.insert({*it, mime});
        }
    }
}

std::string mime_type_detector::get_mime_type(const std::string &extension) {
    auto it = ext_to_mime.find(extension);
    if (it != ext_to_mime.end()){
        return it->second;
    }

    return "";
}
