#ifndef __TCP_MESSAGE_20120514_H__
#define __TCP_MESSAGE_20120514_H__

#define MAX_BODY_SIZE		1024

#include <boost/noncopyable.hpp>
#include <boost/checked_delete.hpp>
#include <boost/function.hpp>
#include <boost/array.hpp>
#include <boost/crc.hpp>



struct MsgHead
{
	unsigned int type;
	unsigned int size;
	unsigned int chksum;
};

class Tcp_Message;

typedef Tcp_Message tcp_request;
typedef Tcp_Message tcp_respone;
typedef tcp_request* tcp_request_ptr;
typedef tcp_respone* tcp_respone_ptr;



class Tcp_Message : public noncopyable
{

#pragma region typedef
	
public:
	typedef boost::function<void(Tcp_Message*)> destroy_type;
	typedef char char_type;

	typedef boost::array<char_type, sizeof(MsgHead)> head_data_type;
	typedef boost::array<char_type, MAX_BODY_SIZE> body_data_type;
#pragma endregion typedef

#pragma region public functions

public:
	template<typename Func>
	Tcp_Message(const tcp_session_ptr& s, Func func)
		:m_session(s)
		,m_destroy(func)
	{

	}

	Tcp_Message(const tcp_session_ptr& s)
		:m_session(s)
	{

	}

	tcp_session_ptr GetSession()
	{
		return m_session;
	}

	void Destroy()
	{
		if (m_destroy)
			m_destroy(this);
		else
			boost::checked_delete(this);
	}

	head_data_type& HeadData()
	{
		return m_head;
	}

	body_data_type& MsgData()
	{
		return m_msg;
	}

	MsgHead* GetHead()
	{
		return reinterpret_cast<MsgHead*>(m_head.data());
	}

	bool CheckHead()
	{
		return GetHead()->size < MAX_BODY_SIZE;
	}

	bool CheckMsgCrc()
	{
		boost::crc_32_type crc32;
		crc32.process_bytes(&m_msg[0], GetHead()->size);
		return GetHead()->chksum == crc32.checksum();
	}

	void SetMsgCrc()
	{
		boost::crc_32_type crc32;
		crc32.process_bytes(&m_msg[0], GetHead()->size);
		GetHead()->chksum = crc32.checksum();
	}

#pragma endregion public functions



#pragma region private members

private:
	tcp_session_ptr m_session;
	destroy_type m_destroy;
	head_data_type m_head;
	body_data_type m_msg;


#pragma endregion private members
};

#endif