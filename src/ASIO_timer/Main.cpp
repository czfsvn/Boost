#include <time.h>

#include "TypeDef.h"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>



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



void Print(const boost::system::error_code& )
{
	std::cout<<"Hello,World!"<<std::endl;

	std::cout<<"now = "<<time(NULL)<<std::endl;
}

void Print1()
{
	std::cout<<"Hello, asio"<<std::endl;
}

void Print2()
{
	std::cout<<"Hello, boost"<<std::endl;
}

void Test_Deadline_timer()
{
	io_service ios;
	std::cout<<"Test_Deadline_timer now = "<<time(NULL)<<std::endl;
	deadline_timer t(ios, boost::posix_time::seconds(2));
	std::cout<<"t.expires_at() = "<<t.expires_at()<<std::endl;
	t.wait();
	std::cout<<"Test_Deadline_timer Hello,World!"<<std::endl;

	std::cout<<"Test_Deadline_timer now = "<<time(NULL)<<std::endl;
}

void Test_async_Deadline_timer()
{
	io_service ios;
	std::cout<<"Test_async_Deadline_timer now = "<<time(NULL)<<std::endl;
	deadline_timer t(ios, boost::posix_time::seconds(2));

	t.async_wait(Print);
	std::cout<<"it show before t expired."<<std::endl;
	ios.run();
}

void Test_async_Deadline_timer_Bind()
{
	io_service ios;
	A_Timer at1(ios, 5, Print1);
	A_Timer at2(ios, 5, Print2);
	ios.run();
}

#pragma region TestSyncThread

class Printer
{
public:
	Printer(boost::asio::io_service& ios) : m_strand(ios)
		,m_timer1(ios, boost::posix_time::seconds(1))
		,m_timer2(ios, boost::posix_time::seconds(2))
		,m_count(0)
	{
		m_timer1.async_wait(m_strand.wrap(boost::bind(&Printer::Print1, this)));
		m_timer2.async_wait(m_strand.wrap(boost::bind(&Printer::Print2, this)));
	}

	~Printer()
	{
		std::cout<<"Final count is: "<<std::endl;
	}

	void Print1()
	{
		if (m_count < 10)
		{
			std::cout<<"Timer1 : "<<m_count++<<std::endl;

			m_timer1.expires_at(m_timer1.expires_at() + boost::posix_time::seconds(1));
			m_timer1.async_wait(m_strand.wrap(boost::bind(&Printer::Print1, this)));
		}
	}

	void Print2()
	{
		if (m_count < 10)
		{
			std::cout<<"Timer2 : "<<m_count++<<std::endl;

			m_timer2.expires_at(m_timer2.expires_at() + boost::posix_time::seconds(1));
			m_timer2.async_wait(m_strand.wrap(boost::bind(&Printer::Print2, this)));
		}
	}

protected:
private:
	boost::asio::strand m_strand;
	boost::asio::deadline_timer m_timer1, m_timer2;
	uint32 m_count;
};

void TestSyncThread()
{
	boost::asio::io_service ios;
	Printer p(ios);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
	ios.run();
	t.join();
	return;	
}

#pragma endregion TestSyncThread



int32 main()
{
	//Test_Deadline_timer();
	//Test_async_Deadline_timer();
	//Test_async_Deadline_timer_Bind();
	TestSyncThread();

	return 0;
}
