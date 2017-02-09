#ifndef PROJECT_ZIP_HEADERS_H
#define PROJECT_ZIP_HEADERS_H


#include <dlib/dir_nav.h>
#include "zip_file.h"
#include "zip_streamer.h"

class zip64_end_of_central_dir {
public:
    const uint32_t MAGIC = 0x06064b50;
    const uint64_t RECORD_SIZE = 44;
    const uint16_t VERSION_MADE_BY = 0x003f;
    const uint16_t VERSION_EXTRACT = 0x002D;
    const uint32_t DISK_NUMBER = 0x00000000;
    const uint32_t START_DISK_NUMBER = 0x00000000;
    uint64_t disk_entries = 0;
    uint64_t directory_entries = 0;
    uint64_t directory_size = 0;
    uint64_t offset = 0;

    inline uint64_t get_size() {
        return 56;
    }
};

class zip64_end_of_central_dir_locator {
public:
    const uint32_t MAGIC = 0x07064b50;
    const uint32_t DISK_NUMBER = 0x00000000;
    uint64_t offset = 0;
    const uint32_t DISKS_COUNT = 0x00000001;

    inline uint64_t get_size() {
        return 20;
    }
};

class data_descriptor {
public:
    const uint32_t MAGIC = 0x08074b50;
    uint32_t crc32 = 0;
    uint64_t compressed_size = 0;
    uint64_t decompressed_size = 0;

    data_descriptor() {};

    void write(std::ostream &stream);

    inline uint64_t get_size() {
        return 24;
    }
};

class local_file_header;

class central_directory_header {
public:
    const uint32_t MAGIC = 0x02014b50;
    const uint16_t VERSION_MADE_BY = 0x003f;
    const uint16_t COMMENT_LENGTH = 0x0000;
    const uint16_t DISK_NUMBER = 0xFFFF;
    const uint16_t INTERNAL_ATTRIBUTES = 0x0000;
    const uint32_t EXTERNAL_ATTRIBUTES = 0x0000;
    const uint16_t EXTRA_FIELD_LEN = 0x0020;
    uint64_t relative_offset_of_local_header = 0;

    central_directory_header() {}

    uint32_t get_size();
};

class local_file_header {
public:
    const uint32_t MAGIC = 0x04034b50;
    const uint16_t VERSION_EXTRACT = 0x002D;
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
    std::string full_name;
    uint64_t file_size;

    local_file_header(zip_file &file) : local_file_header(file, file.zipname) {};

    local_file_header(zip_file &file, std::string zip_name) :
            file_name_len((uint16_t) zip_name.length()),
            central_header(),
            zip_name(zip_name),
            full_name(file.get_full_name()),
            file_size(file.get_filesize()) {
        data_desc.compressed_size = file_size;
        data_desc.decompressed_size = file_size;
    };

    uint32_t get_directory_entry_size();

    uint64_t get_size();

    void write_local_header(zip_streamer_file &streamer, std::ostream &stream);

    void write_file_data_update_descriptor(zip_streamer_file &stream, std::ostream &ostream);

    void write_directory_header(std::ostream &stream);
};

class end_directory_record {
public:
    const uint32_t MAGIC = 0x06054b50;
    const uint16_t DISK_NUMBER = 0xFFFF;
    const uint16_t START_DISK_NUMBER = 0xFFFF;
    const uint16_t DISK_ENTRIES = 0xFFFF;
    const uint16_t DIRECTORY_ENTRIES = 0xFFFF;
    const uint32_t directory_size = 0xFFFFFFFF;
    const uint32_t offset = 0xFFFFFFFF;
    const uint16_t COMMENT_LENGTH = 0x0000;

    zip64_end_of_central_dir zip64_end;
    zip64_end_of_central_dir_locator zip64_locator;

    end_directory_record() : zip64_end(), zip64_locator() {};

    void write(std::ostream &stream);

    uint64_t get_size() { return 22 + zip64_end.get_size() + zip64_locator.get_size(); }
};

class zip_archive {
private:
    std::vector<local_file_header> files;

    end_directory_record edr;

    uint64_t archive_size = edr.get_size();

    zip_streamer_file zsf;

public:
    uint64_t get_archive_size() { return archive_size; };

    void add(zip_file &file);

    void stream(std::ostream &stream);

    zip_archive(dlib::file &file, const zip_streamer_file &zsf = zip_streamer_file());

    zip_archive(dlib::directory &dir, const zip_streamer_file &zsf = zip_streamer_file());

};


#endif //PROJECT_ZIP_HEADERS_H
