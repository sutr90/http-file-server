
#ifndef PROJECT_DIR_UTILS_H
#define PROJECT_DIR_UTILS_H

#include <string>
#include <type_traits>

std::string get_dir_contents_as_json(std::string directory);

long get_date(const std::string &fname);

template<class DF>
void write_type_size(DF &dirfile, std::stringstream &ss);

template<class DF>
std::string generate_json(std::stringstream &ss, DF &dirfile) {
    ss << "{";
    ss << "\"name\":\"" << dirfile.name() << "\",";
    ss << "\"path\":\"" << dirfile.full_name() << "\",";
    write_type_size(dirfile, ss);
    ss << "\"date\":" << get_date(dirfile.full_name());
    ss << "}";
}


#endif //PROJECT_DIR_UTILS_H
