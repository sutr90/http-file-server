

#ifndef PROJECT_NETWORK_STREAMER_H
#define PROJECT_NETWORK_STREAMER_H


#include "../zip-stream/zip_streamer.h"
#include "throttle.h"

class streamer_network : protected streamer {
    friend class zip_streamer_network;

private:
    throttle &t;
protected:
    streamer_network(throttle &throttle, uint32_t chunk_size) : streamer(chunk_size), t(throttle) {};

    void process_data(std::vector<char> &) {};

    void after_buffer_write();
};

class zip_streamer_network : protected zip_streamer_file {
private:
    streamer_network &net_streamer;
protected:
    zip_streamer_network(streamer_network &net_streamer) : net_streamer(net_streamer) {}

    void after_buffer_write() {
        net_streamer.after_buffer_write();
    }
};


#endif //PROJECT_NETWORK_STREAMER_H
