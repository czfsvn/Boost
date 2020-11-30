#ifndef __SERVER_20120316_H__
#define __SERVER_20120316_H__

#include <list>

#include <boost/shared_ptr.hpp>

#include "Client.h"

typedef boost::shared_ptr<Client> ClientPtr;
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketPtr;


class Server : public boost::enable_shared_from_this<Server>
{
public:
	Server(boost::asio::io_service& ios, short port);

	~Server();

	void BroadCast(char* msg);
	void OnDisconnect(Client* cl);

private:
	void _Start();
	void _OnAccept(ClientPtr ptr, const::boost::system::error_code& err);

protected:
private:
	boost::asio::io_service& m_ioService;
	boost::asio::ip::tcp::acceptor m_acceptor;

	boost::asio::ip::tcp::socket m_socket;

	ClientPtr client;

	std::list<ClientPtr> m_clientList;
};

#endif