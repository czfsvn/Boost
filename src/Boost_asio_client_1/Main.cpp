#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "TypeDef.h"

using namespace boost;
using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> SocketPtr;

class Client
{
public:
	Client(io_service& io) : m_ios(io)
		,m_endPoint(ip::address::from_string("127.0.0.1"), 6688)
	{
		Start();
	}

	void Start()
	{
		SocketPtr sockPtr(new ip::tcp::socket(m_ios));
		sockPtr->async_connect(m_endPoint,
			boost::bind(&Client::ConnectHandler, this, placeholders::error, sockPtr));
	}

	void ConnectHandler(const system::error_code& ec, SocketPtr sock)
	{
		if (ec)
		{
			return;
		}

		shared_ptr<std::vector<char>> str(new std::vector<char>(100, 0));

		sock->async_read_some(buffer(*str),
			boost::bind(&Client::ReadHandler, this, placeholders::error, str));
	}

	void ReadHandler(const system::error_code& ec, shared_ptr<std::vector<char>> str)
	{
		if (ec)
		{
			return;
		}

		std::cout<<&(*str)[0]<<std::endl;
	}

protected:
private:
	io_service& m_ios;
	ip::tcp::endpoint m_endPoint;
};


void main()
{
	try
	{
		std::cout<<"client start..."<<std::endl;

		io_service ios;
		Client client(ios);
		ios.run();
	}
	catch (std::exception& e)
	{
		std::cout<<e.what()<<std::endl;
	}
}