#include "Client.h"

using namespace std;

void testClient()
{
	boost::asio::io_service ios;
	boost::asio::io_service::work work(ios);
	boost::thread thrd([&ios] {ios.run(); });

	ns_client1::Connector conn(ios, "127.0.0.1", 9001);
	conn.start();

	std::string str;
	if(!conn.isConnected())
	{
		std::cin >> str;
		return;
	}

	const int32_t len = 512;
	char line[len] = {};

	while(cin >> str)
	{
		char header[HEAD_LEN] = {};
		int total_len = str.length() + HEAD_LEN + 1;
		sprintf_s(header, "%d", total_len);
		memcpy(line, header, HEAD_LEN);
		memcpy(line + HEAD_LEN, str.c_str(), str.length() + 1);
		conn.send(line, total_len);
	}
}

int32_t main()
{
	testClient();
	return 0;
}