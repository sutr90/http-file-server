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
    STRING
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

    virtual const void on_request(
            const incoming_things &incoming,
            outgoing_things &outgoing,
            response &response
    ) = 0;


    void stream_http_response(std::ostream &out, outgoing_things outgoing, std::string &filename) {
        dlib::file file(filename);
        uint64 filesize = file.size();

        outgoing.headers["Content-Type"] = "application/force-download";
        outgoing.headers["Content-Length"] = std::to_string(filesize);
        outgoing.headers["Content-Transfer-Encoding"] = "binary";
        outgoing.headers["Content-Disposition"] = "attachment; filename=\"" + file.name() + "\"";


        out << "HTTP/1.0 " << outgoing.http_return << " " << outgoing.http_return_status << "\r\n";

        // Set any new headers
        for(key_value_map_ci::const_iterator ci = outgoing.headers.begin(); ci != outgoing.headers.end(); ++ci )
        {
            out << ci->first << ": " << ci->second << "\r\n";
        }
        out << "\r\n";

        const int chunk_size = 4096;
        char memblock[chunk_size];
        std::ifstream in(file.full_name(), std::ifstream::binary);
        uint64 current = 0;

        while (current < filesize) {
            in.read(memblock, chunk_size);
            std::streamsize bytes = in.gcount();
            out.write(memblock, bytes);
            current += bytes;
        }

        in.close();
    }

    virtual void on_connect(
            std::istream &in,
            std::ostream &out,
            const std::string &foreign_ip,
            const std::string &local_ip,
            unsigned short foreign_port,
            unsigned short local_port,
            uint64
    ) {
        try {
            incoming_things incoming(foreign_ip, local_ip, foreign_port, local_port);
            outgoing_things outgoing;

            parse_http_request(in, incoming, get_max_content_length());
            read_body(in, incoming);
            response response;
            on_request(incoming, outgoing, response);
            if (response.type == STRING) {
                write_http_response(out, outgoing, response.response);
            } else if (response.type == FILE_NAME) {
                stream_http_response(out, outgoing, response.response);
            }
        }
        catch (http_parse_error &e) {
            write_http_response(out, e);
        }
        catch (std::exception &e) {
            write_http_response(out, e);
        }
    }
};

#endif //SERVER_MYSERVER_H
