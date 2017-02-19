#include <dlib/dir_nav.h>
#include <dlib/server/server_http.h>
#include "admin_interface.h"
#include "dir_utils.h"
#include "../url-generator/url_generator.h"
#include "MyServer.h"

const dlib::logger logan("L.admin");

bool is_param_set(const std::string &param, dlib::key_value_map &request_data) {
    return request_data.find(param) != request_data.end();
}

std::string admin_interface::on_request(dlib::incoming_things &request, dlib::outgoing_things &things) {
    logan << LTRACE << "admin_interface::on_request";
    if (request.path == "/admin" && request.request_type == "GET") {
        logan << LINFO << "Client requested " << request.path << " using GET";
        //serve HTML file
        dlib::file file("file.html");
        std::ifstream t(file.full_name());
        std::stringstream buffer;
        buffer << t.rdbuf();


        std::string file_content = buffer.str();
        std::string needle("__GLOBAL_PATH__");
        std::string replacement(svr_cfg.root_path);
        size_t pos = 0;
        while ((pos = file_content.find(needle, pos)) != std::string::npos) {
            file_content.replace(pos, needle.length(), replacement);
            pos += replacement.length();
        }
        logan << LINFO << "Serving " << file.full_name() << " to client";
        return file_content;
    }

    if (request.path == "/admin" && request.request_type == "POST") {
        logan << LINFO << "Client requested " << request.path << " using POST";
        //generate URL for file
        options opt;

        opt.count_limit = (dlib::uint32) std::stoi(request.queries["count"]);
        opt.file_name = root_dir.full_name() + root_dir.get_separator() + request.queries["path"];
        opt.time_limit = parse_time(request.queries["expire"]);
        if (request.queries["type"] == "count") {
            opt.type = option_type::OPT_COUNTER;
        } else if (request.queries["type"] == "expire") {
            opt.type = option_type::OPT_TIMER;
        }

        logan << LDEBUG << "Client sent following data:\n" << opt;
        validate_option(opt);

        std::string file_id = register_file(db, opt, request.foreign_ip);
        logan << LINFO << "Registered file at URL: " << file_id;
        return file_id;
    }

    if (request.request_type == "GET" && is_param_set(std::string("path"), request.queries)) {
        logan << LINFO << "Client requested " << request.path << " using GET";
        // get params from GET request.queries
        std::string dir_name = root_dir.full_name() + root_dir.get_separator() + request.queries["path"];
        logan << LDEBUG << "Looking for directory " << dir_name;
        try {
            dlib::directory tmp(dir_name);

            if (tmp.full_name().compare(0, root_dir.full_name().size(), root_dir.full_name()) == 0) {
                logan << LINFO << "Serving content of " << dir_name << " as JSON.";
                return get_dir_contents_as_json(root_dir, tmp);
            } else {
                logan << LINFO << "Access to directory " << dir_name << " denied.";
                return "<img src=\"http://thevpnguy.com/wp-content/uploads/2016/01/access-denied.jpg\">";
            }
        } catch (dlib::directory::dir_not_found &e) {
            logan << LERROR << "Directory " << dir_name << " not found: " << e.what();
            return "directory not found";
        }
    }

    if (request.path == "/admin?links" && request.request_type == "GET") {
        clear_old_links();

        std::vector<file_record> files;
        get_list_registered_files(db, files);
        std::stringstream ss;
        ss << "[";
        for (auto it = files.begin(); it != files.end(); ++it) {
            it->to_json(ss);
            ss << ",";
        }
        // move one char back
        ss.seekp(-1, ss.cur);
        // replace last , with ]
        ss << "]";


        return ss.str();
    }

    std::string resources_prefix("/admin/res/");
    if (request.path.compare(0, resources_prefix.size(), resources_prefix) == 0) {
        logan << LINFO << "Client requested resource " << request.path << " using GET";

        std::string filename = request.path.substr(std::string("/admin/").length());
        //serve resource file
        try {

            dlib::file file(filename);
            std::ifstream t(file.full_name());
            std::stringstream buffer;
            buffer << t.rdbuf();
            //TODO fix mime type
            things.headers["Content-Type"] = "text/css";

            logan << LINFO << "Serving " << file.full_name() << " to client";
            return buffer.str();

        } catch (dlib::file::file_not_found &exc) {
            logan << LWARN << "Resource " << filename << " not found.";
            things.http_return = 404;
            things.http_return_status = "file not found";
            return "";
        }
    }

    logan << LERROR << "Client requested " << request.path << " using " << request.request_type
          << " and there is nothing here.";
    things.http_return = 500;
    things.http_return_status = "something's wrong";
    return "<img src=\"http://i.imgur.com/nkp8h.jpg\">";
}

admin_interface::admin_interface(dlib::database &database, server_config &svr_config)
        : db(database), root_dir(svr_config.root_path), svr_cfg(svr_config) {}

void admin_interface::clear_old_links() {
    dlib::statement st(db, "delete from `files` where `limit_type` = 'C' and`dl_counter` <= 0");
    st.exec();

    dlib::statement st1(db,
                        "delete from `files` where `limit_type` = 'T' and `limit_timestamp` - strftime('%s','now') <= 0");
    st1.exec();

}
