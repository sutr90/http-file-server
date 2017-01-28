#ifndef PROJECT_OPTINONS_H
#define PROJECT_OPTINONS_H


#include <dlib/uintn.h>
#include "../limit_type.h"

struct options {
    dlib::uint32 time_limit;
    dlib::uint32 count_limit;
    dl_limit_type limit_type;
    std::string file_name;
};

options parse_cmd_line(int argc, char **argv);

void validate_option(options &opt);

#endif //PROJECT_OPTINONS_H
