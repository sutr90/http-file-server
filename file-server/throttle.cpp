#include <dlib/misc_api.h>
#include "throttle.h"

void throttle::sleep() {
    if (sleep_time > 1) {
        dlib::sleep(sleep_time);
    }
}

void throttle::set_sleep_time() {
    speed_limit = speed_limit * 1024 * 1024;
    float speed_bytes = speed_limit / 8.0f; // speed limit in bytes
    float chunk_count = speed_bytes / chunk_size; // number of chunks per second
    sleep_time = (dlib::uint32) (1000 / chunk_count);
}

const float throttle::MAX_SPEED = 10.f;

throttle::throttle(float speed_limit_mbit, dlib::uint32 chunk_size_byte) :
        speed_limit(std::max(speed_limit_mbit, MAX_SPEED)),
        chunk_size(chunk_size_byte) {
    set_sleep_time();
}
