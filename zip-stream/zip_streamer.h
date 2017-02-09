#ifndef PROJECT_STREAMER_H
#define PROJECT_STREAMER_H

#include <ostream>
#include <istream>
#include <dlib/crc32.h>
#include "../streamer.h"

class zip_streamer_file : public streamer {
protected:
    dlib::crc32 crc;

    void process_data(std::vector<char> &vector);

    virtual void after_buffer_write() {};
public:
    uint32_t get_crc_checksum() {
        return crc.get_checksum();
    }

    zip_streamer_file(uint32_t buffer_size = 64 * 1024) : streamer(buffer_size), crc() {};

    virtual void stream_data(std::istream &in, std::ostream &out);
};


#endif //PROJECT_STREAMER_H
