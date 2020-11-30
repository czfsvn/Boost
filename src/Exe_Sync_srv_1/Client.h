#ifndef __CLINET_20120316_H__
#define __CLINET_20120316_H__

#include <iostream>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

#define MAX_LENGTH			1024

class Server;

class Client : public boost::enable_shared_from_this<Client>
{
public:
	Client(boost::asio::io_service& ios, Server& srv);

	void SendMsg(char* msg);
	boost::asio::ip::tcp::socket& GetSocket();

private:
	void _Start();

	void _HandleRead(const boost::system::error_code& err);

	void _HandleWrite(const boost::system::error_code& err);

protected:
private:
	boost::asio::ip::tcp::socket m_socket;
	char* m_data[MAX_LENGTH];
	Server& m_srv;
};

#endif
