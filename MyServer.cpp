//
// Created by VF on 24. 1. 2017.
//

#include "MyServer.h"

void MyServer::stream_http_response(std::ostream &out, outgoing_things outgoing, std::string &filename) {
    dlib::file file(filename);
    uint64 filesize = file.size();

    outgoing.headers["Content-Type"] = "application/force-download";
    outgoing.headers["Content-Length"] = std::to_string(filesize);
    outgoing.headers["Content-Transfer-Encoding"] = "binary";
    outgoing.headers["Content-Disposition"] = "attachment; filename=\"" + file.name() + "\"";


    out << "HTTP/1.0 " << outgoing.http_return << " " << outgoing.http_return_status << "\r\n";

    // Set any new headers
    for (key_value_map_ci::const_iterator ci = outgoing.headers.begin(); ci != outgoing.headers.end(); ++ci) {
        out << ci->first << ": " << ci->second << "\r\n";
    }
    out << "\r\n";

    const int chunk_size = 64*1024;
    char memblock[chunk_size];
    std::ifstream in(file.full_name(), std::ifstream::binary);
    uint64 current = 0;

    while (current < filesize && out.good()) {
        in.read(memblock, chunk_size);
        std::streamsize bytes = in.gcount();
        out.write(memblock, bytes);
        current += bytes;
        sleep(500);
        std::cout << "sending";
    }

    in.close();
}

void MyServer::on_connect(
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

void MyServer::on_request(
        const incoming_things &incoming,
        outgoing_things &outgoing,
        response &response
) {
    if (incoming.path == "/file_test") {
        response.type = FILE_NAME;
        response.response = "D:\\keras.tar.gz";
        return;
    }

    using namespace std;
    ostringstream sout;
    // We are going to send back a page that contains an HTML form with two text input fields.
    // One field called name.  The HTML form uses the post method but could also use the get
    // method (just change method='post' to method='get').
    sout << " <html> <body> "
         << "<form action='/form_handler' method='post'> "
         << "User Name: <input name='user' type='text'><br>  "
         << "User password: <input name='pass' type='text'> <input type='submit'> "
         << " </form>";

    // Write out some of the inputs to this request so that they show up on the
    // resulting web page.
    sout << "<br>  path = ";
    sout << incoming.path << endl;
    sout << "<br>  request_type = ";
    sout << incoming.request_type << endl;
    sout << "<br>  content_type = ";
    sout << incoming.content_type << endl;
    sout << "<br>  protocol = ";
    sout << incoming.protocol << endl;
    sout << "<br>  foreign_ip = ";
    sout << incoming.foreign_ip << endl;
    sout << "<br>  foreign_port = ";
    sout << incoming.foreign_port << endl;
    sout << "<br>  local_ip = ";
    sout << incoming.local_ip << endl;
    sout << "<br>  local_port = " << incoming.local_port << endl;
    sout << "<br>  body = \"";
    sout << incoming.body << "\"" << endl;


    // If this request is the result of the user submitting the form then echo back
    // the submission.
    if (incoming.path == "/form_handler") {
        sout << "<h2> Stuff from the query string </h2>" << endl;
        sout << "<br>  user = ";
        sout << incoming.queries["user"] << endl;
        sout << "<br>  pass = ";
        sout << incoming.queries["pass"] << endl;

        // save these form submissions as cookies.
        outgoing.cookies["user"] = incoming.queries["user"];
        outgoing.cookies["pass"] = incoming.queries["pass"];
    }


    // Echo any cookies back to the client browser
    sout << "<h2>Cookies the web browser sent to the server</h2>";
    for (key_value_map::const_iterator ci = incoming.cookies.begin(); ci != incoming.cookies.end(); ++ci) {
        sout << "<br/>";
        sout << ci->first << " = " << ci->second << endl;
    }

    sout << "<br/><br/>";

    sout << "<h2>HTTP Headers the web browser sent to the server</h2>";
    // Echo out all the HTTP headers we received from the client web browser
    for (key_value_map_ci::const_iterator ci = incoming.headers.begin(); ci != incoming.headers.end(); ++ci) {
        sout << "<br/>";
        sout << ci->first << ": " << ci->second << endl;
    }

    sout << "</body> </html>";

    response.type = STRING;
    response.response = sout.str();
}