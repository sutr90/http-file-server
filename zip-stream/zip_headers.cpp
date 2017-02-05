#include "zip_headers.h"
#include <dlib/crc32.h>

void write_32bit(std::ostream &stream, uint32_t data) {
    char byte1 = (char) (data & 0x000000ff);
    char byte2 = (char) ((data & 0x0000ff00) >> 8);
    char byte3 = (char) ((data & 0x00ff0000) >> 16);
    char byte4 = (char) ((data & 0xff000000) >> 24);
    stream.write(&byte1, 1);
    stream.write(&byte2, 1);
    stream.write(&byte3, 1);
    stream.write(&byte4, 1);
}

void write_16bit(std::ostream &stream, uint16_t data) {
    char byte1 = (char) (data & 0x00ff);
    char byte2 = (char) ((data & 0xff00) >> 8);
    stream.write(&byte1, 1);
    stream.write(&byte2, 1);
}

void zip_archive::add(zip_file &file) {
    files.emplace_back(local_file_header(file));
}

void zip_archive::stream(std::ostream &stream) {
    for (auto it = files.begin(); it != files.end(); ++it) {
        it->write_local_header(stream);
    }

    uint32_t relative_offset = 0;
    for (auto it = files.begin(); it != files.end(); ++it) {
        it->central_header.relative_offset = relative_offset;
        relative_offset += it->get_entry_size();
        it->write_directory_header(stream);
        edr.disk_entries++;
        edr.directory_entries++;
        edr.directory_size += it->get_directory_entry_size();
        edr.offset += it->get_entry_size();
    }

    edr.write(stream);
}

zip_archive::zip_archive(dlib::file &file) {
    size_t index = file.full_name().find(file.name());
    std::string prefix = file.full_name().substr(0, index);

    zip_file zf(file, prefix);
    add(zf);
}

zip_archive::zip_archive(dlib::directory &dir) {
    dlib::match_all m;
    std::string parent_name = dir.get_parent().full_name();

    if (!dir.get_parent().is_root()) parent_name + dir.get_separator();

    auto files = get_files_in_directory_tree(dir, m);
    for (auto it = files.begin(); it != files.end(); ++it) {
        zip_file zf(*it, parent_name);
        add(zf);
    }
}


uint32_t local_file_header::get_directory_entry_size() {
    return central_header.get_size() + file_name_len + EXTRA_FIELD_LEN;
}

uint32_t local_file_header::get_entry_size() {
    // static + variable + data descriptor
    return 30 + file_name_len + EXTRA_FIELD_LEN + 16 + data_desc.compressed_size;
}

void local_file_header::write_local_header(std::ostream &stream) {
    write_32bit(stream, MAGIC);
    write_16bit(stream, VERSION_EXTRACT);
    write_16bit(stream, FLAGS);
    write_16bit(stream, COMPRESSION);
    write_16bit(stream, MOD_TIME);
    write_16bit(stream, MOD_DATE);
    write_32bit(stream, CRC32);
    write_32bit(stream, COMPRESSED_SIZE);
    write_32bit(stream, DECOMPRESSED_SIZE);
    write_16bit(stream, file_name_len);
    write_16bit(stream, EXTRA_FIELD_LEN);
    // for consistency reasons
    stream.write(zip_name.c_str(), file_name_len);
    //file data
    write_file_data_update_descriptor(stream);
    data_desc.write(stream);
}

void local_file_header::write_file_data_update_descriptor(std::ostream &stream) {
    if (file_size != 0) {

        std::ifstream in(full_name, std::ifstream::binary);
        uint32_t current = 0;
        int buffer_size = 64 * 1024;

        std::vector<char> buffer_vec(64 * 1024);

        uint32_t filesize = (uint32_t) file_size; //TODO

        dlib::crc32 crc;

        while (current < filesize) {
            in.read(&buffer_vec[0], buffer_size);
            std::streamsize bytes = in.gcount();
            buffer_vec.resize(bytes);
            stream.write(&buffer_vec[0], bytes);
            current += bytes;
            crc.add(buffer_vec);
        }

        in.close();
        data_desc.crc32 = crc.get_checksum();
    } else {
        data_desc.crc32 = 0;
    }

    data_desc.compressed_size = file_size;
    data_desc.decompressed_size = file_size;
}

void local_file_header::write_directory_header(std::ostream &stream) {
    write_32bit(stream, central_header.MAGIC);
    write_16bit(stream, central_header.VERSION_MADE_BY);
    write_16bit(stream, VERSION_EXTRACT);
    write_16bit(stream, FLAGS);
    write_16bit(stream, COMPRESSION);
    write_16bit(stream, MOD_TIME);
    write_16bit(stream, MOD_DATE);
    write_32bit(stream, data_desc.crc32);
    write_32bit(stream, data_desc.compressed_size);
    write_32bit(stream, data_desc.decompressed_size);
    write_16bit(stream, file_name_len);
    write_16bit(stream, EXTRA_FIELD_LEN);
    write_16bit(stream, central_header.COMMENT_LENGTH);
    write_16bit(stream, central_header.DISK_NUMBER);
    write_16bit(stream, central_header.INTERNAL_ATTRIBUTES);
    write_32bit(stream, central_header.EXTERNAL_ATTRIBUTES);
    write_32bit(stream, central_header.relative_offset);
    stream.write(zip_name.c_str(), file_name_len);
}

uint32_t central_directory_header::get_size() {
    return 46 + COMMENT_LENGTH;
}

void data_descriptor::write(std::ostream &stream) {
    write_32bit(stream, MAGIC);
    write_32bit(stream, crc32);
    write_32bit(stream, compressed_size);
    write_32bit(stream, decompressed_size);
}

void end_directory_record::write(std::ostream &stream) {
    write_32bit(stream, MAGIC);
    write_16bit(stream, DISK_NUMBER);
    write_16bit(stream, START_DISK_NUMBER);
    write_16bit(stream, disk_entries);
    write_16bit(stream, directory_entries);
    write_32bit(stream, directory_size);
    write_32bit(stream, offset);
    write_16bit(stream, COMMENT_LENGTH);
}
