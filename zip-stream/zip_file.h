#ifndef PROJECT_ZIP_FILE_H
#define PROJECT_ZIP_FILE_H

#include <dlib/dir_nav.h>

class zip_file {
private:
    std::string fullname;

    std::string generate_zip_name(dlib::file &df, const std::string &path_prefix);

    uint64_t size;
public:
    std::string zipname;

    std::string get_full_name() {
        return fullname;
    }

    uint64_t get_filesize();

    zip_file(dlib::file &df, const std::string &path_prefix) : fullname(df.full_name()),
                                                               size(df.size()),
                                                               zipname(generate_zip_name(df, path_prefix)) {};
};

#endif //PROJECT_ZIP_FILE_H
