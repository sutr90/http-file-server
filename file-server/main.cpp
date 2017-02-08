#include <signal.h>
#include "MyServer.h"
#include "../logging.h"

using namespace dlib;
using namespace std;

const dlib::logger logan("L.main");

void my_handler(int s){
    logan << LINFO << "Caught signal " << s;

}

int main() {
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    try {
        setup_logging();
        config_reader cr("config");
        configure_loggers_from_file (cr);
        server_config config(cr);

        MyServer file_server(config);

        logan << LINFO << "Starting server on port " << config.port;
        file_server.set_listening_port(config.port);
        file_server.start_async();
        pause();
    }
    catch (exception &e) {
        logan << LERROR << "Error " << e.what();
    }
}
