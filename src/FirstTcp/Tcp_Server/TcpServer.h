#ifndef __TCP_SERVER_20120514_H__
#define __TCP_SERVER_20120514_H__

#include <boost/smart_ptr.hpp>

#include "TcpSession.h"
#include "IoServicePool.h"

class Tcp_Server
{

#pragma region typedef

public:
	typedef IO_Service_Pool::ios_type ios_type;
	typedef boost::asio::ip::tcp::acceptor acceptor_type;
	typedef boost::shared_ptr<Tcp_Session> tcp_session_ptr;

#pragma endregion typedef

public:
	Tcp_Server(short port, int n = 1);
	
	void Start();
	void Run();

private:
	void _StartAccept();
	void _HandleAccept(const boost::system::error_code& err,
		tcp_session_ptr session);

protected:
private:
	IO_Service_Pool m_ios_pool;
	acceptor_type m_accepter;
};

#endif