#ifndef PROJECT_ZIP_FILE_H
#define PROJECT_ZIP_FILE_H

#include <dlib/dir_nav.h>

class zip_file {
private:
    std::string fullname;

    std::string generate_zip_name(dlib::file &df, const std::string &path_prefix);

    std::string generate_zip_name(dlib::directory &df, const std::string &path_prefix);

    uint32_t size;
public:
    std::string zipname;

    std::string get_full_name() {
        return fullname;
    }

    uint32_t get_filesize();

    zip_file(dlib::file &df, const std::string &path_prefix) : fullname(df.full_name()),
                                                               zipname(generate_zip_name(df, path_prefix)),
                                                               size(df.size()) {};

    zip_file(dlib::directory &df, const std::string &path_prefix) : fullname(df.full_name()),
                                                                    zipname(generate_zip_name(df, path_prefix)),
                                                                    size(0) {};
};

#endif //PROJECT_ZIP_FILE_H
