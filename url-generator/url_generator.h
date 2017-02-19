#ifndef PROJECT_URL_GENERATOR_H
#define PROJECT_URL_GENERATOR_H

#include "options.h"

struct file_record {
    std::string file_id;
    std::string path;
    std::string limit_type;
    int dl_counter;
    int time_limit;

    void to_json(std::stringstream &ss);
};

std::string register_file(dlib::database &db, options &opt, const std::string &user_id);

void unregister_file(dlib::database &db, options &opt);

void list_registered_files(dlib::database &db);

void get_list_registered_files(dlib::database &db, std::vector<file_record> &files);

#endif //PROJECT_URL_GENERATOR_H
