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

options parse_cmd_line(int argc, char **argv);

void validate_option(options &opt);

#endif //PROJECT_OPTINONS_H
