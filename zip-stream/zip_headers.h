#ifndef PROJECT_ZIP_HEADERS_H
#define PROJECT_ZIP_HEADERS_H


#include <dlib/dir_nav.h>


class data_descriptor {
public:
    const uint32_t MAGIC = 0x08074b50;
    uint32_t crc32 =0;
    uint32_t compressed_size = 0;
    uint32_t decompressed_size = 0;

    data_descriptor() {};

    void write(std::ostream &stream);
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
    uint32_t relative_offset = 0;
    local_file_header *local_header = nullptr;

    central_directory_header(local_file_header *local) :
            local_header(local) {}

    uint32_t get_size();
};

class local_file_header {
public:
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
    uint16_t file_name_len = 0;
    const uint16_t EXTRA_FIELD_LEN = 0x0000;

    data_descriptor data_desc;
    central_directory_header central_header;
    std::string zip_name;

    local_file_header(dlib::file &file) : local_file_header(file, file.name()) {};

    local_file_header(dlib::file &file, std::string zip_name) :
            file_ref(file),
            file_name_len((uint16_t) zip_name.length()),
            central_header(this),
            zip_name(zip_name) {};

    uint32_t get_directory_entry_size();

    uint32_t get_entry_size();

    void write_local_header(std::ostream &stream);

    void write_file_data_update_descriptor(std::ostream &stream);

    void write_directory_header(std::ostream &stream);
};

class end_directory_record {
public:
    const uint32_t MAGIC = 0x06054b50;
    const uint16_t DISK_NUMBER = 0x0000;
    const uint16_t START_DISK_NUMBER = 0x0000;
    uint16_t disk_entries = 0;
    uint16_t directory_entries = 0;
    uint32_t directory_size = 0;
    uint32_t offset = 0;
    const uint16_t COMMENT_LENGTH = 0x0000;

    end_directory_record() {};

    void write(std::ostream &stream);
};

class zip_archive {
private:
    std::vector<local_file_header> files;

    end_directory_record edr;

public:
    void add(dlib::file &file);
//    void add(dlib::directory &directory);

    void stream(std::ostream &stream);
};


#endif //PROJECT_ZIP_HEADERS_H
