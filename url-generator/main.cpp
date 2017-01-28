#include <dlib/cmd_line_parser.h>
#include "options.h"

using namespace std;
using namespace dlib;

int main(int argc, char **argv) {
    options opt = parse_cmd_line(argc, argv);

    return 0;
}