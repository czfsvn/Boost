#include "Client.h"
#include "Server.h"

Client::Client(boost::asio::io_service& ios, Server& srv) : m_srv(srv)
	,m_socket(ios)
{

}

void Client::SendMsg(char* msg)
{
	m_socket.async_write_some(boost::asio::buffer(msg, strlen(msg)),
		bind(&Client::_HandleWrite, shared_from_this(), 
		boost::asio::placeholders::error));
}

boost::asio::ip::tcp::socket& Client::GetSocket()
{
	return m_socket;
}

void Client::_Start()
{
	m_socket.async_read_some(boost::asio::buffer(m_data, MAX_LENGTH),
		bind(&Client::_HandleRead, shared_from_this(), 
		boost::asio::placeholders::error));
}

void Client::_HandleRead(const boost::system::error_code& err)
{
	if (!err)
	{
		std::cout<<"Display: "<<m_data<<std::endl;
		m_srv.BroadCast(m_data[0]);
		memset(m_data, 0, MAX_LENGTH);
	}
	else
	{
		m_srv.OnDisconnect(this);
	}	
}

void Client::_HandleWrite(const boost::system::error_code& err)
{
	if (!err)
	{
// 		std::cout<<"Display: "<<m_data<<std::endl;
// 		m_srv.BroadCast(m_data[0]);
// 		memset(m_data, 0, MAX_LENGTH);
	}
	else
	{
		m_srv.OnDisconnect(this);
	}	
}
