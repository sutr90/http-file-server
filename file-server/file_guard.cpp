#include <exception>
#include "file_guard.h"
#include "MyServer.h"
#include "../limit_type.h"
const dlib::logger logan("L.file_guard");

void file_guard::get_file(std::string url, response &response) {
    logan << LTRACE << "file_guard::get_file";
    std::size_t pos = url.find("/");
    std::string file_id = url.substr(pos + 1);

    //CREATE TABLE "files" ( `file_id` TEXT UNIQUE, `file_path` TEXT, `limit_type` TEXT DEFAULT 'C', `dl_counter` INTEGER DEFAULT 0, `limit_timestamp` INTEGER, PRIMARY KEY(`file_id`) )
    dlib::statement st2(db, "select `file_id` , `file_path` , `limit_type` , `dl_counter` , `limit_timestamp` from `files` where `files`.`file_id` = ? COLLATE NOCASE");
    st2.bind(1, file_id);
    st2.exec();

    response.type = FILE_NOT_AVAILABLE;
    response.response = file_id + " is not available anymore";

    if (!st2.move_next()) {
        logan << LINFO << "File " << file_id << " is not available anymore";
        return;
    }

    std::string file_path;
    char limit_type;
    int dl_counter;
    int64 limit_timestamp;

    file_path = st2.get_column_as_text(1);
    limit_type = st2.get_column_as_text(2)[0];
    dl_counter = st2.get_column_as_int(3);
    limit_timestamp = st2.get_column_as_int64(4);

    if (can_download(dl_counter, limit_timestamp, limit_type)) {// test_passed
        logan << LINFO << "Serving file " << file_path;
        response.response = file_path;
        response.type = FILE_NAME;

        if (limit_type == static_cast<char>(dl_limit_type::COUNTER)) {
            logan << LINFO << "Updating file " << file_id << " limits.";
            dlib::statement st(db, "update `files` set `dl_counter` = ? where `files`.`file_id` = ? COLLATE NOCASE");
            st.bind(1, dl_counter - 1);
            st.bind(2, file_id);
            st.exec();
        }
    } else {
        logan << LINFO << "Deleting file " << file_id << " from database.";
        dlib::statement st(db, "delete from `files` where `file_id` = ?");
        st.bind(1, file_id);
        st.exec();
    }
}

file_guard::file_guard(dlib::database &database) : db(database) {}

int64 file_guard::get_current_db_time() {
    logan << LTRACE << "file_guard::get_current_db_time";
    dlib::statement stmt(db, "select strftime('%s','now')");
    stmt.exec();
    if (stmt.move_next()) {
        return stmt.get_column_as_int64(0);
    }
    return INT64_MAX;
}

bool file_guard::can_download(int counter, dlib::int64 timestamp, char type) {
    if (type == static_cast<char>(dl_limit_type::COUNTER)) {
        logan << LDEBUG << "Checking download counter";
        return counter > 0;
    }

    if (type == static_cast<char>(dl_limit_type::TIMER)) {
        logan << LDEBUG << "Checking download timer";

        int64 diff = get_current_db_time() - timestamp;

        return diff < 0;
    }

    logan << LERROR << "Incorrect limit type: " << type;
    return false;
}
