#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "TypeDef.h"

#define MAX_LENGTH			1024

using namespace boost;
using namespace boost::asio;

#pragma region Session

class Session// : public boost::enable_shared_from_this<Session>
{
public:
	Session(asio::io_service& ios) : m_socket(ios)
	{

	}

	~Session()
	{
		m_socket.close();
	}

	ip::tcp::socket& GetSocket()
	{
		return m_socket;
	}

	void Start()
	{
		m_socket.async_read_some(asio::buffer(m_data, MAX_LENGTH),
			boost::bind(&Session::_HandleRead, this, 
			placeholders::error,
			placeholders::bytes_transferred));
	}

private:
	void _HandleRead(const system::error_code& err, size_t bytes_trandferred)
	{
		if (!err)
		{
			asio::async_write(m_socket,
				buffer(m_data, bytes_trandferred),
				bind(&Session::_HandleWrite, this,
				placeholders::error));
		}
		else
		{
			delete this;
		}
	}

	void _HandleWrite(const system::error_code& err)
	{
		Start();
	}

protected:
private:
	ip::tcp::socket m_socket;
	char m_data[MAX_LENGTH];
};

#pragma endregion Session

#pragma region Server
class Server : public enable_shared_from_this<Server>
{
public:
	Server(io_service& ios, short port) : m_ioService(ios)
		,m_acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), port))
	{
		Start();
	}

	void Start()
	{
		m_newSessionPtr.reset(new Session(m_ioService));
		m_acceptor.async_accept(m_newSessionPtr->GetSocket(),
			bind(&Server::_HandleAccept, shared_from_this(),
			m_newSessionPtr,
			placeholders::error));
	}

private:
	void _HandleAccept(shared_ptr<Session> sessionPtr, const system::error_code& err)
	{
		if (!err)
		{
			sessionPtr->Start();
			Start();
		}
		else
		{
			m_newSessionPtr.reset();
		}
	}

protected:
private:
	ip::tcp::acceptor m_acceptor;
	io_service& m_ioService;
	shared_ptr<Session> m_newSessionPtr;
};
#pragma endregion Server

int32 main(int32 argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr<<"Usage: async_tcp_echo_server <port>"<<std::endl;
			return 1;
		}

		io_service ios;
		shared_ptr<Server> s(new Server(ios, atoi(argv[1])));
		ios.run();
	}
	catch (std::exception& e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
	}
	return 0;
}