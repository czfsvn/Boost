#include "AppServer.h"
#include <iostream>

using namespace std;

int main()
{
    std::cout << "Hello, TCPServer\n";
    AppServer::get_mutable_instance().init();
    AppServer::get_mutable_instance().run();
    AppServer::get_mutable_instance().destroy();
    return 0;
}