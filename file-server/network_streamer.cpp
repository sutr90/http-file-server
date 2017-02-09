#include "network_streamer.h"

void streamer_network::after_buffer_write() {
    t.sleep();
}
