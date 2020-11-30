#include <iostream>
#include "TypeDef.h"

#include <boost/thread.hpp>



using namespace std;

boost::mutex m_mutex;

boost::timed_mutex m_timeMutex; 

void Wait(uint32 sec)
{
	boost::this_thread::sleep(boost::posix_time::seconds(sec));
}

void Thread()
{
	try
	{
		for (uint16 i = 0; i < 30; i++)
		{
			std::cout<<"Thread"<<i<<std::endl;
			Wait(1);		
		}
	}
	catch (boost::thread_interrupted& e)
	{
		std::cout<<"boost::Thread thread_interrupted"<<std::endl;
	}	
}

void Thread_Mutex()
{
	try
	{
		//boost::mutex m_mutex;
		for (uint16 i = 0; i < 30; i++)
		{			
			m_mutex.lock();
			Wait(1);		
			std::cout<<boost::this_thread::get_id()<<", i = "<<i<<std::endl;
			m_mutex.unlock();
		}
	}
	catch (boost::thread_interrupted& e)
	{
		std::cout<<"boost::Thread_Mutex thread_interrupted"<<std::endl;
	}
}

void Thread_Guard(uint32 idx)
{
	try
	{
		for (uint16 i = 0; i < 30; i++)
		{			
			boost::lock_guard<boost::mutex> lock(m_mutex);  
			Wait(1);		
			std::cout<<boost::this_thread::get_id()<<", Thread_Guard, i = "<<i<<", idx = "<<idx<<std::endl;
		}
	}
	catch (boost::thread_interrupted& e)
	{
		std::cout<<"boost::Thread_Guard thread_interrupted"<<std::endl;
	}
}

void Thread_Unique(uint32 idx)
{
	for (int i = 0; i < 5; ++i)   
	{   
		Wait(1);   
		boost::unique_lock<boost::timed_mutex> lock(m_timeMutex, boost::try_to_lock);   
		if (!lock.owns_lock())   
			lock.timed_lock(boost::get_system_time() + boost::posix_time::seconds(1));   
		std::cout << "Thread_Unique " << boost::this_thread::get_id() << ": " << i <<", idx = "<<idx<< std::endl;   
		boost::timed_mutex *m = lock.release();   
		m->unlock();   
	}   
}

uint32 main()
{
	//boost::thread t(Thread);
	//Wait(5);
	//t.interrupt();
	//t.join();
	//Sleep(5000);
	{
		//boost::thread t(Thread);
		//t.detach();
	}

	std::cout<<boost::this_thread::get_id()<<std::endl;
	std::cout<<boost::thread::hardware_concurrency()<<std::endl;

	//boost::thread t1(Thread_Mutex);
	//boost::thread t2(Thread_Mutex);

	//boost::thread tg_1(Thread_Guard, 1);
	//boost::thread tg_2(Thread_Guard, 2);

	boost::thread tu_1(Thread_Unique, 1);
	boost::thread tu_2(Thread_Unique, 2);

	//t1.join();
	///t2.join();

	//tg_2.join();
	//tg_1.join();
	
	Wait(100);
	return 0;
}