
#ifndef __devmultichannel_h__included__
#define __devmultichannel_h__included__

#include "../../../_intern.h"
#include "DevSingleChannel.h"
#include "../../../Computing/OpenCLEnvironment.h"
#include <vector>

namespace fff {
namespace Buffer {
namespace Complex {
namespace Device {

template<
    class SampleType
>
class DevMultiChannel
{
public:
    typedef DevMultiChannel<SampleType> My;
    typedef std::vector<DevSingleChannel<SampleType> > MyChannels;

private:
    DevMultiChannel()
        :
        m_sampleCount(0)
    {
    }

public:
    DevMultiChannel(
        const OpenCLEnvironment &env
        UInt channelCount,
        UInt sampleCount,
        cl_mem_flags flags)
        :
        m_env(env),
        m_sampleCount(0)
    {
        alloc(
            channelCount,
            sampleCount,
            flags);
    }

    My createSubBuffer(
        UInt sampleOffset,
        UInt sampleCount,
        cl_mem_flags flags)
    {
        My mch;

        mch.m_channels.resize(getChannelCount());

        for(
            UInt channel = 0;
            channel < getChannelCount()
            ++channel)
        {
            m_channels[channel] = fff_ME[channel].createSubBuffer(
                sampleOffset,
                sampleCount,
                flags);
        }

        mch.m_env = getEnv();
        mch.m_sampleCount = sampleCount;

        return mch;
    }

    MyChannels &getChannels()
    {
        return
            m_channels;
    }

    const MyChannels &getChannels() const
    {
        return
            m_channels;
    }

    void alloc(
        UInt channelCount,
        UInt sampleCount,
        cl_mem_flags flags)
    {
        dealloc();

        fff_EXPECT_TRUE(
            !isAllocated());

        _alloc(
            channelCount,
            sampleCount,
            flags);
    }

    void dealloc()
    {
        _dealloc();
    }

    Bool isAllocated() const throw()
    {
        return
            _isAllocated();
    }

    DevSingleChannel &operator[](UInt index)
    {
        return
            getChannel(index);
    }

    const DevSingleChannel &operator[](UInt index) const
    {
        return
            getChannel(index);
    }

    const DevSingleChannel &getChannel(UInt channel) const
    {
        fff_EXPECT(
            channel,
            <,
            getChannelCount());

        return
            m_channels[channel];
    }

    DevSingleChannel &getChannel(UInt channel)
    {
        fff_EXPECT(
            channel,
            <,
            getChannelCount());

        return
            m_channels[channel];
    }

    UInt getChannelCount() const
    {
        return
            m_channel.size();
    }

private:
    void _alloc(
        UInt channelCount,
        UInt sampleCount,
        cl_mem_flags flags)
    {
        m_channels.resize(channelCount);
        for(MyChannels::iterator it = m_channels.begin(); it != m_channels.end(); ++it)
            (*it).alloc(sampleCount, flags);

        m_sampleCount = sampleCount;
    }

    void _dealloc()
    {
        fff_EXPECT_TRUE(
            isAllocated());
        m_channels.clear();
    }

    Bool _isAllocated() const throw()
    {
        return
            getChannelCount() > 0;
    }

private:
    OpenCLEnvironment
        m_env;

    MyChannels
        m_channels;

    UInt
        m_sampleCount;
};

}
}
}
}

#endif