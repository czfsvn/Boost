#include "Server.h"

int main()
{
	boost::asio::io_service ios;
	boost::shared_ptr<Server> srv(new Server(ios, 5000));
	ios.run();
	return 0;
}

