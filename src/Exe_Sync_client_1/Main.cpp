#include "Client.h"
#include "TypeDef.h"

int16 main()
{
	boost::asio::io_service ios;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 5000);
	boost::shared_ptr<Client> cli(new Client(ios, ep));
	ios.run();
	return 0;
}