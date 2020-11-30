#include <iostream>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "TypeDef.h"

#define MAX_LENGTH						1024

using namespace boost::asio::ip;

typedef boost::shared_ptr<tcp::socket> SocketPtr;

void Session(SocketPtr sockPtr)
{
	try
	{
		for (;;)
		{
			char data[MAX_LENGTH];
			boost::system::error_code err;
			size_t length = sockPtr->read_some(boost::asio::buffer(data), err);		//正常执行， why？
			//size_t length = boost::asio::read(*sockPtr, boost::asio::buffer(data));   //无法执行， why？
			if (err == boost::asio::error::eof)
			{
				break;
			}
			else if (err)
			{
				throw boost::system::system_error(err);
			}

			//boost::asio::write(*sockPtr, boost::asio::buffer(data, length));
			sockPtr->write_some(boost::asio::buffer(data, length));

		}
		
	}
	catch (std::exception& e)
	{
		std::cerr<<"Exception in thread: "<<e.what()<<std::endl;
	}
}

void Server(boost::asio::io_service& ios, int16 port)
{
	tcp::acceptor acceptor(ios, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		SocketPtr sock(new tcp::socket(ios));
		acceptor.accept(*sock);
		tcp::endpoint ep = sock->remote_endpoint();
		std::cout<<"connection established ip = "<<ep.address()<<", port = "<<ep.port()<<std::endl;
		boost::thread th(boost::bind(Session, sock));
	}
}

int32 main(int32 argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr<<"Usage: blockint_tcp_echo_server<port>"<<std::endl;
			return 1;
		}

		boost::asio::io_service ios;
		Server(ios, atoi(argv[1]));

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}