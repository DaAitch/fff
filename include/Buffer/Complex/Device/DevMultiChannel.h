//---------------------------------------------------------+
// fff/include/Buffer/Complex/Device/DevMultiChannel.h
//---------------------------------------------------------+
//  License:
//    
//    The Fast Filtering Framework implements an LTI filter
//    with Khronos Group's OpenCL.
//    Copyright (C) 2012  Philipp Renoth <fff@aitch.de>
//
//    This program is free software: you can redistribute
//    it and/or modify it under the terms of the
//    GNU General Public License as published by the
//    Free Software Foundation, either version 3 of the
//    License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will
//    be useful, but WITHOUT ANY WARRANTY; without even the
//    implied warranty of MERCHANTABILITY or
//    FITNESS FOR A PARTICULAR PURPOSE.
//    See the GNU General Public License for more details.
//
//    You should have received a copy of the
//    GNU General Public License along with this program.
//    If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------+
//!
//!	\file		DevMultiChannel.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Device Multi channel
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

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
class DevMultiChannel :
    public iprintable
{
public:
    typedef DevMultiChannel<SampleType> My;
    typedef std::vector<DevSingleChannel<SampleType> > MyChannels;

public:
    DevMultiChannel()
        :
        m_sampleCount(0),
        m_memFlags(0)
    {
    }

    DevMultiChannel(
        const OpenCLEnvironment &env,
        cl_mem_flags flags,
        UInt channelCount,
        UInt sampleCount
        )
        :
        m_env(env),
        m_sampleCount(0),
        m_memFlags(0)
    {
        alloc(
            flags,
            channelCount,
            sampleCount);
    }

    DevMultiChannel(
        const OpenCLEnvironment &env)
        :
        m_env(env),
        m_sampleCount(0),
        m_memFlags(0)
    {
    }

    My createSubBuffer(
        cl_mem_flags flags,
        UInt sampleOffset,
        UInt sampleCount
        )
    {
        My mch;

        mch.m_channels.resize(getChannelCount());

        for(
            UInt channel = 0;
            channel < getChannelCount();
            ++channel)
        {
            mch.m_channels[channel] = fff_ME[channel].createSubBuffer(
                flags,
                sampleOffset,
                sampleCount
                );
        }

        mch.m_env = getEnv();
        mch.m_sampleCount = sampleCount;
        mch.m_memFlags = flags;

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
        cl_mem_flags flags,
        UInt channelCount,
        UInt sampleCount)
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
        if(isAllocated())
            _dealloc();
    }

    Bool isAllocated() const throw()
    {
        return
            _isAllocated();
    }

    DevSingleChannel<SampleType> &operator[](UInt index)
    {
        return
            getChannel(index);
    }

    const DevSingleChannel<SampleType> &operator[](UInt index) const
    {
        return
            getChannel(index);
    }

    const DevSingleChannel<SampleType> &getChannel(UInt channel) const
    {
        fff_EXPECT(
            channel,
            <,
            getChannelCount());

        return
            m_channels[channel];
    }

    DevSingleChannel<SampleType> &getChannel(UInt channel)
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
            getChannels().size();
    }

    UInt getSampleCount() const
	{
		return m_sampleCount;
	}

	UInt getPerBufferSize() const
	{
		return getSampleCount() * getSampleSize();
	}

    UInt getSampleSize() const
    {
        return sizeof(SampleType);
    }

    cl_mem_flags getMemFlags() const
    {
        return
            m_memFlags;
    }

    bool isWritable() const throw()
    {
        return
            getMemFlags() == CL_MEM_WRITE_ONLY ||
            getMemFlags() == CL_MEM_READ_WRITE;
    }

    bool isReadable() const throw()
    {
        return
            getMemFlags() == CL_MEM_READ_ONLY ||
            getMemFlags() == CL_MEM_READ_WRITE;
    }

    OpenCLEnvironment &getEnv()
    {
        fff_EXPECT_VALID_OBJ_RET(m_env);
    }

    const OpenCLEnvironment &getEnv() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_env);
    }

    bool operator!() const throw()
    {
        return
            !isAllocated() ||
            !m_env ||
            !getMemFlags() != 0;
    }

private:
    void _alloc(
        UInt channelCount,
        UInt sampleCount,
        cl_mem_flags flags)
    {
        m_channels.resize(channelCount);
        for(MyChannels::iterator it = m_channels.begin(); it != m_channels.end(); ++it)
            (*it) = DevSingleChannel<SampleType>(getEnv(), flags, sampleCount);

        m_sampleCount = sampleCount;
        m_memFlags = flags;
    }

    void _dealloc()
    {
        fff_EXPECT_TRUE(
            isAllocated());
        m_channels.clear();
        m_sampleCount = 0;
        m_memFlags = 0;
    }

    Bool _isAllocated() const throw()
    {
        return
            getChannelCount() > 0 &&
            getSampleCount() > 0;
    }

    

private:
    OpenCLEnvironment
        m_env;

    MyChannels
        m_channels;

    UInt
        m_sampleCount;

    cl_mem_flags
        m_memFlags;
};

}
}
}
}

#endif