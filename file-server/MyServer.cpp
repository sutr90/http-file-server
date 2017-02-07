#include "MyServer.h"
#include "dir_utils.h"
#include "../zip-stream/zip_headers.h"

const dlib::logger logan("L.server");

void write_header(std::ostream &out, outgoing_things &outgoing, const std::string &filename, uint64_t filesize) {
    logan << LTRACE << "write_header";
    logan << LDEBUG << "[Header] filename: " << filename << " size: " << filesize;

    outgoing.headers["Content-Type"] = "application/octet-stream";
    if (filesize > 0) {
        outgoing.headers["Content-Length"] = std::to_string(filesize);
    }
    outgoing.headers["Content-Transfer-Encoding"] = "binary";
    outgoing.headers["Content-Disposition"] = "attachment; filename=\"" + filename + "\"";
    out << "HTTP/1.0 " << outgoing.http_return << " " << outgoing.http_return_status << "\r\n";

    // Set any new headers
    for (key_value_map_ci::const_iterator ci = outgoing.headers.begin(); ci != outgoing.headers.end(); ++ci) {
        out << ci->first << ": " << ci->second << "\r\n";
    }
    out << "\r\n";
}

void MyServer::stream_http_response(std::ostream &out, outgoing_things &outgoing, std::string &filename) {
    logan << LTRACE << "MyServer::stream_http_response";
    logan << LDEBUG << "Preparing to stream " << filename;

    uint64 filesize;
    if (is_path_file(filename)) {
        logan << LDEBUG << "'" << filename << "' is file.";
        dlib::file file(filename);
        filesize = file.size();
        write_header(out, outgoing, file.name(), filesize);

        std::ifstream in(file.full_name(), std::ifstream::binary);
        uint64 current = 0;

        while (current < filesize && out.good()) {
            in.read(buffer.get(), chunk_size);
            std::streamsize bytes = in.gcount();
            out.write(buffer.get(), bytes);
            current += bytes;
            t.sleep();
        }

        in.close();
    } else {
        logan << LDEBUG << "'" << filename << "' is directory.";
        dlib::directory dir(filename);
        zip_archive zip(dir);
        std::string zip_filename = dir.name() + ".zip";
        write_header(out, outgoing, zip_filename, 0);
        zip.stream(out);
    }
}

void
MyServer::on_connect(std::istream &in, std::ostream &out, const std::string &foreign_ip, const std::string &local_ip,
                     unsigned short foreign_port, unsigned short local_port, uint64) {
    try {
        logan << LTRACE << "MyServer::on_connect";
        logan << LINFO << "Client connected: " << foreign_ip;
        incoming_things incoming(foreign_ip, local_ip, foreign_port, local_port);
        outgoing_things outgoing;

        parse_http_request(in, incoming, get_max_content_length());
        read_body(in, incoming);

        logan << LINFO << "Client requested path: " << incoming.path;

        std::string admin_prefix("/admin");
        //incoming.path[0:prefix.len()] == prefix
        if (incoming.path.compare(0, admin_prefix.size(), admin_prefix) == 0) {
            std::string response = admin.on_request(incoming, outgoing);
            write_http_response(out, outgoing, response);
        } else {
            response response;
            on_request(incoming, outgoing, response);
            if (response.type == STRING) {
                write_http_response(out, outgoing, response.response);
            } else if (response.type == FILE_NAME) {
                stream_http_response(out, outgoing, response.response);
            } else if (response.type == FILE_NOT_AVAILABLE) {
                outgoing.http_return = 404;
                outgoing.http_return_status = response.response;
                write_http_response(out, outgoing, response.response);
            }
        }
    }
    catch (std::exception &e) {
        logan << LERROR << "MyServer::on_connect: " << e.what();
        write_http_response(out, e);
    }
}

void MyServer::on_request(const incoming_things &incoming, outgoing_things &, response &response) {
    logan << LTRACE << "MyServer::on_request";
    if (incoming.path != "/") {
        logan << LINFO << "Client requested download of file " << incoming.path;
        fileguard.get_file(incoming.path, response);
        return;
    }
    logan << LINFO << "Client requested root: " << incoming.path;
    response.type = STRING;
    response.response = "<html> <body> nothing to see here </body> </html>";
}

MyServer::MyServer(server_config &config) : chunk_size(config.chunk_size),
                                            buffer(new char[chunk_size]),
                                            t(config.max_speed, chunk_size),
                                            db(config.db_path),
                                            fileguard(db),
                                            debug(config.debug),
                                            admin(db, config) {}
