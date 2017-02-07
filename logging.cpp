#include <dlib/logger.h>
#include "logging.h"

dlib::logger logp("L");

void setup_logging(){
    logp.set_level(dlib::LALL);
}
