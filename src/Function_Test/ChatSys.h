#ifndef __CHATSYS_20120308_H__
#define __CHATSYS_20120308_H__

#include <iostream>

#include <boost/function.hpp>



enum CHAT_TYPE
{
	CHAT_TYPE_COMMON,		//普通区域内聊天
	CHAT_TYPE_GUILD,		//行会	
	CHAT_TYPE_WHISPER,		//私聊		
	CHAT_TYPE_MAX,
};

class ChatSys;

typedef boost::function<void (ChatSys&, std::string&)> Func;
typedef boost::function<void (std::string&)> Func1;
//typedef boost::function<void (ChatSys*, std::string&)> Func;

//typedef void (ChatSys::*CSys)(std::string&);


class ChatSys
{
public:
	ChatSys();
	~ChatSys();

	static void FillHandle();

	void HandleMsg(CHAT_TYPE type, std::string msg);


	void _CommonChat(std::string&);
	void _GuildChat(std::string&);
	void _WhisperChat(std::string&);

protected:
private:

	static Func m_Handle[CHAT_TYPE_MAX];
};

#endif