#include <vector>
#include "zip_streamer.h"


void zip_streamer_file::process_data(std::vector<char> &vector) {
    crc.add(vector);
}
