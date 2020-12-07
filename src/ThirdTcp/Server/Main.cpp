#include <iostream>

#include "TcpServer.h"

using namespace std;

void testServer()
{
	boost::asio::io_service ios;

	ns_server1::Server server(ios, 9001);
	server.accept();
	ios.run();
}

int main()
{
	testServer();
	return 0;
}