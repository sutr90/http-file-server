

#ifndef PROJECT_ADMIN_H
#define PROJECT_ADMIN_H


#include <dlib/server.h>

class admin_interface {
    friend class MyServer;
private:
    std::string on_request(dlib::incoming_things request, dlib::outgoing_things &things);
};


#endif //PROJECT_ADMIN_H
