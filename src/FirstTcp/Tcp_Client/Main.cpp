#include <iostream>

#include <boost/asio.hpp>

using namespace boost::asio;



int main()
{
	std::cout<<"client start..."<<std::endl;

	io_service ios;
	ip::tcp::socket socket(ios);

	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 6677);
	socket.connect(ep);

	std::string str("Hello, World");
	socket.write_some(buffer(str));
	std::vector<char> v(100, 0);

	size_t n = socket.read_some(buffer(v));
	std::cout<<&v[0]<<std::endl;

	return 0;
}