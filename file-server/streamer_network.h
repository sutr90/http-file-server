

#ifndef PROJECT_NETWORK_STREAMER_H
#define PROJECT_NETWORK_STREAMER_H


#include "../zip-stream/zip_streamer.h"
#include "throttle.h"

class streamer_network : public streamer {
    friend class zip_streamer_network;

private:
    throttle &t;
protected:

    void process_data(std::vector<char> &) {};

    virtual void after_buffer_write();
public:
    streamer_network(throttle &throttle, uint32_t chunk_size) : streamer(chunk_size), t(throttle) {};
};

class zip_streamer_network : public zip_streamer_file {
private:
    streamer_network &net_streamer;
public:
    zip_streamer_network(streamer_network &net_streamer) : net_streamer(net_streamer) {}

protected:
    void after_buffer_write() {
        net_streamer.after_buffer_write();
    }
};


#endif //PROJECT_NETWORK_STREAMER_H
