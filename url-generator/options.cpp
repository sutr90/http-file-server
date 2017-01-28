#include <dlib/cmd_line_parser.h>
#include "options.h"

using namespace dlib;
using namespace std;

uint32 parse_time(const string &time_string);

command_line_parser parser;

void print_usage_and_exit() {
    cout << "Usage:\ngenerator [-c <arg> | -t <arg>] <absolute file path>\n";
    parser.print_options();
    exit(0);
}

options parse_cmd_line(int argc, char **argv) {
    try {
        parser.add_option("c", "Allow only this number of downloads. Limit <1, 65536>", 1);
        parser.add_option("t", "Allow downloads only for this time from creating the link. Format hh:mm.", 1);
        parser.add_option("h", "Show help");
        parser.parse(argc, argv);

        if (parser.option("h")) {
            print_usage_and_exit();
        }

        const char *one_time_opts[] = {"c", "t"};
        parser.check_one_time_options(one_time_opts);
        parser.check_incompatible_options("c", "t");
        parser.check_option_arg_range("c", 1, 65536);

        if (parser.number_of_arguments() != 1) {
            cout << "File path is missing" << endl;
            print_usage_and_exit();
        }
    }
    catch (exception &e) {
        cout << e.what() << endl;
        print_usage_and_exit();
    }


    options o;
    o.limit_type = dl_limit_type::COUNTER;
    o.file_name = parser[0];
    o.count_limit = (uint32) get_option(parser, "c", 1);

    if (parser.option("t")) {
        o.limit_type = dl_limit_type::TIMER;
        o.time_limit = parse_time(parser.option("t").argument());
    } else {
        o.time_limit = 0;
    }

    return o;
}

uint32 parse_time(const string &time_string) {
    //check format hh:mm
    size_t colon = time_string.find(":");
    if (colon == string::npos) {
        cout << "incorrect time format" << endl;
        print_usage_and_exit();
    }

    int hours = stoi(time_string.substr(0, colon));
    int minutes = stoi(time_string.substr(colon + 1));

    if (minutes >= 60 || minutes < 0 || hours < 0) {
        cout << "incorrect time value";
        print_usage_and_exit();
    }

    return (uint32) (hours * 3600 + minutes * 60);
}
