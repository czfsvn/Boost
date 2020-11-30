#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#pragma region DayTime_1

int DayTime_1(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: client <host>" << std::endl;
			return 1;
		}

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(argv[1], "daytime");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);

		for (;;)
		{
			boost::array<char, 128> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			std::cout.write(buf.data(), len);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
#pragma endregion DayTime_1

#pragma region DayTime_2
std::string make_daytime_string()
{
	time_t now = time(0);
	return ctime(&now);
}

void DayTime_2()
{
	try
	{
		boost::asio::io_service ios;
		tcp::acceptor acceptor(ios, tcp::endpoint(tcp::v4(), 13));

		for (;;)
		{
			tcp::socket socket(ios);
			acceptor.accept(socket);

			std::string msg = make_daytime_string();
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(msg),
				boost::asio::transfer_all(), ignored_error);
		}
	}
	catch (std::exception& e)
	{
		std::cerr<<e.what()<<std::endl;
	}
}

#pragma endregion DayTime_2

int main(int argc, char* argv[])
{
	DayTime_1(argc, argv);
	DayTime_2();
	return 0;
} 