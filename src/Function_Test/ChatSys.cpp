#include <iostream>

#include <boost/bind.hpp>

#include "ChatSys.h"



Func ChatSys::m_Handle[CHAT_TYPE_MAX];

void ChatSys::FillHandle()
{

	memset(m_Handle, 0, sizeof(Func)*CHAT_TYPE_MAX);

	m_Handle[CHAT_TYPE_COMMON]				=		boost::bind(&ChatSys::_CommonChat, _1, _2);//&ChatSys::_CommonChat;
	//m_Handle[CHAT_TYPE_GUILD]				=		boost::bind(&ChatSys::_GuildChat, _1, _2);//&ChatSys::_CommonChat;
	//m_Handle[CHAT_TYPE_WHISPER]				=		boost::bind(&ChatSys::_WhisperChat, _1, _2);//&ChatSys::_CommonChat;

	//boost::function2<void, ChatSys*, std::string&> f = &ChatSys::_CommonChat;
	//m_Handle[CHAT_TYPE_COMMON]				=		&ChatSys::_CommonChat;
	//m_Handle[CHAT_TYPE_GUILD]				=		boost::bind(&ChatSys::_GuildChat, _1, _2);//&ChatSys::_CommonChat;
	//m_Handle[CHAT_TYPE_WHISPER]				=		boost::bind(&ChatSys::_WhisperChat, _1, _2);//&ChatSys::_CommonChat;

	
}

ChatSys::ChatSys()
{

}

ChatSys::~ChatSys()
{

}

void ChatSys::HandleMsg(CHAT_TYPE type, std::string msg)
{
	if (type >= CHAT_TYPE_MAX || type < CHAT_TYPE_COMMON)
	{
		return;
	}

	/*Func f = m_Handle[type];
	if (f)
	{
		f(*this, msg);
	}*/	
}

void ChatSys::_CommonChat(std::string& msg)
{
	std::cout<<"_CommonChat:: msg = "<<msg<<std::endl;
}


void ChatSys::_GuildChat(std::string& msg)
{
	std::cout<<"_GuildChat:: msg = "<<msg<<std::endl;
}

void ChatSys::_WhisperChat(std::string& msg)
{
	std::cout<<"_WhisperChat:: msg = "<<msg<<std::endl;
}
