

#ifndef PROJECT_ADMIN_H
#define PROJECT_ADMIN_H


#include <dlib/server.h>
#include <dlib/sqlite/sqlite.h>
#include <dlib/dir_nav.h>

class server_config;

class admin_interface {
    friend class MyServer;
private:
    std::string on_request(dlib::incoming_things &request, dlib::outgoing_things &things);
    dlib::database &db;
    dlib::directory root_dir;
    server_config &svr_cfg;
    void clear_old_links();
public:
    admin_interface(dlib::database &database, server_config &svr_config);
};


#endif //PROJECT_ADMIN_H
