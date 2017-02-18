#include <dlib/sqlite/sqlite.h>
#include "url_generator.h"
#include "names.h"

using namespace dlib;
using namespace std;

std::string generate_file_id(database &db) {
    bool name_exist;
    string file_id;

    int counter = 0;

    do {
        counter++;

        file_id = names::get_random_name();
        if (counter > 20) {
            file_id += to_string(time(NULL));
        }

        dlib::statement st(db, "select `files`.`file_id` from `files` where `files`.`file_id` = ? COLLATE NOCASE");
        st.bind(1, file_id);
        st.exec();
        name_exist = st.move_next();
    } while (name_exist);

    return file_id;
}

std::string register_file(database &db, options &opt) {
    string file_id = generate_file_id(db);

    dlib::statement st(db, "insert into `files` values (?, ?, ?, ?, strftime('%s','now')+?)");
    st.bind(1, file_id);
    st.bind(2, opt.file_name);

    if (opt.type == option_type::OPT_COUNTER) {
        st.bind_text(3, "C");
    } else if (opt.type == option_type::OPT_TIMER) {
        st.bind_text(3, "T");
    }

    st.bind(4, opt.count_limit);
    st.bind(5, opt.time_limit);
    st.exec();

    return file_id;
}

void list_registered_files(dlib::database &db) {
    std::vector<file_record> files;
    get_list_registered_files(db, files);

    cout << "file_id | path | remaining" << endl;

    for (auto it = files.begin(); it != files.end(); ++it) {
        cout << it->file_id << " | ";
        cout << it->path << " | ";
        if (it->limit_type == "C") {
            cout << it->dl_counter << " downloads";
        } else {
            if (it->time_limit <= 0) {
                cout << "0 minutes";
            } else {
                cout << it->time_limit / 60 << " minutes";
            }
        }

        cout << endl;
    }

}

void unregister_file(dlib::database &db, options &opt) {
    dlib::statement st(db, "delete from `files` where `file_id` = ? COLLATE NOCASE");
    st.bind(1, opt.file_name);
    st.exec();
}

void get_list_registered_files(dlib::database &db, std::vector<file_record> &files) {
    dlib::statement st(db,
                       "select `file_id` , `file_path` , `limit_type` , `dl_counter` , `limit_timestamp` - strftime('%s','now') from files");
    st.exec();


    while (st.move_next()) {
        file_record fr;
        st.get_column(0, fr.file_id);
        st.get_column(1, fr.path);
        st.get_column(2, fr.limit_type);
        st.get_column(3, fr.dl_counter);
        st.get_column(4, fr.time_limit);

        files.push_back(fr);
    }
}

void file_record::to_json(std::stringstream &ss) {
    ss << "{";
    ss << "\"file_id\":\"" << file_id << "\",";
    ss << "\"path\":\"" << path << "\",";
    ss << "\"limit_type\":\"" << limit_type << "\",";
    ss << "\"dl_counter\":\"" << dl_counter << "\",";
    ss << "\"time_limit\":\"" << time_limit << "\"";
    ss << "}";
}
