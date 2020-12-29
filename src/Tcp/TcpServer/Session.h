#ifndef __SESSION_H__
#define __SESSION_H__

#include "Buffer.h"
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <list>

class Session;

using SOCKET_PTR = boost::shared_ptr<boost::asio::ip::tcp::socket>;
using SESSION_PTR = boost::shared_ptr<Session>;

enum class SessionState : uint16_t
{
    eState_inValid = 0, // 无效
    eState_verify = 1,  // 待验证连接
    eState_ok = 2,      // 验证Ok
};

class Session : public boost::enable_shared_from_this<Session>
{
public:
    Session();
    //Session(const SOCKET_PTR &sock_ptr, const uint32_t &idx);
    ~Session() {}

    void start();
    void onReceive(const BaseBufferPtr &buf);
    void sendMsg(const Message &msg);

    void setSocket(const SOCKET_PTR& ptr){socket_ = ptr;} 
    
    void setConnIdx(const uint32_t idx) { connect_index_ = idx; }
    uint32_t getConnIdx()
    {
        return connect_index_;
    }

    bool isStateOk() { return state == SessionState::eState_ok; }

private:
    SOCKET_PTR socket_;
    uint32_t connect_index_ = 0;
    SessionState state = SessionState::eState_inValid;
    BufferPool<102400> buffer_pool = {};
    std::list<BaseBufferPtr> receive_list = {};
    std::list<Message> send_list = {};
};

#endif