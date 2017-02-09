#include "zip_headers.h"
#include "zip_streamer.h"

void write_64bit(std::ostream &stream, std::uint64_t v) {
    static const uint8_t max_byte = 0xFF; // Let the compiler hardcode this constant.
    static uint8_t buffer[8];

    buffer[0] = static_cast<uint8_t>((v) & max_byte);
    buffer[1] = static_cast<uint8_t>((v >> 8) & max_byte);
    buffer[2] = static_cast<uint8_t>((v >> 16) & max_byte);
    buffer[3] = static_cast<uint8_t>((v >> 24) & max_byte);
    buffer[4] = static_cast<uint8_t>((v >> 32) & max_byte);
    buffer[5] = static_cast<uint8_t>((v >> 40) & max_byte);
    buffer[6] = static_cast<uint8_t>((v >> 48) & max_byte);
    buffer[7] = static_cast<uint8_t>((v >> 56) & max_byte);
    stream.write((char *) &buffer[0], 8);
}

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

    archive_size += files.back().get_size() + files.back().get_directory_entry_size();
}

void zip_archive::stream(std::ostream &stream) {
    for (auto it = files.begin(); it != files.end(); ++it) {
        it->write_local_header(zsf, stream);
    }

    uint64_t relative_offset = 0;
    for (auto it = files.begin(); it != files.end(); ++it) {
        it->central_header.relative_offset_of_local_header = relative_offset;
        relative_offset += it->get_size();
        it->write_directory_header(stream);
        edr.zip64_end.disk_entries++;
        edr.zip64_end.directory_entries++;
        edr.zip64_end.directory_size += it->get_directory_entry_size();
        edr.zip64_end.offset += it->get_size();

        edr.zip64_locator.offset += it->get_size();
        edr.zip64_locator.offset += it->get_directory_entry_size();
    }

    edr.write(stream);
}

zip_archive::zip_archive(dlib::file &file, const zip_streamer_file &zsf) : zsf(zsf) {
    size_t index = file.full_name().find(file.name());
    std::string prefix = file.full_name().substr(0, index);

    zip_file zf(file, prefix);
    add(zf);
}

zip_archive::zip_archive(dlib::directory &dir, const zip_streamer_file &zsf) : zsf(zsf) {
    dlib::match_all m;
    std::string parent_name = dir.full_name();

    if (!dir.is_root()) {
        parent_name += dir.get_separator();
    }

    auto files = get_files_in_directory_tree(dir, m);
    for (auto it = files.begin(); it != files.end(); ++it) {
        zip_file zf(*it, parent_name);
        add(zf);
    }
}


uint32_t local_file_header::get_directory_entry_size() {
    return central_header.get_size() + file_name_len + central_header.EXTRA_FIELD_LEN;
}

uint64_t local_file_header::get_size() {
    // static + variable + data descriptor
    return 30 + file_name_len + data_desc.get_size() + file_size;
}

void local_file_header::write_local_header(zip_streamer_file &streamer, std::ostream &stream) {
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
    write_file_data_update_descriptor(streamer, stream);
    data_desc.write(stream);
}

void local_file_header::write_file_data_update_descriptor(zip_streamer_file &streamer, std::ostream &ostream) {
    if (file_size != 0) {

        std::ifstream in(full_name, std::ifstream::binary);
        streamer.stream_data(in, ostream);
        in.close();

        data_desc.crc32 = streamer.get_crc_checksum();
    } else {
        data_desc.crc32 = 0;
    }
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
    write_32bit(stream, 0xFFFFFFFF);//data_desc.compressed_size);
    write_32bit(stream, 0xFFFFFFFF);//data_desc.decompressed_size);
    write_16bit(stream, file_name_len);
    write_16bit(stream, central_header.EXTRA_FIELD_LEN);
    write_16bit(stream, central_header.COMMENT_LENGTH);
    write_16bit(stream, central_header.DISK_NUMBER);
    write_16bit(stream, central_header.INTERNAL_ATTRIBUTES);
    write_32bit(stream, central_header.EXTERNAL_ATTRIBUTES);
    write_32bit(stream, 0xFFFFFFFF);//central_header.relative_offset_of_local_header);
    stream.write(zip_name.c_str(), file_name_len);
//extra field
    write_16bit(stream, 0x0001);
    write_16bit(stream, 28);
    write_64bit(stream, data_desc.compressed_size);
    write_64bit(stream, data_desc.decompressed_size);
    write_64bit(stream, central_header.relative_offset_of_local_header);
    write_32bit(stream, 0);
}

uint32_t central_directory_header::get_size() {
    return 46 + COMMENT_LENGTH;
}

void data_descriptor::write(std::ostream &stream) {
    write_32bit(stream, MAGIC);
    write_32bit(stream, crc32);
    write_64bit(stream, compressed_size);
    write_64bit(stream, decompressed_size);
}

void end_directory_record::write(std::ostream &stream) {
    // write zip64_end
    write_32bit(stream, zip64_end.MAGIC);
    write_64bit(stream, zip64_end.RECORD_SIZE);
    write_16bit(stream, zip64_end.VERSION_MADE_BY);
    write_16bit(stream, zip64_end.VERSION_EXTRACT);
    write_32bit(stream, zip64_end.DISK_NUMBER);
    write_32bit(stream, zip64_end.START_DISK_NUMBER);
    write_64bit(stream, zip64_end.disk_entries);
    write_64bit(stream, zip64_end.directory_entries);
    write_64bit(stream, zip64_end.directory_size);
    write_64bit(stream, zip64_end.offset);

    // write zip64_locator
    write_32bit(stream, zip64_locator.MAGIC);
    write_32bit(stream, zip64_locator.DISK_NUMBER);
    write_64bit(stream, zip64_locator.offset);
    write_32bit(stream, zip64_locator.DISKS_COUNT);

    write_32bit(stream, MAGIC);
    write_16bit(stream, DISK_NUMBER);
    write_16bit(stream, START_DISK_NUMBER);
    if (zip64_end.disk_entries < DISK_ENTRIES) {
        write_16bit(stream, (uint16_t) zip64_end.disk_entries);
    } else {
        write_16bit(stream, DISK_ENTRIES);
    }

    if (zip64_end.directory_entries < DIRECTORY_ENTRIES) {
        write_16bit(stream, (uint16_t) zip64_end.directory_entries);
    } else {
        write_16bit(stream, DIRECTORY_ENTRIES);
    }

    write_32bit(stream, directory_size);
    write_32bit(stream, offset);
    write_16bit(stream, COMMENT_LENGTH);
}
