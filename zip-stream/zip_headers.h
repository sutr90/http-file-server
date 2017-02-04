#ifndef PROJECT_ZIP_HEADERS_H
#define PROJECT_ZIP_HEADERS_H


#include <dlib/dir_nav.h>


class data_descriptor {
public:
    const uint32_t MAGIC = 0x08074b50;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t decompressed_size;

    data_descriptor() {};
};

class local_file_header;

class central_directory_header {
public:
    const uint32_t MAGIC = 0x02014b50;
    const uint16_t VERSION_MADE_BY = 0x003f;
    const uint16_t COMMENT_LENGTH = 0x0000;
    const uint16_t DISK_NUMBER = 0x0000;
    const uint16_t INTERNAL_ATTRIBUTES = 0x0000;
    const uint32_t EXTERNAL_ATTRIBUTES = 0x0000;
    uint32_t relative_offset;
    local_file_header *local_header;

    central_directory_header(local_file_header *local) :
            local_header(local) {}

};

class local_file_header {
protected:
    dlib::file &file_ref;

    const uint32_t MAGIC = 0x04034b50;
    const uint16_t VERSION_EXTRACT = 0x000a;
    const uint16_t FLAGS = 0x0008;
    const uint16_t COMPRESSION = 0x0000;
    const uint16_t MOD_TIME = 0x0000;
    const uint16_t MOD_DATE = 0x0000;
    const uint32_t CRC32 = 0x0000;
    const uint32_t COMPRESSED_SIZE = 0x0000;
    const uint32_t DECOMPRESSED_SIZE = 0x0000;
    uint16_t file_name_len;
    const uint16_t EXTRA_FIELD_LEN = 0x0000;

    data_descriptor data_desc;
    central_directory_header central_header;
public:
    local_file_header(dlib::file &file) : local_file_header(file, file.name()) {};

    local_file_header(dlib::file &file, std::string zip_name) :
            file_ref(file),
            file_name_len((uint16_t) zip_name.length()),
            central_header(this) {};

};

class end_directory_record {
public:
    const uint32_t MAGIC = 0x06054b50;
    const uint16_t DISK_NUMBER = 0x0000;
    const uint16_t START_DISK_NUMBER = 0x0000;
    uint16_t disk_entries;
    uint16_t directory_entries;
    uint32_t directory_size;
    uint32_t offset;
    const uint16_t COMMENT_LENGTH = 0x0000;

    end_directory_record() {};
};

class zip_archive {
private:
    std::vector<local_file_header> files;

    end_directory_record edr;

public:
    void add(dlib::file &file);
//    void add(dlib::directory &directory);
};


#endif //PROJECT_ZIP_HEADERS_H
