#include <cstdio>
#include <deque>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "TypeDef.h"
#include "ChatMsg.h"

using boost::asio::ip::tcp;

typedef std::deque<ChatMsg> ChatMsgDeque;

class ChatClient
{
public:
	ChatClient(boost::asio::io_service& ios,
		tcp::resolver::iterator endIter) : m_ioService(ios)
		,m_socket(ios)
	{
		tcp::endpoint endpoint = *endIter;
		m_socket.async_connect(endpoint, 
			boost::bind(&ChatClient::_HandleConnect, this, 
			boost::asio::placeholders::error, ++endIter));
	}

	void Write(const ChatMsg& msg)
	{
		m_ioService.post(boost::bind(&ChatClient::_DoWrite, this, msg));
	}

	void Close()
	{
		m_ioService.post(boost::bind(&ChatClient::_DoClose, this));
	}

private:
	void _HandleConnect(const boost::system::error_code& err, tcp::resolver::iterator endIter)
	{
		if (!err)
		{
			boost::asio::async_read(m_socket, 
				boost::asio::buffer(m_readMsg.Data(), ChatMsg::Header_Length),
				boost::bind(&ChatClient::_HandleReadHeader, this, 
				boost::asio::placeholders::error));
		}
		else if (endIter != tcp::resolver::iterator())
		{
			_DoClose();
			tcp::endpoint endpoint = *endIter;
			m_socket.async_connect(endpoint, boost::bind(&ChatClient::_HandleConnect, this, 
				boost::asio::placeholders::error, ++endIter));
		}		
	}

	void _HandleReadHeader(const boost::system::error_code& err)
	{
		if (!err && m_readMsg.DecodeHeader())
		{
			boost::asio::async_read(m_socket,
				boost::asio::buffer(m_readMsg.Body(), m_readMsg.BodyLength()),
				boost::bind(&ChatClient::_HandleReadBody, this,
				boost::asio::placeholders::error));
		}
		else
		{
			_DoClose();
		}
	}

	void _HandleReadBody(const boost::system::error_code& err)
	{
		if (!err)
		{
			std::cout.write(m_readMsg.Body(), m_readMsg.BodyLength());
			std::cout<<"\n";
			boost::asio::async_read(m_socket,
				boost::asio::buffer(m_readMsg.Data(), ChatMsg::Header_Length),
				boost::bind(&ChatClient::_HandleReadHeader, this,
				boost::asio::placeholders::error));
		}
		else
		{
			_DoClose();
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
					boost::bind(&ChatClient::_HandleWrite, this,
					boost::asio::placeholders::error));
			}
		}
		else
		{
			_DoClose();
		}
	}
		 

	void _DoWrite(ChatMsg& msg)
	{
		bool progress = !m_writeMsgDeque.empty();
		m_writeMsgDeque.push_back(msg);
		if (!progress)
		{
			boost::asio::async_write(m_socket,
				boost::asio::buffer(m_writeMsgDeque.front().Data(), m_writeMsgDeque.front().Length()),
				boost::bind(&ChatClient::_HandleWrite, this,
				boost::asio::placeholders::error));
		}
		else
		{
			_DoClose();
		}
	}

	void _DoClose()
	{
		m_socket.close();
	}

protected:
private:
	boost::asio::io_service& m_ioService;
	tcp::socket m_socket;
	ChatMsg m_readMsg;
	ChatMsgDeque m_writeMsgDeque;
};

int32 main(int32 argc, char* argv[])
{
	std::cout<<"argc = "<<argc<<std::endl;
	for (int idx = 0; idx < argc; idx++)
	{
		std::cout<<"argv["<<idx<<"] = "<<(argv[idx])<<std::endl;
	}

	try
	{
		if (argc != 3)
		{
			std::cerr<<"Usage: ChatClient <host><port>\n";
			return 0;
		}

		boost::asio::io_service m_ios;

		tcp::resolver resolver(m_ios);
		tcp::resolver::query query(argv[1], argv[2]);
		tcp::resolver::iterator iterator = resolver.resolve(query);
		
		ChatClient cClient(m_ios, iterator);

		boost::thread th(boost::bind(&boost::asio::io_service::run, &m_ios));

		char line[ChatMsg::Max_Body_Length + 1] = "";
		while (std::cin.getline(line, ChatMsg::Max_Body_Length + 1))
		{
			using namespace std;
			ChatMsg msg;
			msg.BodyLength(strlen(line));
			memcpy(msg.Body(), line, msg.BodyLength());
			msg.EncodeHeader();
			cClient.Write(msg);
		}
		cClient.Close();
		th.join();
	}
	catch (std::exception* e)
	{
		std::cerr<<"Exception: "<<e->what()<<std::endl;
	}
	return 0;
}