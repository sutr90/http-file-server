#include <dlib/misc_api.h>
#include <dlib/logger.h>
#include "throttle.h"

using namespace dlib;
const dlib::logger logan("L.throttle");

void throttle::sleep() {
    if (sleep_time > 0) {
        dlib::sleep(sleep_time);
    }
}

void throttle::set_sleep_time() {
    logan << LTRACE << "throttle::set_sleep_time";
    if (speed_limit > 0) {
        logan << LINFO << "Setting speed limit to " << speed_limit << "mbit";
        speed_limit = speed_limit * 1024 * 1024;
        float speed_bytes = speed_limit / 8.0f; // speed limit in bytes
        float chunk_count = speed_bytes / chunk_size; // number of chunks per second
        sleep_time = (dlib::uint32) (1000 / chunk_count);
        logan << LINFO << "Sleeping for " << sleep_time << "ms after every chunk";
    }
}

throttle::throttle(float speed_limit_mbit, dlib::uint32 chunk_size_byte) :
        speed_limit(speed_limit_mbit),
        chunk_size(chunk_size_byte) {
    set_sleep_time();
}
