#ifndef PROJECT_STREAMER_H
#define PROJECT_STREAMER_H

#include <ostream>
#include <istream>
#include <dlib/crc32.h>
#include "../streamer.h"


class zip_streamer_file : protected streamer {
protected:
    dlib::crc32 crc;

    zip_streamer_file(uint32_t buffer_size = 64 * 1024) : streamer(buffer_size), crc() {};

    void process_data(std::vector<char> &vector);

    virtual void after_buffer_write() {};

    uint32_t get_crc_checksum() {
        return crc.get_checksum();
    }
};


#endif //PROJECT_STREAMER_H
