#include <dlib/cmd_line_parser.h>
#include <dlib/dir_nav.h>
#include "options.h"
#include "../utils.h"

using namespace dlib;
using namespace std;

command_line_parser parser;

void print_usage_and_exit() {
    cout << "Usage:\ngenerator [-c <arg> | -t <arg>] <absolute file path> :generate link\n";
    cout << "generator -r <file id> :remove link <file id>\n";
    cout << "generator -l :list all links\n";
    parser.print_options();
    exit(0);
}

options parse_cmd_line(int argc, char **argv) {
    try {
        parser.add_option("c", "Allow only this number of downloads. Limit <1, 65536>", 1);
        parser.add_option("t", "Allow downloads only for this time from creating the link. Format hh:mm.", 1);
        parser.add_option("r", "Remove the link.", 1);
        parser.add_option("l", "List available links.");

        parser.add_option("h", "Show help");
        parser.parse(argc, argv);

        if (parser.option("h")) {
            print_usage_and_exit();
        }

        const char *one_time_opts[] = {"c", "t", "r", "l"};
        parser.check_one_time_options(one_time_opts);
        parser.check_incompatible_options("c", "t");
        parser.check_incompatible_options("c", "r");
        parser.check_incompatible_options("c", "l");
        parser.check_incompatible_options("r", "t");
        parser.check_incompatible_options("r", "l");
        parser.check_incompatible_options("t", "l");

        parser.check_option_arg_range("c", 1, 65536);

        if (parser.option("r")) {
            options o;
            o.type = option_type::OPT_REMOVE;
            o.file_name = parser.option("r").argument();
            return o;
        }

        if (parser.option("l")) {
            options o;
            o.type = option_type::OPT_LIST;
            return o;
        }

        if (parser.number_of_arguments() < 1) {
            cout << "File path is missing" << endl;
            print_usage_and_exit();
        }
    }
    catch (exception &e) {
        cout << e.what() << endl;
        print_usage_and_exit();
    }

    options o;
    o.type = option_type::OPT_COUNTER;
    o.count_limit = (uint32) get_option(parser, "c", 1);
    o.file_name = parser[0];

    if (parser.option("t")) {
        o.type = option_type::OPT_TIMER;
        o.time_limit = parse_time(parser.option("t").argument());
    } else if (parser.option("c")) {
        o.type = option_type::OPT_COUNTER;
        o.time_limit = 0;
    }

    validate_option(o);

    return o;
}

void validate_option(options &opt) {

    //TODO

    if (opt.type != option_type::OPT_TIMER && opt.type != option_type::OPT_COUNTER) {
        throw std::exception();
    }

    if (opt.type == option_type::OPT_TIMER && opt.time_limit < 60) {
        cout << "Minimal time limit is 1 minute" << endl;
        throw std::exception();
    }

    if (opt.type == option_type::OPT_COUNTER && opt.count_limit < 1) {
        cout << "Minimal count is 1" << endl;
        throw std::exception();
    }

    try {
        if (is_path_file(opt.file_name)) {
            file test(opt.file_name);
            opt.file_name = test.full_name();
        } else {
            directory test(opt.file_name);
            opt.file_name = test.full_name();
        }
    }
    catch (file::file_not_found &e) {
        cout << "File " << opt.file_name << " not found or accessible: " << e.info << endl;
        throw std::exception();
    }
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
        cout << "incorrect time value" << endl;
        print_usage_and_exit();
    }

    return (uint32) (hours * 3600 + minutes * 60);
}


std::ostream &operator<<(std::ostream &os, const options &opt) {
    os << "[options::count_limit=" << opt.count_limit << endl;
    os << "options::time_limit=" << opt.time_limit << endl;
    os << "options::type=" << opt.type << endl;
    os << "options::file_name=" << opt.file_name << "]";

    return os;
}