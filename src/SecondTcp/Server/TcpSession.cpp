#include "TcpSession.h"

TcpSession::object_pool_type TcpSession::m_msg_pool;

TcpSession::TcpSession(ios_type& ios, queue_type& q)
	:m_socket(ios)
	,m_strand(ios)
	,m_queue(q)
{

}

TcpSession::socket_type& TcpSession::Socket()
{
	return m_socket;
}

TcpSession::ios_type& TcpSession::IO_Service()
{
	return m_socket.get_io_service();
}

void TcpSession::Close()
{
	boost::system::error_code ignored_ec;
	m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	m_socket.close(ignored_ec);
}

void TcpSession::Start()
{
	tcp_request_ptr req = _CreateRrequest();
	_Read(req);
}

tcp_request_ptr TcpSession::_CreateRrequest()
{
	return m_msg_pool.construct(shared_from_this(),
		boost::bind(&object_pool_type::destroy, ref(m_msg_pool), _1));
}

void TcpSession::_Read(tcp_request_ptr req)
{
	async_read(m_socket, 
		boost::buffer(req->HeadData().data(), req->HeadData().size()),
		m_strand.wrap(boost::bind(&TcpSession::_HandleReadHead, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			req)));
}

void TcpSession::_HandleReadHead(const boost::system::error_code& err, size_t bytes_transferred, tcp_request_ptr req)
{
	if (err || !req->CheckHead())
	{
		Close();
		return;
	}

	async_read(m_socket, buffer(req->MsgData().data(), req->GetHead()->size),
		m_strand.wrap())

}

void TcpSession::_HandleWriteMsg(const boost::system::error_code& err, size_t bytes_transferred, tcp_request_ptr req)
{

}

void TcpSession::Write(tcp_respone_ptr resp)
{

}

void TcpSession::_HandleWriteHead(const boost::system::error_code& err, size_t bytes_transferred, tcp_request_ptr req)
{

}

void TcpSession::_HandleWriteMsg(const boost::system::error_code& err, size_t bytes_transferred, tcp_request_ptr req)
{

}