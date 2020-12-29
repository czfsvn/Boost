#include <iostream>

#include <boost/serialization/singleton.hpp>

#include <boost/shared_ptr.hpp>

class TcpServer;

using namespace std;

class AppServer : public boost::serialization::singleton<AppServer>
{
public:
    AppServer() {}
    ~AppServer() {}

    void init();
    void run();
    void destroy();

    void start();

private:
    bool loadConfigs();

private:
    bool terminate = false;
    boost::shared_ptr<TcpServer> srv = nullptr;
};