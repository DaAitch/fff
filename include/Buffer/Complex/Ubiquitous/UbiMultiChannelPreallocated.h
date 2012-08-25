
#ifndef __ubimultichannelpreallocated_h__included__
#define __ubimultichannelpreallocated_h__included__

#include "../../../_intern.h"
#include "../../../Computing/OpenCLEnvironment.h"
#include "IUbiMultiChannel.h"
#include "../Host/HostMultiChannelPreallocated.h"

using namespace fff::Buffer::Complex::Host;

namespace fff {
namespace Buffer {
namespace Complex {
namespace Ubiquitous {

template<
    class SampleType
>
class UbiMultiChannelPreallocated
    :
    public IUbiMultiChannel<SampleType>
{
public:
    typedef UbiMultiChannelPreallocated<SampleType> My;
public:
    UbiMultiChannelPreallocated(
        OpenCLEnvironment &env,
        cl_mem_flags flags,
        UInt channelCount=0,
        UInt maxSampleCount=0,
        Bool blockingRWs=False)
        :
        IUbiMultiChannel(
            env,
            maxSampleCount,
            blockingRWs),
        m_memFlags(
            flags)
    {
        m_hostBuffer.alloc(channelCount, maxSampleCount);
        allocMax(channelCount, maxSampleCount);
        m_ubiBuffer.setHostMultiChannel(
            &getHostBuffer());
    }

    ~UbiMultiChannelPreallocated() throw()
    {
    }

    virtual IHostMultiChannel<SampleType> &getHostBuffer()
    {
        return
            getPreallocatedBuffer();
    }

	virtual const IHostMultiChannel<SampleType> &getHostBuffer() const
    {
        return
            getPreallocatedBuffer();
    }

    const HostMultiChannelPreallocated<SampleType> &getPreallocatedBuffer() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_hostBuffer);
    }

    HostMultiChannelPreallocated<SampleType> &getPreallocatedBuffer()
    {
        fff_EXPECT_VALID_OBJ_RET(m_hostBuffer);
    }

    void allocMax(UInt channelCount, UInt sampleCount)
    {
        m_ubiBuffer.alloc(getMemFlags(), channelCount, sampleCount);
        m_hostBuffer.alloc(channelCount, sampleCount);
        m_hostBuffer.setPointer(NULL, sampleCount);
    }

    void setTarget(SampleType **samples, UInt sampleCount)
    {
        m_hostBuffer.setPointer(samples, sampleCount);
    }

    cl_mem_flags getMemFlags() const
    {
        return
            m_memFlags;
    }

    virtual Bool isFix() const throw()
    {
        return
            False;
    }

    virtual bool operator!() const throw()
    {
        return
            false;
    }

private:
    HostMultiChannelPreallocated<SampleType> m_hostBuffer;

    cl_mem_flags m_memFlags;
};

}
}
}
}

#endif