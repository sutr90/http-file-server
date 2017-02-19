#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <sys/stat.h>
#include <string>

bool is_path_file(std::string &path);

bool is_prefix(const std::string& needle, const std::string& haystack);
bool is_prefix(const char* needle, const std::string& haystack);

#endif //PROJECT_UTILS_H
