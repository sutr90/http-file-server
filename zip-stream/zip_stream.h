#ifndef PROJECT_ZIP_STREAM_H
#define PROJECT_ZIP_STREAM_H

#include <iostream>
#include <dlib/dir_nav.h>

void stream_dirfile(dlib::directory &directory, std::ostream &ostream);
void stream_dirfile(dlib::file &file, std::ostream &ostream);

template<typename DIRFILE>
void stream_to_zip(DIRFILE &obj, std::ostream &ostream){
    stream_dirfile(obj, ostream);
}

#endif //PROJECT_ZIP_STREAM_H
