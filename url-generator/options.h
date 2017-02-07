#ifndef PROJECT_OPTINONS_H
#define PROJECT_OPTINONS_H


#include <dlib/uintn.h>

enum option_type {
    OPT_COUNTER = 'c',
    OPT_TIMER = 't',
    OPT_REMOVE = 'r',
    OPT_LIST
};

struct options {
    dlib::uint32 time_limit;
    dlib::uint32 count_limit;
    std::string file_name;
    option_type type;
};

std::ostream& operator<<(std::ostream& os, const options& opt);

options parse_cmd_line(int argc, char **argv);

void validate_option(options &opt);

dlib::uint32 parse_time(const std::string &time_string);

#endif //PROJECT_OPTINONS_H
