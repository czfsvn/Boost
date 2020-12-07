#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

const uint32_t MAX_PACK_SIZE = 65536;
const uint32_t HEAD_LEN = 4;
const uint32_t MAX_CONNECTION = 65536;

using namespace std;
using namespace boost::asio::ip;

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
			if(err)
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
		if(err)
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
		if(m_cb)
			m_cb(m_conn_id);
	}

private:
	boost::asio::ip::tcp::socket m_socket;
	std::array<char, MAX_PACK_SIZE> m_buff;
	uint32_t m_conn_id = 0;
	std::function<void(int)> m_cb;
};

namespace ns_client1
{
	class Connector
	{
	public:
		Connector(boost::asio::io_service& ios, const std::string& ip, uint16_t port): m_ios(ios)
			, m_socket(ios)
			, m_server_addr(tcp::endpoint(address::from_string(ip), port))
		{
			createEventHandler();
		}

		~Connector() {}

		bool start()
		{
			m_event_handler->getSocket().async_connect(m_server_addr,
				[this](const boost::system::error_code& err)
			{
				if(err)
				{
					handlerConnErr(err);
					return;
				}

				std::cout << "connect Ok\n";
				m_is_connected = true;
				m_event_handler->handleRead();
			});
			boost::this_thread::sleep(boost::posix_time::seconds(1));
			return m_is_connected;
		}

		bool isConnected() { return m_is_connected; }
		void send(char* data, uint32_t len)
		{
			if(!isConnected())
				return;

			m_event_handler->handleWrite(data, len);
		}
	private:
		void handlerConnErr(const boost::system::error_code& err)
		{
			m_is_connected = false;
			std::cout << "connect Error: " << err.message() << std::endl;
			m_event_handler->closeSocket();
			checkConnected();
		}

		void checkConnected()
		{
			if(m_ck_thrd != nullptr)
				return;

			m_ck_thrd = boost::make_shared<std::thread>([this]
			{
				while (1)
				{
					if(!isConnected())
						start();

					boost::this_thread::sleep(boost::posix_time::seconds(1));
				}
			});
		}

		void createEventHandler()
		{
			m_event_handler = boost::make_shared<RWHandler>(m_ios);
			m_event_handler->setCBErr([this](uint32_t connid)
			{
				m_is_connected = false;
				checkConnected();
			});
		}


	protected:
	private:
		boost::asio::io_service& m_ios;
		boost::asio::ip::tcp::socket m_socket;
		boost::asio::ip::tcp::endpoint m_server_addr = {};
		boost::shared_ptr<RWHandler> m_event_handler = {};
		boost::shared_ptr<std::thread> m_ck_thrd = {};
		bool m_is_connected = false;
	};
}