#include <iostream>

#include <boost/asio.hpp>

#include "TypeDef.h"

#define MAX_LENGTH				1024

using namespace boost::asio::ip;


int32 main(int32 argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
			return 1;
		}

		boost::asio::io_service ios;
		
		tcp::resolver resolver(ios);
		tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
		tcp::resolver::iterator iterator = resolver.resolve(query);

		tcp::socket sock(ios);
		sock.connect(*iterator);

		std::cout<<"Enter msg:";
		char request[MAX_LENGTH];
		std::cin.getline(request, MAX_LENGTH);
		size_t req_length = strlen(request);
		boost::asio::write(sock, boost::asio::buffer(request, req_length));

		char reply[MAX_LENGTH];
		size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, req_length));
		std::cout<<"Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout<<"\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}