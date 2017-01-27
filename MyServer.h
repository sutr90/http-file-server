//
// Created by VF on 24. 1. 2017.
//

#ifndef SERVER_MYSERVER_H
#define SERVER_MYSERVER_H

#include <dlib/server.h>
#include <dlib/dir_nav.h>
#include <iostream>
#include <string>

using namespace dlib;

enum response_type {
    FILE_NAME,
    STRING,
    ERR
};

struct response {
    std::string response;
    response_type type;
};

class MyServer : public server_http {
private:
    const std::string on_request(
            const incoming_things &incoming,
            outgoing_things &outgoing
    ) { return ""; };

    void on_request(
            const incoming_things &incoming,
            outgoing_things &outgoing,
            response &response
    );


    void stream_http_response(std::ostream &out, outgoing_things outgoing, std::string &filename);

    virtual void on_connect(
            std::istream &in,
            std::ostream &out,
            const std::string &foreign_ip,
            const std::string &local_ip,
            unsigned short foreign_port,
            unsigned short local_port,
            uint64
    );
};

#endif //SERVER_MYSERVER_H
