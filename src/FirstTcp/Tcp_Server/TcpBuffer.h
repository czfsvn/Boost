#ifndef __TCPBUFFER_20120515_H__
#define __TCPBUFFER_20120515_H__

#include <boost/asio.hpp>
//#include <boost/config/suffix.hpp>
#include <boost/cast.hpp>

using namespace std;

class Tcp_Buffer
{
#pragma region typedef

public:
	typedef std::size_t size_type;
	typedef boost::asio::streambuf streambuf_type;
	typedef streambuf_type::const_buffers_type const_buffers_type;
	typedef streambuf_type::mutable_buffers_type mutable_buffers_type;	

#pragma endregion typedef

#pragma region public functions

public:
	mutable_buffers_type Prepare(size_type n = BUF_SIZE)
	{
		return m_buf.prepare(n);
	}

	void Retrive(size_type n)
	{
		m_buf.commit(n);
	}

	size_type Size() const
	{
		return m_buf.size();
	}

	const char* Peek() const
	{
		return boost::asio::buffer_cast<const char*>(m_buf.data());
	}

	void Append(const void* data, size_type len)
	{
		m_buf.sputn(static_cast<const char*>(data), boost::numeric_cast<std::streamsize>(len));
	}
	
	const_buffers_type Data() const
	{
		return m_buf.data();
	}

	void Comsume(size_type n)
	{
		m_buf.consume(n);
	}

#pragma endregion public functions

#pragma region private members

private:
	BOOST_STATIC_CONSTANT(size_type, BUF_SIZE = 512);
	streambuf_type m_buf;

#pragma endregion private members

};


#endif