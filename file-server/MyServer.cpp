#include "MyServer.h"
#include "../zip-stream/zip_headers.h"
#include "../utils.h"
#include "streamer_network.h"
#include "mime_type_detector.h"

const dlib::logger logan("L.server");

void write_header(std::ostream &out, outgoing_things &outgoing, const std::string &filename, uint64_t filesize) {
    logan << LTRACE << "write_header";
    logan << LDEBUG << "[Header] filename: " << filename << " size: " << filesize;

    outgoing.headers["Content-Type"] = mime_type_detector::get_mime_type(dlib::split_on_last(filename, ".").second);

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

        streamer_network sn(t, chunk_size);
    uint64 filesize;
    if (is_path_file(filename)) {
        logan << LDEBUG << "'" << filename << "' is file.";
        dlib::file file(filename);
        filesize = file.size();
        write_header(out, outgoing, file.name(), filesize);

        std::ifstream in(file.full_name(), std::ifstream::binary);
        sn.stream_data(in, out);

        in.close();
    } else {
        logan << LDEBUG << "'" << filename << "' is directory.";
        dlib::directory dir(filename);
        zip_streamer_network zsn(sn);
        zip_archive zip(dir, zsn);
        std::string zip_filename = dir.name() + ".zip";
        write_header(out, outgoing, zip_filename, zip.get_archive_size());
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

        if (is_prefix("/admin", incoming.path)) {
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

    if (is_prefix("/res/",incoming.path) || is_prefix("/fonts/",incoming.path)) {
        logan << LINFO << "Client requested resource " << incoming.path << " using GET";

        auto split = split_on_last(incoming.path, "/");
        std::string filename = split.second;
        //TODO add check that requested file is in CWD dlib::get_current_dir()
        //serve resource file
        try {
            dlib::file file("res/" + filename);
            logan << LINFO << "Serving " << file.full_name() << " to client";
            response.response = file.full_name();
            response.type = FILE_NAME;

        } catch (dlib::file::file_not_found &exc) {
            response.type = FILE_NOT_AVAILABLE;
        }
        return;
    }

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
                                            buffer(new char[chunk_size]), //TODO zmenit na vector
                                            t(config.max_speed, chunk_size),
                                            db(config.db_path),
                                            fileguard(db),
                                            debug(config.debug),
                                            admin(db, config) {}
