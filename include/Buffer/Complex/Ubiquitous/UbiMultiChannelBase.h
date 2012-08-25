//---------------------------------------------------------+
// fff/include/fffCvUbiMChBuf.h
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
//!	\file		fffCvUbiMChBuf.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Multi channel ubiquitous complex vector
//!				buffer, means multi channel host buffer,
//!				single channel device buffer mapping.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffcvubimchbuf_h_included__
#define __fffcvubimchbuf_h_included__

#include "../../../_intern/_base.h"
#include "../Host/IHostMultiChannel.h"
#include "../../../Computing/OpenCLEnvironment.h"
#include "../Device/DevSingleChannel.h"

using namespace fff::Buffer::Complex::Device;
using namespace fff::Buffer::Complex::Host;

namespace fff {
namespace Buffer {
namespace Complex {
namespace Ubiquitous {

template<
	class SampleType
>
class UbiMultiChannelBase
	: public DevSingleChannel<SampleType>
{
public:
	typedef
		UbiMultiChannelBase<MySample> My;

public:
	UbiMultiChannelBase(
		const OpenCLEnvironment &env,
		cl_mem_flags flags,
		IHostMultiChannel<SampleType> *hostMultiChannel,
		Bool blockingRWs = False)
	:
		m_env(
			env),
		DevSingleChannel(
			env),
		m_multiChannel(
			NULL),
		m_blocking(
			blockingRWs)
	{
        fff_EXPECT_VALID_PTR(hostMultiChannel);

        setHostMultiChannel(
            hostMultiChannel);

        alloc(flags, getMultiChannel().getSampleCount());
	}

    UbiMultiChannelBase(
		const OpenCLEnvironment &env,
        cl_mem_flags flags,
        UInt maxSampleCount,
		Bool blockingRWs = False)
	:
		m_env(
			env),
		DevSingleChannel(
			env),
		m_multiChannel(
			NULL),
		m_blocking(
			blockingRWs)
	{
        alloc(flags, maxSampleCount);
	}

    void setHostMultiChannel(
        IHostMultiChannel<SampleType> *hostMultiChannel)
    {
        fff_EXPECT_VALID_PTR(hostMultiChannel);
        m_multiChannel = hostMultiChannel;
    }

	virtual ~UbiMultiChannelBase()
	{
		dealloc();
	}



	void enqueueHostUpdate(
		UInt channel)
	{
        fff_EXPECT_VALID_THIS();
		fff_RTCLC_ERR_INIT();

		const ::cl::CommandQueue &queue = getEnv().getQueue();
	
        fff_EXPECT_VALID_CLOBJ(queue);
        
		fff_RTCLC_SEQ_CHECK_RET(
			queue.enqueueReadBuffer(
				getReal(),
				isBlocking(),
				0,
				getMultiChannel().getSampleCount() * getSampleSize(),
				getMultiChannel().getRawReal(channel)));

		fff_RTCLC_SEQ_CHECK_RET(
			queue.enqueueReadBuffer(
				getImag(),
				isBlocking(),
				0,
				getMultiChannel().getSampleCount() * getSampleSize(),
				getMultiChannel().getRawImag(channel)));
	}

	void enqueueDeviceUpdate(
		UInt channel) const
	{
        fff_EXPECT_VALID_THIS();
		fff_RTCLC_ERR_INIT();

		const ::cl::CommandQueue &queue = getEnv().getQueue();
	
		fff_EXPECT_VALID_CLOBJ(queue);

        fff_EXPECT(
            getMultiChannel().getSampleCount(),
            <=,
            getSampleCount());

		fff_RTCLC_SEQ_CHECK_RET(
			queue.enqueueWriteBuffer(
				getReal(),
				isBlocking(),
				0,
                getMultiChannel().getSampleCount() * getSampleSize(),
				//getPerBufferSize(),
				getMultiChannel().getRawReal(channel)));

		fff_RTCLC_SEQ_CHECK_RET(
			queue.enqueueWriteBuffer(
				getImag(),
				isBlocking(),
				0,
				//getPerBufferSize(),
                getMultiChannel().getSampleCount() * getSampleSize(),
				getMultiChannel().getRawImag(channel)));
	}

	const IHostMultiChannel<SampleType> &getMultiChannel() const
	{
        fff_EXPECT_VALID_PTR(m_multiChannel);
        fff_EXPECT_VALID_OBJ_RET(*m_multiChannel);
	}

    IHostMultiChannel<SampleType> &getMultiChannel()
	{
        fff_EXPECT_VALID_PTR(m_multiChannel);
        fff_EXPECT_VALID_OBJ_RET(*m_multiChannel);
	}

	void setBlocking(
		Bool blockingRWs)
	{
		m_blocking = blockingRWs;
	}

	Bool isBlocking() const
	{
		return
            m_blocking;
	}

    OpenCLEnvironment &getEnv()
    {
        fff_EXPECT_VALID_OBJ_RET(m_env);
    }

    const OpenCLEnvironment &getEnv() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_env);
    }

    virtual void print(logstream &out) const
    {
        out("UbiMultiChannelBase");
        !fff_LOG_VAL(out, getEnv());
        !fff_LOG_VAL(out, getMultiChannel());
        !fff_LOG_VAL(out, isBlocking());
        out();
    }

    bool operator!() const throw()
    {
        return
            !m_env ||
            !m_multiChannel ||
            !*m_multiChannel;
    }

    Bool hasSameSize(const My &rhs) const
    {
        return
            hasSameSampleCount(rhs) &&
            hasSameChannelCount(rhs);
    }

    Bool hasSameSampleCount(const My &rhs) const
    {
        return
            getMultiChannel().getSampleCount() ==
            rhs.getMultiChannel().getSampleCount();
    }

    Bool hasSameChannelCount(const My &rhs) const
    {
        return
            getMultiChannel().getChannelCount() ==
            rhs.getMultiChannel().getChannelCount();
    }

private:

private:
	OpenCLEnvironment
		m_env;
	IHostMultiChannel<SampleType>
		*m_multiChannel;
	Bool
		m_blocking;
};

}
}
}
}

#endif