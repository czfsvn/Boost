#include <algorithm>
#include <deque>
#include <set>
#include <list>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "TypeDef.h"
#include "ChatMsg.h"

using boost::asio::ip::tcp;

class ChatParticipant;
class ChatSession;
class ChatServer;


typedef boost::shared_ptr<ChatParticipant> ChatParticipantPtr;
typedef boost::shared_ptr<ChatSession> ChatSessionPtr;
typedef boost::shared_ptr<ChatServer> ChatSrvPtr;

typedef std::deque<ChatMsg> ChatMsgDeque;
typedef std::list<ChatSrvPtr> ChatSrvPtrList;

#pragma region ChatParticipant

class ChatParticipant
{
public:
	virtual ~ChatParticipant()
	{

	}

	virtual void Deliver(const ChatMsg& msg) = 0;
protected:
private:
};

#pragma endregion ChatParticipant

#pragma region ChatRoom

class CharRoom
{
public:
	void Join(ChatParticipantPtr cPtr)
	{
		m_chatPPtrSet.insert(cPtr);
		std::for_each(m_recentMsgDeque.begin(),
			m_recentMsgDeque.end(),
			boost::bind(&ChatParticipant::Deliver, cPtr, _1));
	}

	void Leave(ChatParticipantPtr cPtr)
	{
		m_chatPPtrSet.erase(cPtr);
	}

	void Deliver(const ChatMsg& msg)
	{
		m_recentMsgDeque.push_back(msg);
		while (m_recentMsgDeque.size() > Max_Recent_Msgs)
		{
			m_recentMsgDeque.pop_front();
		}

		std::for_each(m_chatPPtrSet.begin(), m_chatPPtrSet.end(),
			boost::bind(&ChatParticipant::Deliver, _1, boost::ref(msg)));
	}

protected:
private:
	std::set<ChatParticipantPtr> m_chatPPtrSet;
	ChatMsgDeque m_recentMsgDeque;
	enum
	{
		Max_Recent_Msgs		= 100
	};
};

#pragma endregion ChatRoom

#pragma region ChatSession
class ChatSession : public ChatParticipant, public boost::enable_shared_from_this<ChatSession>
{
public:
	ChatSession(boost::asio::io_service& ios, 
		CharRoom& room) : m_socket(ios)
		,m_room(room)
	{

	}

	tcp::socket& Socket()
	{
		return m_socket;
	}

	void Start()
	{
		m_room.Join(shared_from_this());
		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_readMsg.Data(), ChatMsg::Header_Length),
			boost::bind(&ChatSession::_HandleReadHeader, shared_from_this(),
			boost::asio::placeholders::error));
	}

	void Deliver(const ChatMsg& msg)
	{
		bool isProgress = !m_writeMsgDeque.empty();
		m_writeMsgDeque.push_back(msg);
		if (!isProgress)
		{
			boost::asio::async_write(m_socket,
				boost::asio::buffer(m_writeMsgDeque.front().Data(), m_writeMsgDeque.front().Length()),
				boost::bind(&ChatSession::_HandleWrite, shared_from_this(), 
				boost::asio::placeholders::error));
		}
	}

private:
	void _HandleReadHeader(const boost::system::error_code& err)
	{
		if (!err && m_readMsg.DecodeHeader())
		{
			boost::asio::async_read(m_socket,
				boost::asio::buffer(m_readMsg.Body(), m_readMsg.BodyLength()),
				boost::bind(&ChatSession::_HandleReadBody, shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			_LeaveRoom();
		}
	}

	void _HandleReadBody(const boost::system::error_code& err)
	{
		if (!err)
		{
			m_room.Deliver(m_readMsg);
			boost::asio::async_read(m_socket,
				boost::asio::buffer(m_readMsg.Data(), ChatMsg::Header_Length),
				boost::bind(&ChatSession::_HandleReadHeader, shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			_LeaveRoom();
		}
	}

	void _HandleWrite(const boost::system::error_code& err)
	{
		if (!err)
		{
			m_writeMsgDeque.pop_front();
			if (!m_writeMsgDeque.empty())
			{
				boost::asio::async_write(m_socket,
					boost::asio::buffer(m_writeMsgDeque.front().Data(), m_writeMsgDeque.front().Length()),
					boost::bind(&ChatSession::_HandleWrite, shared_from_this(), 
					boost::asio::placeholders::error));
			}
		}
		else
		{
			_LeaveRoom();
		}
	}

	void _LeaveRoom()
	{
		m_room.Leave(shared_from_this());
	}

protected:
private:
	tcp::socket m_socket;
	CharRoom& m_room;
	ChatMsg m_readMsg;
	ChatMsgDeque m_writeMsgDeque;
};
#pragma endregion ChatSession

#pragma region ChatServer
class ChatServer
{
public:
	ChatServer(boost::asio::io_service& ios,
		const tcp::endpoint& ep) : m_ioService(ios)
		,m_acceptor(ios, ep)
	{
		ChatSessionPtr session(new ChatSession(m_ioService, m_chatRoom));
		m_acceptor.async_accept(session->Socket(),
			boost::bind(&ChatServer::_HandleAccept, this, session,
			boost::asio::placeholders::error));
	}

private:
	void _HandleAccept(ChatSessionPtr cPtr, const boost::system::error_code& err)
	{
		if (!err)
		{
			cPtr->Start();

			tcp::endpoint& ep = cPtr->Socket().remote_endpoint();
			boost::asio::ip::address addr = ep.address();
			std::cout<<"new connection added, ip = "<<addr<<", port = "<<ep.port()<<std::endl;

			ChatSessionPtr session(new ChatSession(m_ioService, m_chatRoom));
			m_acceptor.async_accept(session->Socket(),
				boost::bind(&ChatServer::_HandleAccept, this, session,
				boost::asio::placeholders::error));
		}
	}

protected:
private:
	boost::asio::io_service& m_ioService;
	tcp::acceptor m_acceptor;
	CharRoom m_chatRoom;
};
#pragma endregion ChatServer

int32 main(int32 argc, char* argv[])
{
	using namespace std;
	try
	{
		if (argc < 2)
		{
			std::cerr<<"Usage: ChatServer<port>[<port>...]"<<std::endl;
			return 1;
		}

		boost::asio::io_service m_ios;
		ChatSrvPtrList m_srvList;
		for (int32 i = 1; i < argc; ++i)
		{
			
			tcp::endpoint ep(tcp::v4(), atoi(argv[i]));
			ChatSrvPtr srv(new ChatServer(m_ios, ep));
			m_srvList.push_back(srv);
		}

		m_ios.run();
	}
	catch (std::exception& e)
	{
		std::cerr<<"Exception : "<<e.what()<<std::endl;
	}
	
	return 0;
}