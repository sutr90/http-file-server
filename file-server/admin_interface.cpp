

#include <dlib/dir_nav.h>
#include <dlib/server/server_http.h>
#include "admin_interface.h"
#include "dir_utils.h"
#include "../url-generator/url_generator.h"

std::string admin_interface::on_request(dlib::incoming_things request, dlib::outgoing_things &things) {
    if (request.path == "/admin" && request.request_type == "GET") {
        //serve HTML file
        dlib::file file("file.html");
        std::ifstream t(file.full_name());
        std::stringstream buffer;
        buffer << t.rdbuf();
        return buffer.str();
    }

    if (request.path == "/admin" && request.request_type == "POST") {
        //generate URL for file
        options opt;
        register_file(db, opt);
        return "URL for file is XYZ";
    }

    if (request.request_type == "GET") {
        // get params from GET request.queries

        return get_dir_contents_as_json("D:/dev");
    }

    things.http_return = 500;
    things.http_return_status = "something's wrong";
    return "error";
}

admin_interface::admin_interface(dlib::database &database) : db(database) {

}
