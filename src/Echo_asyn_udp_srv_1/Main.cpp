#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "TypeDef.h"

#define MAX_LENGTH				1024

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

 class Server
 {
 public:
	 Server(io_service& ios, int16 port) : m_ioService(ios)
		 ,m_socket(ios, udp::endpoint(udp::v4(), port))
	 {
		 _Start();
	 }


 private:
	 void _Start()
	 {
		 m_socket.async_receive_from(buffer(m_data, MAX_LENGTH),
			 m_sendEp,
			 bind(&Server::_HandleRcvFrom, this, 
			 placeholders::error,
			 placeholders::bytes_transferred));
	 }

	 void _HandleRcvFrom(const system::error_code& err, size_t bytesRcvd)
	 {
		 if (!err && bytesRcvd > 0)
		 {
			 m_socket.async_send_to(buffer(m_data, bytesRcvd),
				 m_sendEp,
				 bind(&Server::_HandleSendTo, this, 
				 placeholders::error,
				 placeholders::bytes_transferred));
		 }
		 else
		 {
			 _Start();
		 }
	 }

	 void _HandleSendTo(const system::error_code& err, size_t bytesSend)
	 {
		 _Start();
	 }

 protected:
 private:
	 io_service& m_ioService;
	 udp::socket m_socket;
	 udp::endpoint m_sendEp;
	 char m_data[MAX_LENGTH];
 };

int32 main(int32 argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: async_udp_echo_server <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		Server s(io_service, atoi(argv[1]));

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}