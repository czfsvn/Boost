#ifndef __CLINET_20120316_H__
#define __CLINET_20120316_H__

#include <boost/asio.hpp>
//#include <boost/enable_shared_from_this.hpp>


class Client// : public enable_shared_from_this<Client>
{
public:
	Client(boost::asio::io_service& ios, boost::asio::ip::tcp::endpoint& ep);

private:
	void _Connect();

	void _HandleConnected(const boost::system::error_code& err);

	void _HandleRead(std::string& msg, const boost::system::error_code& err);
	void _HandleWrite(std::string& msg, const boost::system::error_code& err);

	void _InputMsg();

protected:
private:
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::ip::tcp::endpoint& m_endPoint;
};

#endif