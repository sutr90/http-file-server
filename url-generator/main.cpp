#include <dlib/cmd_line_parser.h>
#include <dlib/sqlite/sqlite.h>
#include <dlib/config_reader.h>
#include "options.h"
#include "url_generator.h"

using namespace std;
using namespace dlib;

int main(int argc, char **argv) {
    options opt = parse_cmd_line(argc, argv);

    config_reader cr("config");
    database db(cr["db_path"]);

    if (opt.type == option_type::OPT_REMOVE) {
        unregister_file(db, opt);
        cout << "Download link with id " << opt.file_name << " was removed.";
    } else if (opt.type == option_type::OPT_LIST) {
        list_registered_files(db);
    } else {
        string file_id = register_file(db, opt, string(""));
        string url = get_option(cr, "generator.domain", "") + file_id;

        cout << "Your file - " << opt.file_name << " - is available for download at:" << endl;
        cout << url << endl;
        if (opt.type == option_type::OPT_COUNTER) {
            cout << "For " << opt.count_limit << " downloads";
        } else {
            cout << "For " << opt.time_limit / 60 << " minutes";
        }
    }

    cout << endl;

    return 0;
}