#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "TypeDef.h"

using namespace boost;
using namespace boost::asio;

class A_Timer
{
public:
	template<typename F>
	A_Timer(io_service& ios, int32 x, F func): f(func)
		,countMax(x)
		,count(0)
		,t(ios, boost::posix_time::millisec(500))
	{
		t.async_wait(bind(&A_Timer::call_Func, this, placeholders::error));
	}

	void call_Func(const boost::system::error_code& e)
	{
		if (count >= countMax)
		{
			return;
		}

		++count;

		f();
		t.expires_at(t.expires_at() + boost::posix_time::millisec(500));
		t.async_wait(bind(&A_Timer::call_Func, this, placeholders::error));
	}
protected:
private:
	int32 count, countMax;
	boost::function <void()> f;
	deadline_timer t;
};


void Client(io_service& ios)
{
	try
	{
		std::cout<<"Boost Client start..."<<std::endl;

		ip::tcp::socket sock(ios);
		ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 6688);

		sock.connect(ep);

		std::vector<char> str(100, 0);
		sock.read_some(buffer(str));
		std::cout<<"receive from "<<sock.remote_endpoint().address();
		std::cout<<&str[0]<<std::endl;
		
	}
	catch (std::exception& e)
	{
		std::cout<<e.what()<<std::endl;		
	}
}

int32 main()
{
	io_service ios;
	A_Timer at(ios, 10, bind(Client, ref(ios)));
	ios.run();
	return 0;
}