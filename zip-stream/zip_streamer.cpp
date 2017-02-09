#include <vector>
#include "zip_streamer.h"


void zip_streamer_file::process_data(std::vector<char> &vector) {
    crc.add(vector);
}

void zip_streamer_file::stream_data(std::istream &in, std::ostream &out) {
    crc.clear();
    streamer::stream_data(in, out);
}
