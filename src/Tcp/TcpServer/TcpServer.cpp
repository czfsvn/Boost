#include "TcpServer.h"
#include "IoServicePool.h"

#include <boost/functional/factory.hpp>

TcpServer::TcpServer(uint16_t port) : acceptor_(*IoServicePool::get_mutable_instance().getIos(), tcp::endpoint(tcp::v4(), port))
{
    startAccept();
}

void TcpServer::startAccept()
{
    //SOCKET_PTR acc_socket = boost::make_shared<boost::asio::ip::tcp::socket>(*ios); //boost::factory<SOCKET_PTR>()();
    SOCKET_PTR acc_socket = boost::factory<SOCKET_PTR>()(*IoServicePool::get_mutable_instance().getIos());
    acceptor_.async_accept(*acc_socket, boost::bind(&TcpServer::onAccept, this, acc_socket, boost::asio::placeholders::error));
}

void TcpServer::onAccept(SOCKET_PTR sock, boost::system::error_code ec)
{
    if (!sock || ec)
    {
        handleAcpErr(sock, ec);
        return;
    }
    else
    {
        //todo
        const uint32_t conn_idx = generateConnIdx();
        //SESSION_PTR session1 = boost::factory<SESSION_PTR>()(sock, conn_idx);
        //SESSION_PTR session = boost::make_shared<Session>(sock, conn_idx);
        SESSION_PTR session(new Session());
        //session->start();
        //session_map_.emplace(session->getConnIdx(), session);

        startAccept();
    }
}

void TcpServer::handleAcpErr(SOCKET_PTR sock, const boost::system::error_code &err)
{
    boost::system::error_code ec;
    sock->shutdown(tcp::socket::shutdown_both, ec);
    sock->close(ec);
    stopAccept();
}

void TcpServer::stopAccept()
{
    boost::system::error_code err;
    acceptor_.cancel(err);
    acceptor_.close(err);
}