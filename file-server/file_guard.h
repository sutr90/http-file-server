#ifndef SERVER_FILE_GUARD_H
#define SERVER_FILE_GUARD_H

#include <string>
#include <dlib/sqlite.h>
struct response;

class file_guard {
private:
    dlib::database &db;

    dlib::int64 get_current_db_time();
    bool can_download(int counter, dlib::int64 timestamp, char type);

public:
    file_guard(dlib::database &database);
    void get_file(std::string url, response &response);

};


#endif //SERVER_FILE_GUARD_H
