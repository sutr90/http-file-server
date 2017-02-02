

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


        std::string file_content = buffer.str();
        std::string needle("__GLOBAL_PATH__");
        std::string replacement(root_dir.full_name() + "/");
        size_t pos = 0;
        while ((pos = file_content.find(needle, pos)) != std::string::npos) {
            file_content.replace(pos, needle.length(), replacement);
            pos += replacement.length();
        }

        return file_content;
    }

    if (request.path == "/admin" && request.request_type == "POST") {
        //generate URL for file
        options opt;

        opt.count_limit = (dlib::uint32) std::stoi(request.queries["count"]);
        opt.file_name = root_dir.full_name() + "/" + request.queries["path"];
        opt.time_limit = parse_time(request.queries["expire"]);
        if (request.queries["type"] == "count") {
            opt.type = option_type::OPT_COUNTER;
        } else if (request.queries["type"] == "expire") {
            opt.type = option_type::OPT_TIMER;
        }

        validate_option(opt);

        register_file(db, opt);
        return "URL for file is XYZ";
    }

    if (request.request_type == "GET") {
        // get params from GET request.queries
        try {

            dlib::directory tmp(root_dir.full_name() + "/" + request.queries["path"]);

            if (tmp.full_name().compare(0, root_dir.full_name().size(), root_dir.full_name()) == 0) {
                return get_dir_contents_as_json(root_dir, tmp);
            } else {
                return "<img src=\"http://thevpnguy.com/wp-content/uploads/2016/01/access-denied.jpg\">";
            }
        } catch (dlib::directory::dir_not_found &e) {
            //TODO add the path in debug mode
            return "directory not found";
        }

    }

    things.http_return = 500;
    things.http_return_status = "something's wrong";
    return "<img src=\"http://i.imgur.com/nkp8h.jpg\">";
}

admin_interface::admin_interface(dlib::database &database, dlib::directory &rd) : db(database), root_dir(rd) {

}
