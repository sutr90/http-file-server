#include "MyServer.h"

using namespace dlib;
using namespace std;

int main() {
    try {
        // create an instance of our web server
        MyServer our_web_server("C:\\Users\\VF\\ClionProjects\\server\\server.db");

        // make it listen on port 5000
        our_web_server.set_listening_port(5000);
        // Tell the server to begin accepting connections.
        our_web_server.start_async();

        cout << "Press enter to end this program" << endl;
        cin.get();
    }
    catch (exception &e) {
        cout << e.what() << endl;
    }
}
