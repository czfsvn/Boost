#include "IoServicePool.h"

bool IoServicePool::initPool(uint32_t size)
{
    pool_size_ = size;
    if (pool_size_ < 1)
        return false;

    ios_pool_.clear();
    for (uint32_t idx = 0; idx < pool_size_; idx++)
    {
        boost::shared_ptr<boost::asio::io_service> ios = boost::make_shared<boost::asio::io_service>();
        ios_pool_.push_back(ios);
    }

    return start();
}

boost::shared_ptr<boost::asio::io_service> &IoServicePool::getIos()
{
    cur_used_ = ++cur_used_ % pool_size_;
    return ios_pool_[cur_used_];
}

bool IoServicePool::start()
{
    if (threads_.size())
        return false;

    for (auto ios : ios_pool_)
    {
        threads_.create_thread([&ios] { ios->run(); });
    }

    threads_.join_all();

    return true;
}

void IoServicePool::stop()
{
    for (auto ios : ios_pool_)
    {
        ios->stop();
    }
}
