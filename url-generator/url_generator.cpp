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

void unregister_file(dlib::database &db, options &opt) {
    dlib::statement st(db, "delete from `files` where `file_id` = ? COLLATE NOCASE");
    st.bind(1, opt.file_name);
    st.exec();
}

