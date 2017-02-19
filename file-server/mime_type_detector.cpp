//
// Created by vf on 19.2.17.
//

#include <fstream>
#include <dlib/string.h>
#include "mime_type_detector.h"

std::string mime_type_detector::get_mime_type(const std::string &extension) {
    if (!initialized) {
        init();
    }

    auto it = ext_to_mime.find(extension);
    if (it != ext_to_mime.end()) {
        return it->second;
    }

    return "";
}

void mime_type_detector::init() {
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

    mime_type_detector::initialized = true;
}

std::map<std::string, std::string> mime_type_detector::ext_to_mime;
bool mime_type_detector::initialized;