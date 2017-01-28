#include "MyServer.h"

using namespace dlib;
using namespace std;

int main() {
    try {
        MyServer our_web_server("../../server.db");

        our_web_server.set_listening_port(5000);
        our_web_server.start_async();

        cout << "Press enter to end this program" << endl;
        cin.get();
    }
    catch (exception &e) {
        cout << e.what() << endl;
    }
}
