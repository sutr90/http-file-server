#include "MyServer.h"
#include "../logging.h"

using namespace dlib;
using namespace std;

const dlib::logger logan("L.main");

int main() {
    try {
        setup_logging();
        config_reader cr("config");
        configure_loggers_from_file (cr);
        server_config config(cr);

        MyServer file_server(config);

        logan << LINFO << "Starting server on port " << config.port;
        file_server.set_listening_port(config.port);
        file_server.start();
    }
    catch (exception &e) {
        logan << LERROR << "Error " << e.what();
    }
}
