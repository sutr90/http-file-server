#ifndef PROJECT_URL_GENERATOR_H
#define PROJECT_URL_GENERATOR_H

#include "options.h"

std::string register_file(dlib::database &db, options &opt);

void unregister_file(dlib::database &db, options &opt);

void list_registered_files(dlib::database &db);

#endif //PROJECT_URL_GENERATOR_H
