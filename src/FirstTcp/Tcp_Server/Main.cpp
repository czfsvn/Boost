#include <iostream>

#include "TcpServer.h"

using namespace std;

void main()
{
	std::cout<<"tcpserver start..."<<std::endl;
	Tcp_Server svr(6677);
	svr.Run();
	return;
}