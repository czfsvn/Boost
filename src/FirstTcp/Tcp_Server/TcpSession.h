#ifndef __TCP_SESSION_20120515_H__
#define __TCP_SESSION_20120515_H__

#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "TcpBuffer.h"

class Tcp_Session : public boost::enable_shared_from_this<Tcp_Session>
{

#pragma region typdef

public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::asio::io_service ios_type;
	typedef Tcp_Buffer buffer_type;
	typedef std::size_t size_type;

#pragma endregion typdef

#pragma region public functions

public:
	Tcp_Session(ios_type& ios);

	socket_type& Socket();
	ios_type& IO_Service();
	buffer_type& Read_Buf();
	buffer_type& Write_Buf();

	void Start();
	void Close();

	void Write();
	void Write(const void* data, size_type len);

#pragma endregion public functions

#pragma region private functions

	void _Read();
	void _Handle_Read(const boost::system::error_code& err,
		size_type bytes_transferred);

	void _Handle_Write(const boost::system::error_code& err,
		size_type bytes_transferred);

#pragma endregion private functions

#pragma region private members

private:
	socket_type m_socket;
	buffer_type m_read_buf;
	buffer_type m_write_buf;

#pragma endregion private members
};



#endif