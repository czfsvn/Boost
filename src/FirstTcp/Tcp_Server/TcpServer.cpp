

#include "TcpServer.h"


Tcp_Server::Tcp_Server(short port, int n /* = 1 */) 
	: m_ios_pool(n)
	,m_accepter(m_ios_pool.Get(), 
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	_StartAccept();
}

void Tcp_Server::_StartAccept()
{
	tcp_session_ptr session = boost::factory<tcp_session_ptr>()(m_ios_pool.Get());

	m_accepter.async_accept(session->Socket(),
		boost::bind(&Tcp_Server::_HandleAccept, 
		this, 
		boost::asio::placeholders::error, 
		session));
}

void Tcp_Server::_HandleAccept(const boost::system::error_code& err, tcp_session_ptr session)
{
	_StartAccept();

	if (err)
	{
		session->Close();
		return;
	}

	session->Start();
}

void Tcp_Server::Start()
{
	m_ios_pool.Start();
}

void Tcp_Server::Run()
{
	m_ios_pool.Run();
}
