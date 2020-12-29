#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "Session.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TcpServer
{
public:
    TcpServer(uint16_t port);
    ~TcpServer() {}

    void startAccept();
    void onAccept(SOCKET_PTR sock, boost::system::error_code ec);

    void handleAcpErr(SOCKET_PTR sock, const boost::system::error_code &err);
    void stopAccept();

    uint32_t generateConnIdx() { return ++connect_idx_; }

private:
    tcp::acceptor acceptor_;
    //boost::shared_ptr<boost::asio::io_service> ios_ = nullptr;

    uint32_t connect_idx_ = 0;
    std::map<uint32_t, SESSION_PTR> session_map_ = {};
};

#endif