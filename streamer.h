

#ifndef PROJECT_STREAMER_H_H
#define PROJECT_STREAMER_H_H

#include <istream>
#include <vector>

class streamer {
private:
    uint32_t buffer_size;
protected:
    streamer(uint32_t buf_size) : buffer_size(buf_size) {};

    void stream_data(std::istream &in, std::ostream &out);

    virtual void process_data(std::vector<char> &vector) = 0;

    virtual void after_buffer_write() = 0;
};

#endif //PROJECT_STREAMER_H_H
