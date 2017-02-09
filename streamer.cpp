#include "streamer.h"

void streamer::stream_data(std::istream &in, std::ostream &out) {
    std::vector<char> buffer_vec(buffer_size);

    while (in.good()) {
        in.read(&buffer_vec[0], buffer_size);
        uint32_t bytes = (uint32_t) in.gcount();
        buffer_vec.resize(bytes);
        out.write(&buffer_vec[0], bytes);
        process_data(buffer_vec);
        after_buffer_write();
    }
}
