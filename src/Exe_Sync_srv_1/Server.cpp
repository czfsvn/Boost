#include "Server.h"


Server::Server(boost::asio::io_service& ios, short port) : m_ioService(ios)
	,m_socket(ios)
	,m_acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	_Start();
}

Server::~Server()
{

}

void Server::BroadCast(char* msg)
{
	std::for_each(m_clientList.begin(), m_clientList.end(), 
		boost::bind(&Client::SendMsg, _1, msg));
}

void Server::OnDisconnect(Client* cl)
{
	for (std::list<ClientPtr>::iterator iter = m_clientList.begin();
		iter != m_clientList.end(); iter++)
	{
		ClientPtr ptr = *iter;
		if (ptr.get() == cl)
		{
			m_clientList.erase(iter);
			return;
		}
	}
}

void Server::_Start()
{
	ClientPtr client(new Client(m_ioService, *this));
	m_acceptor.async_accept(client->GetSocket(), 
		boost::bind(&Server::_OnAccept, this,
		client, boost::asio::placeholders::error));
}

void Server::_OnAccept(ClientPtr ptr, const::boost::system::error_code& err)
{
	m_clientList.push_back(ptr);
	boost::asio::ip::tcp::endpoint ep = ptr->GetSocket().remote_endpoint();
	std::cout<<"connect from Ip = "<<ep.address()<<", port = "<<ep.port()<<std::endl;
	_Start();
}






	