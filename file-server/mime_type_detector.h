//
// Created by vf on 19.2.17.
//

#ifndef PROJECT_MIME_TYPE_DETECTOR_H
#define PROJECT_MIME_TYPE_DETECTOR_H

#include <map>

class mime_type_detector {
private:
    std::map<std::string, std::string> ext_to_mime;
public:
    mime_type_detector();
    std::string get_mime_type(const std::string &extension);
};


#endif //PROJECT_MIME_TYPE_DETECTOR_H
