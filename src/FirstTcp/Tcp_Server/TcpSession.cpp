#include <iostream>

#include <boost/bind.hpp>

#include "TcpSession.h"

Tcp_Session::Tcp_Session(ios_type& ios) : m_socket(ios)
{
}

Tcp_Session::socket_type& Tcp_Session::Socket()
{
	return m_socket;
}

Tcp_Session::ios_type& Tcp_Session::IO_Service()
{
	return m_socket.get_io_service();
}

Tcp_Session::buffer_type& Tcp_Session::Read_Buf()
{
	return m_read_buf;
}

Tcp_Session::buffer_type& Tcp_Session::Write_Buf()
{
	return m_write_buf;
}

void Tcp_Session::Start()
{
	std::cout<<"session start"<<std::endl;
	_Read();
}

void Tcp_Session::Close()
{
	boost::system::error_code ignored_ec;
	m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	m_socket.close(ignored_ec);
}

void Tcp_Session::_Read()
{
	m_socket.async_read_some(m_read_buf.Prepare(), 
		boost::bind(&Tcp_Session::_Handle_Read, 
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void Tcp_Session::_Handle_Read(const boost::system::error_code& err, size_type bytes_transferred)
{
	if (err)
	{
		Close();
		return;
	}

	std::cout<<"read size"<<bytes_transferred<<std::endl;
	m_read_buf.Retrive(bytes_transferred);
	std::cout<<string(m_read_buf.Peek(), bytes_transferred)<<std::endl;
	Write(m_read_buf.Peek(), bytes_transferred);
	m_read_buf.Comsume(bytes_transferred);
	_Read();
}

void Tcp_Session::Write(const void* data, size_type len)
{
	std::cout<<"write:"<<len<<std::endl;
	std::cout<<static_cast<const char*>(data)<<std::endl;

	m_write_buf.Append(data, len);
	Write();
}

void Tcp_Session::Write()
{
	m_socket.async_write_some(m_write_buf.Data(),
		boost::bind(&Tcp_Session::_Handle_Write,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void Tcp_Session::_Handle_Write(const boost::system::error_code& err, size_type bytes_transferred)
{
	if (err)
	{
		Close();
		return;
	}

	m_write_buf.Comsume(bytes_transferred);
	std::cout<<"write complete"<<std::endl;
}
