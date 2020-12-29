#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <boost/functional/factory.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <vector>

#define MAX_BUFFER_SIZE 8192

class BaseBuffer;
using BaseBufferPtr = boost::shared_ptr<BaseBuffer>;

class BaseBuffer
{
    BaseBuffer()
    {
        data_.clear();
        len_ = MAX_BUFFER_SIZE;
        data_.reserve(len_);
    }

    void Reset()
    {
        rd_offset_ = 0;
        wr_offset_ = 0;
    }

    uint32_t canRdSize()
    {
        if (rd_offset_ >= wr_offset_)
            return 0;

        return wr_offset_ - rd_offset_;
    }

    uint32_t consume_rd(uint32_t size)
    {
        rd_offset_ += size;
    }

    uint32_t canWrSize()
    {
        if (wr_offset_ >= len_)
            return 0;

        return len_ - wr_offset_;
    }

    uint32_t consume_wr(uint32_t size)
    {
        wr_offset_ += size;
    }

    uint32_t append_data(const char *data, uint32_t size)
    {
        if (!data || !size)
            return 0;

        uint32_t can_wr = std::min<uint32_t>(size, canWrSize());
        if (can_wr)
        {
            std::copy(data, data + can_wr, beginWr());
            wr_offset_ += can_wr;
        }
        return can_wr;
    }

private:
    char *begin()
    {
        return &*data_.begin();
    }

    const char *begin() const
    {
        return &*data_.begin();
    }

    char *beginWr()
    {
        return begin() + wr_offset_;
    }

    const char *beginWr() const
    {
        return begin() + wr_offset_;
    }

    char *beginRd()
    {
        return begin() + rd_offset_;
    }

    const char *beginRd() const
    {
        return begin() + rd_offset_;
    }

private:
    std::vector<char> data_;
    uint32_t rd_offset_ = 0;
    uint32_t wr_offset_ = 0;
    uint32_t len_ = 0;
};

class MsgHeader
{
    uint16_t msg_code;
    uint16_t msg_len;
};

class Message : public BaseBuffer
{
};

class RingBuffer
{
public:
    RingBuffer(uint32_t init_size) : size(init_size)
    {
        buffers_.reserve(size);
    }

private:
    std::vector<BaseBuffer> buffers_ = {};
    uint32_t rd_idx = 0;
    uint32_t wr_idx = 0;
    uint32_t size = 0;
};

template <uint32_t N>
class BufferPool
{
public:
    BufferPool() {}

    BaseBufferPtr GetBuffer()
    {
        if (buffer_pool.empty())
        {
            return boost::factory<BaseBufferPtr>();
        }
        BaseBufferPtr ptr = buffer_pool.back();
        buffer_pool.pop_back();
        return ptr;
    }

    void PutBuffer(const BaseBufferPtr &ptr)
    {
        if (!ptr)
            return;

        if (buffer_pool.size() >= N)
            return;
        buffer_pool.emplace_back(ptr);
    }

private:
    std::list<BaseBufferPtr> buffer_pool = {};
};

#endif