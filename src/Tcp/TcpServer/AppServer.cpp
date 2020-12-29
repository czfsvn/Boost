#include "AppServer.h"
#include "IoServicePool.h"
#include "TcpServer.h"
#include <boost/functional/factory.hpp>

void AppServer::init()
{
    std::cout << "AppServer Init...\n";
    loadConfigs();

    IoServicePool::get_mutable_instance().initPool(4);

    const uint32_t port = 9999;
    srv = boost::factory<boost::shared_ptr<TcpServer>>()(port);
    //srv = boost::make_shared<TcpServer>(9999);
    srv->startAccept();
}

void AppServer::run()
{
    while (!terminate)
    {
        sleep(1);
    }
}

bool AppServer::loadConfigs()
{
    return true;
}

void AppServer::destroy()
{
}
