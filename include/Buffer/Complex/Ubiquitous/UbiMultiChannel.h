#ifndef __ubibufpair_h__included__
#define __ubibufpair_h__included__
	
#include "../../../_intern.h"
#include "../../../Computing/OpenCLEnvironment.h"

#include "../Host/HostMultiChannel.h"
#include "UbiMultiChannelBase.h"
#include "IUbiMultiChannel.h"

using namespace fff::Buffer::Complex::Ubiquitous;

namespace fff {
namespace Buffer {
namespace Complex {
namespace Ubiquitous {

template<
	class SampleType
>
class UbiMultiChannel
    : public IUbiMultiChannel<SampleType>
{
public:
    typedef UbiMultiChannel<SampleType> My;

public:

	UbiMultiChannel(
		const OpenCLEnvironment &env,
        cl_mem_flags flags,
        UInt channelCount,
        UInt sampleCount,
        Bool blockingRWs=False)
		:
        IUbiMultiChannel(
            env,
            sampleCount,
            blockingRWs),
        m_hostBuffer(
            channelCount,
            sampleCount)
	{
        fff_EXPECT_VALID_OBJ(getHostBuffer());
                
        m_ubiBuffer.setHostMultiChannel(
            &getHostBuffer());
        m_ubiBuffer.alloc(flags, sampleCount);

        fff_EXPECT_VALID_THIS();
	}

    ~UbiMultiChannel() throw()
    {
    }



private:
    UbiMultiChannel(const UbiMultiChannel &rhs)
    {
        // dont!
    }

    UbiMultiChannel &operator=(const UbiMultiChannel &rhs)
    {
        // dont!
        return fff_ME;
    }

public:

    virtual IHostMultiChannel<SampleType> &getHostBuffer()
	{
        return
            getMultiBuffer();
	}

	virtual const IHostMultiChannel<SampleType> &getHostBuffer() const
	{
		return
            getMultiBuffer();
	}

    virtual Bool isFix() const throw()
    {
        return
            True;
    }

    HostMultiChannel<SampleType> &getMultiBuffer()
	{
        fff_EXPECT_VALID_OBJ_RET(m_hostBuffer);
	}

	const HostMultiChannel<SampleType> &getMultiBuffer() const
	{
		fff_EXPECT_VALID_OBJ_RET(m_hostBuffer);
	}

    virtual bool operator!() const throw()
    {
        return
            !m_hostBuffer ||
            IUbiMultiChannel::operator!();
    }



	private:
	    HostMultiChannel<SampleType>
		    m_hostBuffer;

};
	
}
}
}
}
#endif