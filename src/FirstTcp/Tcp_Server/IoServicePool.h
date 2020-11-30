#ifndef __IO_SERVICE_POOL_20120514_H__
#define __IO_SERVICE_POOL_20120514_H__

#include <boost/functional/factory.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>


class IO_Service_Pool : public boost::noncopyable
{

#pragma region typedef

public:

	typedef boost::asio::io_service ios_type;
	typedef boost::asio::io_service::work work_type;
	typedef boost::ptr_vector<ios_type> io_service_type;
	typedef boost::ptr_vector<work_type> works_type;

#pragma endregion typedef

#pragma region public functions

public:
	explicit IO_Service_Pool(int n = 4)
		: m_next_io_service(0)
	{
		BOOST_ASSERT(n > 0);
		_Init(n);
	}

	ios_type& Get()
	{
		if (m_next_io_service >= m_io_services.size())
			m_next_io_service = 0;

		return m_io_services[m_next_io_service++];
	}

	void Start()
	{
		if (m_threads.size() > 0)
			return;

		BOOST_FOREACH(ios_type& ios, m_io_services)
		{
			m_threads.create_thread(
				boost::bind(&ios_type::run, boost::ref(ios)));
		}
	}

	void Run()
	{
		Start();
		m_threads.join_all();
	}

	void Stop()
	{
		m_works.clear();
		std::for_each(m_io_services.begin(), m_io_services.end(),
			boost::bind(&ios_type::stop, _1));
	}

#pragma endregion public functions
protected:

#pragma region private functions

private:
	void _Init(int n)
	{
		for (int i = 0; i<n; i++)
		{
			m_io_services.push_back(boost::factory<ios_type*>()());

			m_works.push_back(boost::factory<work_type*>()(m_io_services.back()));
		}
	}

#pragma endregion private functions


#pragma region private members

private:
	io_service_type m_io_services;
	works_type m_works;

	boost::thread_group m_threads;
	std::size_t m_next_io_service;

#pragma endregion private members
};

#endif