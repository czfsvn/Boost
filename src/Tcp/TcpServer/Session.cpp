#include "Session.h"

//Session::Session(const SOCKET_PTR &sock_pt, const uint32_t &idx) : socket_(sock_pt), connect_index_(idx), state(SessionState::eState_verify)
//{
//}

Session::Session() : state(SessionState::eState_verify)
{
}

void Session::start()
{
    BaseBufferPtr buf = buffer_pool.GetBuffer();
    m_socket.async_read_some(boost::asio::buffer(buf->begin(), buf->canWrSize()),
                             [this, &buf](boost::system::error_code ec, std::size_t size) {
                                 if (!ec && size)
                                 {
                                     buf->consume_wr(size);
                                     onReceive(buf);
                                     start();
                                 }
                                 else
                                 {
                                     // todo
                                 }
                             });
}

void Session::onReceive(const BaseBufferPtr &buf)
{
    // need lock
    receive_list.emplace_back(buf);
}