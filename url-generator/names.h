#ifndef SERVER_NAMES_H
#define SERVER_NAMES_H

#include <string>
#include <dlib/rand.h>

class names {
private:
    static dlib::rand rng;

public:
    static const unsigned char ADJECTIVE_COUNT = 101;
    static const unsigned char ANIMAL_COUNT = 76;

    static const std::string animals[];
    static const std::string adjectives[];

    static std::string get_random_name();
};
#endif //SERVER_NAMES_H
