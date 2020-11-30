#include <iostream>
#include <boost/bind.hpp>

#include "Client.h"


Client::Client(boost::asio::io_service& ios, boost::asio::ip::tcp::endpoint& ep) : m_socket(ios)
	,m_endPoint(ep)
{
	_Connect();
}

void Client::_Connect()
{
	m_socket.async_connect(m_endPoint, 
		boost::bind(&Client::_HandleConnected, this,
		boost::asio::placeholders::error));
}

void Client::_HandleConnected(const boost::system::error_code& err)
{
	if (!err)
	{
		_InputMsg();
	}
	else
	{
		m_socket.close();
		_Connect();
	}
}

void Client::_InputMsg()
{
	//while (1)
	{
		std::string msg;
		std::cin>>msg;

		boost::asio::async_write(m_socket, 
			boost::asio::buffer(msg.c_str(), msg.size()),
			boost::bind(&Client::_HandleWrite, this, msg, 
			boost::asio::placeholders::error));
	}
}

void Client::_HandleWrite(std::string& msg, const boost::system::error_code& err)
{
	if (!err)
	{
		std::cout<<"Send	: "<<msg<<std::endl;
		/*m_socket.async_read_some(boost::asio::buffer(msg), 
			boost::bind(&Client::_HandleRead, this, msg,
			boost::asio::placeholders::error));

		boost::asio::async_read(m_socket, boost::asio::buffer(msg),
			boost::bind(&Client::_HandleRead, this, msg,
			boost::asio::placeholders::error));*/

		/*
		boost::asio::async_read(m_socket, 
			boost::asio::buffer(msg), 
			boost::bind(&Client::_HandleRead, this, msg,
			boost::asio::placeholders::error));*/
	}	
}

void Client::_HandleRead(std::string& msg, const boost::system::error_code& err)
{
	if (!err)
	{
		std::cout<<"Receive	: "<<msg<<std::endl;
		_InputMsg();
	}
}


