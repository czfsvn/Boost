#ifndef __JOB_QUEUE_20120514_H__
#define __JOB_QUEUE_20120514_H__

#include <deque>

#include <boost/noncopyable.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/utility/value_init.hpp>

template <typename Job>
class JobQueue : public boost::noncopyable
{

#pragma region typedef

public: 
	typedef Job job_type;
	typedef std::deque<job_type> queue_type;

	typedef boost::mutex mutex_type;
	typedef typename mutex_type::scoped_lock lock_type;
	typedef boost::condition_variable_any condition_type;

	//BOOST_CONCEPT_ASSERT((boost::SGIAssignable<job_type>));
	//BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<job_type>));

#pragma endregion typedef

#pragma region public functions

public:
	JobQueue() 
		: m_stop_flag(false)
	{

	}

	void Push(const job_type& x)
	{
		lock_type lock(m_mutex);
		m_queue.push_back(x);
		m_hasJob.notify_one();
	}

	job_type Pop()
	{
		lock_type lock(m_mutex);
		while (m_queue.empty() && !m_stop_flag)
		{
			m_hasJob.wait(m_mutex);
		}

		if (m_stop_flag)
			return boost::initialized_value;

		BOOST_ASSERT(!m_queue.empty());

		job_type tmp = m_queue.front();
		m_queue.pop_front();
		return tmp;
	}

	void Stop()
	{
		m_stop_flag = true;
		m_hasJob.notify_all();
	}

#pragma endregion public functions

#pragma region private members

private:
	queue_type  m_queue;
	mutex_type m_mutex;
	condition_type m_hasJob;
	bool m_stop_flag;

#pragma endregion private members

};



//---------------------------Test

#include <boost/detail/lightweight_test.hpp>
void TestQueue()
{
	JobQueue<int> q;
	q.Push(10);
	q.Push(20);

	int tmp = q.Pop();
	BOOST_TEST_EQ(tmp, 10);
	BOOST_TEST_EQ(q.Pop(), 20);

	q.Push(30);
	q.Stop();
	tmp = q.Pop();
	BOOST_TEST_EQ(tmp, 0);
}


#endif