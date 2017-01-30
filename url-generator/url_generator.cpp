#include <dlib/sqlite/sqlite.h>
#include "url_generator.h"
#include "names.h"

using namespace dlib;
using namespace std;

std::string generate_file_id(database &db) {
    bool name_exist;
    string file_id;
    do {
        file_id = names::get_random_name();
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
    dlib::statement st(db,
                       "select `file_id` , `file_path` , `limit_type` , `dl_counter` , `limit_timestamp` - strftime('%s','now') from files");
    st.exec();

    cout << "file_id | path | remaining" << endl;

    while (st.move_next()) {
        string file_id, path, limit_type;
        int dl_counter, time_limit;
        st.get_column(0, file_id);
        st.get_column(1, path);
        st.get_column(2, limit_type);
        st.get_column(3, dl_counter);
        st.get_column(4, time_limit);

        cout << file_id << " | ";
        cout << path << " | ";
        if (limit_type == "C") {
            cout << dl_counter << " downloads";
        } else {
            if (time_limit <= 0) {
                cout << "0 minutes";
            } else {
                cout << time_limit / 60 << " minutes";
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

