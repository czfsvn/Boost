#ifndef __TCPSESSION_20120515_H__
#define __TCPSESSION_20120515_H__

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/pool/object_pool.hpp>

#include "TcpMessage.h"
#include "JobQueue.h"

class TcpSession : public boost::enable_shared_from_this<TcpSession>
{

#pragma region typedef

public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::asio::io_service ios_type;
	
	typedef ios_type::strand strand_type;
	typedef JobQueue<tcp_request_ptr> queue_type;
	typedef boost::object_pool<Tcp_Message> object_pool_type;

#pragma endregion typedef

#pragma region public functions

public:
	TcpSession(ios_type& ios, queue_type& q);

	socket_type& Socket();
	ios_type& IO_Service();

	void Start();
	void Close();
	void Write(tcp_respone_ptr resp);

#pragma endregion public functions

#pragma region private functions

	tcp_request_ptr _CreateRrequest();
	void _Read(tcp_request_ptr req);

	void _HandleReadHead(const boost::system::error_code& err,
		size_t bytes_transferred,
		tcp_request_ptr req);

	void _HandleReadMsg(const boost::system::error_code& err,
		size_t bytes_transferred,
		tcp_request_ptr req);

	void _HandleWriteHead(const boost::system::error_code& err,
		size_t bytes_transferred,
		tcp_request_ptr req);

	void _HandleWriteMsg(const boost::system::error_code& err,
		size_t bytes_transferred,
		tcp_request_ptr req);

#pragma endregion private functions

#pragma region private members

private:
	socket_type m_socket;
	strand_type m_strand;
	queue_type& m_queue;
	static object_pool_type m_msg_pool;

#pragma endregion private members
};

#endif