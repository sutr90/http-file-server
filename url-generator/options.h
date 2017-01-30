#ifndef PROJECT_OPTINONS_H
#define PROJECT_OPTINONS_H


#include <dlib/uintn.h>

enum class option_type : char {
    OPT_COUNTER = 'c',
    OPT_TIMER = 't',
    OPT_REMOVE = 'r',
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
