#include "MyServer.h"

using namespace dlib;
using namespace std;

int main() {
    try {
        config_reader cr("config");
        server_config config(cr);

        MyServer file_server(config);

        file_server.set_listening_port(config.port);
        file_server.start();
    }
    catch (exception &e) {
        cout << e.what() << endl;
    }
}
