#include <dlib/dir_nav.h>
#include "dir_utils.h"

#include <sys/stat.h>

using namespace std;
using namespace dlib;

template<>
void write_type_size<dlib::directory>(dlib::directory &dirfile, std::stringstream &ss) {
    ss << "\"type\":\"dir\",";
    ss << "\"size\":null,";
};

template<>
void write_type_size<dlib::file>(dlib::file &dirfile, std::stringstream &ss) {
    ss << "\"type\":\"file\",";
    ss << "\"size\":" << dirfile.size() << ",";
};


string get_dir_contents_as_json(std::string directory) {
    dlib::directory dir(directory);

    auto child_dirs = dir.get_dirs();
    auto child_files = dir.get_files();

    sort(child_dirs.begin(), child_dirs.end(),
         [](const dlib::directory &a, const dlib::directory &b) {
             return a.name() > b.name();
         });

    sort(child_files.begin(), child_files.end(),
         [](const dlib::file &a, const dlib::file &b) {
             return a.name() > b.name();
         });


    stringstream ss;
    ss << "[";
    for (auto it = child_dirs.begin(); it != child_dirs.end(); ++it) {
        generate_json(ss, *it);
        ss << ",";
    }

    for (auto it = child_files.begin(); it != child_files.end(); ++it) {
        generate_json(ss, *it);
        if (it != child_files.end() - 1) {
            ss << ",";
        }
    }
    ss << "]";

    std::string s = ss.str();
    std::replace(s.begin(), s.end(), '\\', '/'); // replace all 'x' to 'y'

    return s;
}

long get_date(const std::string &fname) {
    struct stat t_stat;
    stat(fname.c_str(), &t_stat);
    return t_stat.st_ctime;
}