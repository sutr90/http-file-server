//
// Created by VF on 27.01.2017.
//

#ifndef SERVER_THROTTLE_H
#define SERVER_THROTTLE_H

#include <dlib/uintn.h>

class throttle {
private:
    float speed_limit;
    dlib::uint32 chunk_size;
    dlib::uint32 sleep_time;
    void set_sleep_time();
    const float MAX_SPEED = 10;
public:
    throttle(float speed_limit_mbit, dlib::uint32 chunk_size_byte);
    void sleep();
};


#endif //SERVER_THROTTLE_H
