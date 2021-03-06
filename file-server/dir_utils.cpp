#include <dlib/dir_nav.h>
#include <sys/stat.h>
#include "dir_utils.h"
#include "../logging.h"
#include "../utils.h"

using namespace std;
using namespace dlib;

const dlib::logger logan("L.dir_utils");

std::string get_parent_dir_json(dlib::directory &root, dlib::directory &dir);

template<>
void write_type_size<dlib::directory>(dlib::directory &, std::stringstream &ss) {
    ss << "\"type\":\"dir\",";
    ss << "\"size\":null,";
};

template<>
void write_type_size<dlib::file>(dlib::file &dirfile, std::stringstream &ss) {
    ss << "\"type\":\"file\",";
    ss << "\"size\":" << dirfile.size() << ",";
};


string get_dir_contents_as_json(dlib::directory &root, dlib::directory &dir) {
    logan << LTRACE << "get_dir_contents_as_json";
    auto child_dirs = dir.get_dirs();
    auto child_files = dir.get_files();

    sort(child_dirs.begin(), child_dirs.end(),
         [](const dlib::directory &a, const dlib::directory &b) {
             return a.name() < b.name();
         });

    sort(child_files.begin(), child_files.end(),
         [](const dlib::file &a, const dlib::file &b) {
             return a.name() < b.name();
         });


    stringstream ss;
    ss << "[";

    ss << get_parent_dir_json(root, dir) << ",";

    for (auto it = child_dirs.begin(); it != child_dirs.end(); ++it) {
        generate_json(ss, *it);
        ss << ",";
    }

    for (auto it = child_files.begin(); it != child_files.end(); ++it) {
        generate_json(ss, *it);
        ss << ",";
    }

    // move one char back
    ss.seekp(-1, ss.cur);
    // replace last , with ]
    ss << "]";

    std::string s = ss.str();
    std::replace(s.begin(), s.end(), '\\', '/'); // replace all 'x' to 'y'

    return s;
}

std::string get_parent_dir_json(dlib::directory &root, dlib::directory &dir) {
    logan << LTRACE << "get_parent_dir_json";
    directory parent = dir.get_parent();
    auto tmp = root;
    if (is_prefix(root.full_name(), parent.full_name())) {
        tmp = parent;
    }
    return "{\"name\":\"..\",\"path\":\"" + tmp.full_name() + tmp.get_separator() +
           "\",\"type\":\"dir\",\"size\":null,\"date\":null}";
}

long get_date(const std::string &fname) {
    struct stat t_stat;
    stat(fname.c_str(), &t_stat);
    return t_stat.st_ctime;
}


