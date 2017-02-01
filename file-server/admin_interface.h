

#ifndef PROJECT_ADMIN_H
#define PROJECT_ADMIN_H


#include <dlib/server.h>
#include <dlib/sqlite/sqlite.h>
#include <dlib/dir_nav.h>

class admin_interface {
    friend class MyServer;
private:
    std::string on_request(dlib::incoming_things request, dlib::outgoing_things &things);
    dlib::database &db;
    dlib::directory &root_dir;

public:
    admin_interface(dlib::database &database, dlib::directory &root_dir);
};


#endif //PROJECT_ADMIN_H
