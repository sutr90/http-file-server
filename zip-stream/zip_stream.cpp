#include <dlib/dir_nav.h>

union byteint {
    uint8_t b[sizeof(uint32_t)];
    uint32_t i;
};

uint32_t tm_to_dosdate(const struct tm *ptm);

void write_local_file_header(dlib::file &file, std::ostream &ostream);

void write_file_data(dlib::file &file, std::ostream &ostream);

void write_data_central_directory(dlib::file &file, std::ostream &ostream);

void stream_dirfile(dlib::directory &directory, std::ostream &ostream) {
    std::cout << "streaming dir\n";

//    dlib::match_all ma;
//    auto files = dlib::get_files_in_directory_tree(directory, ma);
//    for (auto it = files.begin(); it != files.end(); ++it) {
//        stream_dirfile(*it, ostream);
//    }
}

void stream_dirfile(dlib::file &file, std::ostream &ostream) {
    std::cout << "streaming file\n";

    write_local_file_header(file, ostream);
    write_file_data(file, ostream);
    write_data_central_directory(file, ostream);
}

void write_local_file_header(dlib::file &file, std::ostream &ostream) {
//    local file header signature     4 bytes  (0x04034b50)
//    version needed to extract       2 bytes
//    general purpose bit flag        2 bytes
//    compression method              2 bytes
//    last mod file time              2 bytes
//    last mod file date              2 bytes
//    crc-32                          4 bytes
//    compressed size                 4 bytes
//    uncompressed size               4 bytes
//    file name length                2 bytes
//    extra field length              2 bytes
//
//    file name (variable size)
//    extra field (variable size)

    byteint mag;
    mag.i = (uint32_t) 0x04034b50;
    ostream.write((char *) &mag.b, 4);

    char data[] = {0x0a, 0, 8, 0, 0, 0};
    ostream.write((char *) &data, 6);

    byteint datetime;

    datetime.i = 0;
    ostream.write((char *) &datetime.b, 4);

    //crc
    datetime.i = 0;//0xE8B7BE43;
    ostream.write((char *) &datetime.b, 4);

    byteint size;
    size.i = 0;//1;
    ostream.write((char *) &size.b, 4);
    ostream.write((char *) &size.b, 4);
    size.i = 5;
    ostream.write((char *) &size.b, 2);
    size.i = 0;
    ostream.write((char *) &size.b, 2);

    const char *a = "a.txt";
    ostream.write(a, 5);
}

void write_file_data(dlib::file &file, std::ostream &ostream) {
    const char *a = "a";
    ostream.write(a, 1);

    byteint datetime;

    datetime.i = 0x08074b50;
    ostream.write((char *) &datetime.b, 4);

    //crc
    datetime.i = 0xE8B7BE43;
    ostream.write((char *) &datetime.b, 4);
    datetime.i = 1;
    ostream.write((char *) &datetime.b, 4);
    ostream.write((char *) &datetime.b, 4);
}

void write_data_central_directory(dlib::file &file, std::ostream &ostream) {
//    central file header signature   4 bytes  (0x02014b50)
//    version made by                 2 bytes
//    version needed to extract       2 bytes
//    general purpose bit flag        2 bytes
//    compression method              2 bytes
//    last mod file time              2 bytes
//    last mod file date              2 bytes
//    crc-32                          4 bytes
//    compressed size                 4 bytes
//    uncompressed size               4 bytes
//    file name length                2 bytes
//    extra field length              2 bytes
//    file comment length             2 bytes
//    disk number start               2 bytes
//    internal file attributes        2 bytes
//    external file attributes        4 bytes
//    relative offset of local header 4 bytes
//
//    file name (variable size)
//    extra field (variable size)
//    file comment (variable size)
    byteint mag;
    mag.i = (uint32_t) 0x02014b50;
    ostream.write((char *) &mag.b, 4);

    char data[] = {0x3f, 0, 0x0a, 0, 8, 0, 0, 0};
    ostream.write((char *) &data, 8);

    byteint datetime;

    datetime.i = 0;
    ostream.write((char *) &datetime.b, 4);

    //crc
    datetime.i = 0xE8B7BE43;
    ostream.write((char *) &datetime.b, 4);

    byteint size;
    size.i = 1;
    ostream.write((char *) &size.b, 4);
    ostream.write((char *) &size.b, 4);

    size.i = 5;
    ostream.write((char *) &size.b, 2);
    size.i = 0;
    ostream.write((char *) &size.b, 2);
//file comment + disk number
    ostream.write((char *) &size.b, 4);
    //int + ext attributes + offset
    ostream.write((char *) &size.b, 2);
    ostream.write((char *) &size.b, 4);
    ostream.write((char *) &size.b, 4);

    const char *a = "a.txt";
    ostream.write(a, 5);


//    4.3.16  End of central directory record:
//
//    end of central dir signature    4 bytes  (0x06054b50)
//    number of this disk             2 bytes
//    number of the disk with the
//    start of the central directory  2 bytes
//    total number of entries in the
//    central directory on this disk  2 bytes
//    total number of entries in
//    the central directory           2 bytes
//    size of the central directory   4 bytes
//    offset of start of central
//    directory with respect to
//    the starting disk number        4 bytes
//            .ZIP file comment length        2 bytes
//            .ZIP file comment       (variable size)

    mag.i = (uint32_t) 0x06054b50;
    ostream.write((char *) &mag.b, 4);

    char data2[] = {0, 0, 0, 0, 0x1, 0, 0x1, 0};
    ostream.write((char *) &data2, 8);

    size.i = 0x33;
    ostream.write((char *) &size.b, 4);
    size.i = 0x34;//offset
    ostream.write((char *) &size.b, 4);
    size.i = 0;
    ostream.write((char *) &size.b, 2);
}
