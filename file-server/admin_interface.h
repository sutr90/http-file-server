

#ifndef PROJECT_ADMIN_H
#define PROJECT_ADMIN_H


#include <dlib/server.h>

class admin_interface {
    friend class MyServer;
private:
    void on_request(dlib::incoming_things &things);
};


#endif //PROJECT_ADMIN_H
