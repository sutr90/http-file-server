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

    char data[] = {0x0a, 0, 0, 0, 0, 0};
    ostream.write((char *) &data, 6);

    byteint datetime;
    time_t time_val = time(NULL);
    tm *tm = localtime(&time_val);

    datetime.i = 0;
    ostream.write((char *) &datetime.b, 4);

    //crc
    datetime.i =0xE8B7BE43;
    ostream.write((char *) &datetime.b, 4);

    byteint size;
    size.i = 1;
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

    char data[] = {0x3f, 0, 0x0a, 0, 0, 0, 0, 0};
    ostream.write((char *) &data, 8);

    byteint datetime;
    time_t time_val = time(NULL);
    tm *tm = localtime(&time_val);

    datetime.i = 0;
    ostream.write((char *) &datetime.b, 4);

    //crc
    datetime.i =0xE8B7BE43;
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
    size.i = 0x24;//offset
    ostream.write((char *) &size.b, 4);
    size.i = 0;
    ostream.write((char *) &size.b, 2);
}

//[local file header 1]
//[encryption header 1]
//[file data 1]
//[data descriptor 1]
//.
//.
//.
//[local file header n]
//[encryption header n]
//[file data n]
//[data descriptor n]
//[archive decryption header]
//[archive extra data record]
//[central directory header 1]
//.
//.
//.
//[central directory header n]
//[zip64 end of central directory record]
//[zip64 end of central directory locator]
//[end of central directory record]

uint32_t tm_to_dosdate(const struct tm *ptm) {
    uint32_t year = 0;

#define datevalue_in_range(min, max, value) ((min) <= (value) && (value) <= (max))
    /* Years supported:
    * [00, 79] (assumed to be between 2000 and 2079)
    * [80, 207] (assumed to be between 1980 and 2107, typical output of old
    software that does 'year-1900' to get a double digit year)
    * [1980, 2107]
    Due to the date format limitations, only years between 1980 and 2107 can be stored.
    */
    if (!(datevalue_in_range(1980, 2107, ptm->tm_year) || datevalue_in_range(0, 207, ptm->tm_year)) ||
        !datevalue_in_range(0, 11, ptm->tm_mon) ||
        !datevalue_in_range(1, 31, ptm->tm_mday) ||
        !datevalue_in_range(0, 23, ptm->tm_hour) ||
        !datevalue_in_range(0, 59, ptm->tm_min) ||
        !datevalue_in_range(0, 59, ptm->tm_sec)) {
        return 0;
    }
#undef datevalue_in_range

    year = (uint32_t) ptm->tm_year;
    if (year >= 1980) /* range [1980, 2107] */
        year -= 1980;
    else if (year >= 80) /* range [80, 99] */
        year -= 80;
    else /* range [00, 79] */
        year += 20;

    return (uint32_t) (((ptm->tm_mday) + (32 * (ptm->tm_mon + 1)) + (512 * year)) << 16) |
           ((ptm->tm_sec / 2) + (32 * ptm->tm_min) + (2048 * (uint32_t) ptm->tm_hour));
}