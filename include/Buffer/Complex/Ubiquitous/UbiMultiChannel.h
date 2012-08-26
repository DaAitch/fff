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
#include "../Device/DevMultiChannel.h"

#include <algorithm>

using namespace fff::Buffer::Complex::Device;
using namespace fff::Buffer::Complex::Host;

namespace fff {
namespace Buffer {
namespace Complex {
namespace Ubiquitous {

template<
	class SampleType
>
class UbiMultiChannel
    : public iprintable
{
public:
	typedef
		UbiMultiChannel<SampleType> My;

public:

	UbiMultiChannel(
		IHostMultiChannel<SampleType> &host,
        DevMultiChannel<SampleType> &dev,
        Bool blockingRWs=False
		)
	:
		m_dev(
			dev),
		m_host(
			host),
		m_blocking(
			False),
        m_sampleLength(
            0),
        m_channelCount(
            host.getChannelCount()),
        m_hostSampleOffset(0),
        m_devSampleOffset(0)
	{

        setBlocking(
            blockingRWs);

        smartSampleLength();
	}

	virtual ~UbiMultiChannel() throw()
	{
	}

    void setSampleLength(UInt sampleLength)
    {
        m_sampleLength = sampleLength;
    }

    void smartSampleLength()
    {
        UInt sampleLength = ::std::min<UInt>(
            getDev().getSampleCount(),
            getHost().getSampleCount());

        setSampleLength(sampleLength);
    }

    void enqueueHostUpdate()
    {
        fff_EXPECT_VALID_THIS();

        for(UInt channel = 0;
            channel < getChannelCount();
            ++channel)
        {
            enqueueHostUpdate(channel, NULL, NULL);
        }
    }

    void enqueueDeviceUpdate() const
    {
        fff_EXPECT_VALID_THIS();

        for(UInt channel = 0;
            channel < getChannelCount();
            ++channel)
        {
            enqueueDeviceUpdate(channel);
        }
    }

	void enqueueHostUpdate(
		UInt channel,
        ::cl::Event *evtReal=NULL,
        ::cl::Event *evtImag=NULL)
	{
        fff_EXPECT_VALID_THIS();
		fff_RTCLC_ERR_INIT();
        
		const ::cl::CommandQueue &queue = getDev().getEnv().getQueue();
        
		fff_RTCLC_SEQ_CHECK_RET(
			queue.enqueueReadBuffer(
				getDev()[channel].getReal(),
				isBlocking(),
				getDevOffset()    * sizeof(SampleType),
				getSampleLength() * sizeof(SampleType),
				getHost().getRawReal(channel) +
                getHostOffset()   * sizeof(SampleType),
                NULL,
                evtReal));

		fff_RTCLC_SEQ_CHECK_RET(
			queue.enqueueReadBuffer(
				getDev()[channel].getImag(),
				isBlocking(),
				getDevOffset()    * sizeof(SampleType),
				getSampleLength() * sizeof(SampleType),
				getHost().getRawImag(channel) +
                getHostOffset()   * sizeof(SampleType),
                NULL,
                evtImag));
	}

	void enqueueDeviceUpdate(
		UInt channel,
        ::cl::Event *evtReal=NULL,
        ::cl::Event *evtImag=NULL) const
	{
        fff_EXPECT_VALID_THIS();
		fff_RTCLC_ERR_INIT();

		const ::cl::CommandQueue &queue = getDev().getEnv().getQueue();

		fff_RTCLC_SEQ_CHECK_RET(
			queue.enqueueWriteBuffer(
				getDev()[channel].getReal(),
				isBlocking(),
				getDevOffset()    * sizeof(SampleType),
                getSampleLength() * sizeof(SampleType),
				getHost().getRawReal(channel) +
                getHostOffset()   * sizeof(SampleType),
                NULL,
                evtReal));

		fff_RTCLC_SEQ_CHECK_RET(
			queue.enqueueWriteBuffer(
				getDev()[channel].getImag(),
				isBlocking(),
				getDevOffset()    * sizeof(SampleType),
                getSampleLength() * sizeof(SampleType),
				getHost().getRawImag(channel) +
                getHostOffset()   * sizeof(SampleType),
                NULL,
                evtImag));
	}

    UInt getSampleLength() const
    {
        return
            m_sampleLength;
    }

    UInt getChannelCount() const
    {
        fff_EXPECT(
            getHost().getChannelCount(),
            ==,
            getDev().getChannelCount());

        return
            getHost().getChannelCount();
    }

	const IHostMultiChannel<SampleType> &getHost() const
	{
        fff_EXPECT_VALID_OBJ_RET(m_host);
	}

    IHostMultiChannel<SampleType> &getHost()
	{
        fff_EXPECT_VALID_OBJ_RET(m_host);
	}

    const DevMultiChannel<SampleType> &getDev() const
	{
        fff_EXPECT_VALID_OBJ_RET(m_dev);
	}

    DevMultiChannel<SampleType> &getDev()
	{
        fff_EXPECT_VALID_OBJ_RET(m_dev);
	}

	void setBlocking(
		Bool blockingRWs)
	{
		m_blocking = blockingRWs;
	}

    void setHostOffset(UInt offsetSamples)
    {
        m_hostSampleOffset = offsetSamples;
    }

    UInt getHostOffset() const
    {
        return m_hostSampleOffset;
    }

    void setDevOffset(UInt offsetSamples)
    {
        m_devSampleOffset = offsetSamples;
    }

    UInt getDevOffset() const
    {
        return m_devSampleOffset;
    }

	Bool isBlocking() const throw()
	{
		return
            m_blocking;
	}

    virtual void print(logstream &out) const
    {
        out("UbiMultiChannelBase");
        !fff_LOG_VAL(out, getHost());
        !fff_LOG_VAL(out, getDev());
        !fff_LOG_VAL(out, isBlocking());
        out();
    }

    bool operator!() const throw()
    {
        return
            !m_dev ||
            !m_host ||

            !(m_dev.getChannelCount() ==
            m_host.getChannelCount()) ||

            !(getSampleLength() <=
            getHost().getSampleCount()) ||
            
            !(getSampleLength() <=
            getDev().getSampleCount());
    }

private:

    DevMultiChannel<SampleType>
        &m_dev;

	IHostMultiChannel<SampleType>
		&m_host;

    UInt
        m_sampleLength;

	Bool
		m_blocking;

    UInt
        m_channelCount,
        m_hostSampleOffset,
        m_devSampleOffset;
};

}
}
}
}

#endif