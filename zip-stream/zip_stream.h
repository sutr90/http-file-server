#ifndef PROJECT_ZIP_STREAM_H
#define PROJECT_ZIP_STREAM_H

#include <dlib/dir_nav.h>

void stream_dirfile(dlib::directory &obj, std::ostream &ostream);
void stream_dirfile(dlib::file &obj, std::ostream &ostream);

#include <iostream>
#include <c++/5.3.0/iosfwd>

template<class DIRFILE>
void stream_to_zip(DIRFILE &obj, std::ostream &ostream){
    stream_dirfile(obj, ostream);
}

#endif //PROJECT_ZIP_STREAM_H
