#include "streamer_network.h"

void streamer_network::after_buffer_write() {
    t.sleep();
}
