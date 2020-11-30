#include <iostream>

#include <boost/function.hpp>

#include "ChatSys.h"

//typedef boost::function<void (&ChatSys, std::string&)> Func;

void main()
{
	ChatSys::FillHandle();
	ChatSys* cSys = new ChatSys;
	cSys->HandleMsg(CHAT_TYPE_COMMON, "msg");
	std::cout<<"..."<<std::endl;
}