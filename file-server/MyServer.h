#ifndef SERVER_MYSERVER_H
#define SERVER_MYSERVER_H

#include <dlib/server.h>
#include <iostream>
#include <string>
#include <dlib/dir_nav.h>
#include "file_guard.h"
#include "throttle.h"
#include "admin_interface.h"

using namespace dlib;

enum response_type {
    FILE_NAME,
    STRING,
    FILE_NOT_AVAILABLE
};

struct response {
    std::string response;
    response_type type;
};

class server_config {
public:
    std::string db_path;
    int port;
    uint32 chunk_size;
    float max_speed;
    bool debug;
    std::string root_path;
    std::string domain;

    server_config(config_reader &cr) :
            db_path(cr["db_path"]),
            port(get_option(cr, "server.port", 5000)),
            chunk_size((uint32) (get_option(cr, "server.chunk_size", 64) * 1024)),
            max_speed(get_option(cr, "server.max_speed", 10.f)),
            debug(get_option(cr, "server.debug", false)),
            root_path(get_option(cr, "server.root_dir", "")),
            domain(get_option(cr, "server.domain", "<error domain not set>")){}
};

class MyServer : public server_http {
public:
    MyServer(server_config &config);

    virtual ~MyServer() {}

private:
    uint32 chunk_size;
    std::unique_ptr<char[]> buffer;
    throttle t;
    database db;
    file_guard fileguard;
    bool debug;
    admin_interface admin;

    const std::string on_request(const incoming_things &, outgoing_things &) { return ""; };

    void on_request(const incoming_things &incoming, outgoing_things &, response &response);


    void stream_http_response(std::ostream &out, outgoing_things &outgoing, std::string &filename);

    virtual void on_connect(std::istream &in, std::ostream &out, const std::string &foreign_ip,
                            const std::string &local_ip, unsigned short foreign_port, unsigned short local_port,
                            uint64);
};

#endif //SERVER_MYSERVER_H
