#include <iostream>

#include <boost/asio.hpp>

#include "TypeDef.h"

using namespace boost::asio;

void TestBoost_socket()
{
	try
	{
		std::cout<<"Boost server start..."<<std::endl;
		io_service ios;
		ip::tcp::acceptor acept(ios, ip::tcp::endpoint(ip::tcp::v4(), 6688));

		std::cout<<acept.local_endpoint().address()<<std::endl;

		while (true)
		{
			ip::tcp::socket sock(ios);
			acept.accept(sock);

			std::cout<<"client:";
			std::cout<<sock.remote_endpoint().address()<<std::endl;

			sock.write_some(buffer("Hello, asio Client!"));
		}
	}
	catch (std::exception& e)
	{
		std::cout<<e.what()<<std::endl;
	}
}

int32 main()
{
	TestBoost_socket();
	return 0;
}

