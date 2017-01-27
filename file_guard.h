//
// Created by VF on 27.01.2017.
//

#ifndef SERVER_FILE_GUARD_H
#define SERVER_FILE_GUARD_H

#include <string>
#include <dlib/sqlite.h>
struct response;

class file_guard {
private:
    dlib::database db;

    dlib::int64 get_current_db_time();

public:
    file_guard(std::string &db_path);
    void get_file(std::string url, response &response);
};


#endif //SERVER_FILE_GUARD_H
