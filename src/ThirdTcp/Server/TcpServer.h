#include <iostream>
#include <unordered_map>
#include <boost/asio.hpp>
#include <stdio.h>
#include <string>
#include <boost/smart_ptr.hpp>

const uint32_t MAX_PACK_SIZE = 65536;

const uint32_t MAX_CONNECTION = 65536;

const uint32_t HEAD_LEN = 4;
const uint32_t BODY_LEN = 512;

class RWHandler
{
public:
	RWHandler(boost::asio::io_service& ios): m_socket(ios)
	{

	}

	~RWHandler() {}

	void handleRead()
	{
		boost::asio::async_read(m_socket, boost::asio::buffer(m_buff), boost::asio::transfer_at_least(HEAD_LEN),
			[this](const boost::system::error_code& err, size_t size) 
		{
			if (err)
			{
				handleErr(err);
				return;
			}
			std::cout << m_buff.data() + HEAD_LEN << std::endl;
		});
	}

	void handleWrite(char* data, int32_t len)
	{
		boost::system::error_code err;
		boost::asio::write(m_socket, boost::asio::buffer(data, len), err);
		if (err)
			handleErr(err);
	}

	boost::asio::ip::tcp::socket& getSocket() { return m_socket; }

	void closeSocket()
	{
		boost::system::error_code err;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, err);
		m_socket.close(err);
	}

	void setConnID(uint32_t con) { m_conn_id = con; }
	uint32_t getConnID() { return m_conn_id; }

	template<typename F>void setCBErr(F f) { m_cb = f; }

private:
	void handleErr(const boost::system::error_code& err)
	{
		closeSocket();
		std::cout << "err: " << err.message() << std::endl;
		if (m_cb)
			m_cb(m_conn_id);
	}

private:
	boost::asio::ip::tcp::socket m_socket;
	std::array<char, MAX_PACK_SIZE> m_buff;
	uint32_t m_conn_id = 0;
	std::function<void(int)> m_cb;
};

class Message
{
public:
	Message()
	{
	}

	~Message()
	{
	}

	const char* data() const { return datas; }
	char* data() { return datas; }

	const char* body() const { return datas + HEAD_LEN; }
	char* body() { return datas + body_len_; }

	uint32_t length() { return HEAD_LEN + body_len_; }
	uint32_t body_len()const { return body_len_; }
	void body_len(uint32_t len)
	{
		body_len_ = len;
		if(body_len_ > BODY_LEN)
			body_len_ = BODY_LEN;
	}

	bool decode_header()
	{
		char header[HEAD_LEN + 1] = {};
		strncat_s(header, datas, HEAD_LEN);
		body_len_ = atoi(header) - HEAD_LEN;
		if(body_len_ > BODY_LEN)
		{
			body_len_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		char header[HEAD_LEN + 1] = {};
		sprintf_s(header, "%4d", body_len_);
		std::memcpy(datas, header, HEAD_LEN);
	}
	

private:
	char datas[HEAD_LEN + BODY_LEN] = {};
	uint32_t body_len_ = 0;
};

class RWHandler2 : public boost::enable_shared_from_this<RWHandler2>
{
public:
	RWHandler2(boost::asio::io_service& ios):m_socket(ios)
	{
	}
	~RWHandler2()
	{

	}

	void readHead()
	{
		async_read(m_socket, boost::asio::buffer(m_read_msg.data(), HEAD_LEN),
			[this](const boost::system::error_code& ec, uint32_t size)
		{
			if(ec || !m_read_msg.decode_header())
			{
				handleErr(ec);
				return;
			}
		});
		readBody();
	}

	void readBody()
	{
		async_read(m_socket, boost::asio::buffer(m_read_msg.body(), m_read_msg.body_len()),
			[this](const boost::system::error_code& ec, uint32_t size)
		{
			if(ec || !m_read_msg.decode_header())
			{
				handleErr(ec);
				return;
			}

			callBack(m_read_msg.data(), m_read_msg.length());
			readHead();
		});
	}

	void handleWrite(char* data, uint32_t len)
	{
		boost::system::error_code ec;
		write(m_socket, boost::asio::buffer(data, len), ec);
		if(ec)
			handleErr(ec);
	}

	boost::asio::ip::tcp::socket& getSocket() { return m_socket; }

	void closeSocket()
	{
		boost::system::error_code ec;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		m_socket.close(ec);
	}
	void setConnID(uint32_t con) { m_conn_id = con; }
	uint32_t getConnID() { return m_conn_id; }

	template<typename F>void setCBErr(F f) { m_cb = f; }

private:
	void callBack(char* data, uint32_t len)
	{
		std::cout << data + HEAD_LEN << std::endl;
	}

	void handleErr(const boost::system::error_code& ec)
	{
		closeSocket();
		std::cout << "handler err: " << ec.value() << ", msg: " << ec.message() << std::endl;
		if(m_cb)
			m_cb(m_conn_id);
	}

protected:
private:
	boost::asio::ip::tcp::socket m_socket;
	std::array<char, MAX_PACK_SIZE> m_buffer = {};
	uint32_t m_conn_id = 0;
	std::function<void(int)> m_cb;
	Message m_read_msg = {};
};


namespace ns_server1
{
	class Server
	{
	public:
		Server(boost::asio::io_service& ios, uint16_t port):m_ios(ios)
			, m_acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
		{

		}

		~Server() {}

		void accept()
		{
			std::cout << "start listen\n";
			std::shared_ptr<RWHandler> handler = createHandler();
			m_acceptor.async_accept(handler->getSocket(),
				[this, &handler](const boost::system::error_code& err)
			{
				if(err)
				{
					std::cout << "err: " << err.value() << ", msg: " << err.message() << std::endl;
					handleAcpErr(handler, err);
					return;
				}
				m_handlers.emplace(handler->getConnID(), handler);
				std::cout << "current connect count: " << m_handlers.size() << std::endl;
				handler->handleRead();
				accept();
			});
		}

	private:

		void handleAcpErr(std::shared_ptr<RWHandler> handler, const boost::system::error_code& err)
		{
			std::cout << "current connect count: " << m_handlers.size() << std::endl;
			handler->closeSocket();
			stopAccept();
		}

		void stopAccept()
		{
			boost::system::error_code err;
			m_acceptor.cancel(err);
			m_acceptor.close(err);
			m_ios.stop();
		}

		std::shared_ptr<RWHandler> createHandler()
		{
			uint32_t conn_id = 0;
			if(m_conn_pool.empty())
			{
				conn_id = ++connection_idx;
			}
			else
			{
				conn_id = m_conn_pool.front();
				m_conn_pool.pop_front();
			}

			std::shared_ptr<RWHandler> handler = std::make_shared<RWHandler>(m_ios);
			handler->setConnID(conn_id);
			handler->setCBErr([this](uint32_t connid)
			{
				recycleConnid(connid);
			});
			return handler;
		}

		void recycleConnid(uint32_t conn_id)
		{
			auto iter = m_handlers.find(conn_id);
			if(iter != m_handlers.end())
				m_handlers.erase(iter);
			std::cout << "current connect count: " << m_handlers.size() << std::endl;
			m_conn_pool.emplace_back(conn_id);
		}

	private:
		boost::asio::io_service& m_ios;
		boost::asio::ip::tcp::acceptor m_acceptor;
		std::unordered_map<uint32_t, std::shared_ptr<RWHandler>> m_handlers = {};
		std::list<uint32_t> m_conn_pool = {};
		uint32_t connection_idx = 0;
	};
}

namespace ns_server2
{

}



