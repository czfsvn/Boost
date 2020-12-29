#ifndef __IOS_POOL_H__
#define __IOS_POOL_H__

#include <boost/asio.hpp>
//#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/thread.hpp>

class IoServicePool : public boost::serialization::singleton<IoServicePool> //, boost::noncopyable
{
public:
    IoServicePool() {}
    ~IoServicePool() {}

    bool initPool(uint32_t size);

    bool start();
    void stop();

    boost::shared_ptr<boost::asio::io_service>& getIos();

private:
    std::vector<boost::shared_ptr<boost::asio::io_service>> ios_pool_ = {};
    boost::thread_group threads_;
    uint32_t pool_size_ = 0;
    uint32_t cur_used_ = 0;
};

#endif