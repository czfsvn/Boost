#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "TypeDef.h"

using namespace boost;
using namespace boost::asio;


typedef boost::shared_ptr<ip::tcp::socket> SocketPtr;


class Server
{
public:
	Server(io_service& io) : m_ios(io)
		,m_acceptor(m_ios, ip::tcp::endpoint(ip::tcp::v4(), 6688))
	{
		Start();
	}

	void Start()
	{
		SocketPtr sockPtr(new ip::tcp::socket(m_ios)); 

		m_acceptor.async_accept(*sockPtr, bind(&Server::AcceptHandler, this, placeholders::error, sockPtr));
	}

	void AcceptHandler(const system::error_code& ec, SocketPtr sock)
	{
		if (ec)
		{
			return;
		}

		std::cout<<"client start";
		std::cout<<sock->remote_endpoint().address()<<std::endl;

		sock->async_write_some(buffer("Hello, boost asio asyn client!"), 
			bind(&Server::WriteHandler, this, placeholders::error));

		Start();
	}

	void WriteHandler(const system::error_code& )
	{
		std::cout<<"send msg complete"<<std::endl;
	}

protected:
private:
	io_service& m_ios;
	ip::tcp::acceptor m_acceptor;
	SocketPtr m_sockPtr;
};



int32 main()
{
	try
	{
		std::cout<<"server start..."<<std::endl;

		io_service ios;
		Server srv(ios);
		ios.run();
	}
	catch (std::exception& e)
	{
		std::cout<<e.what()<<std::endl;
	}
	return 0;
}