

#ifndef __iubimultichannel_h__included__
#define __iubimultichannel_h__included__

#include "../../../_intern.h"
#include "../../../Computing/OpenCLEnvironment.h"
#include "../Host/IHostMultiChannel.h"
#include "UbiMultiChannelBase.h"


using namespace fff::Buffer::Complex::Host;


namespace fff {
namespace Buffer {
namespace Complex {
namespace Ubiquitous {

template<
    class SampleType
>
class IUbiMultiChannel
    : public iprintable
{
public:
    typedef IUbiMultiChannel<SampleType> My;

public:

    IUbiMultiChannel(
        const OpenCLEnvironment &env,
        cl_mem_flags flags,
        UInt sampleCount,
        Bool blockingRWs=False)
        :
        m_ubiBuffer(
            env,
            flags,
            sampleCount,
            blockingRWs)
    {
    }


    virtual IHostMultiChannel<SampleType> &getHostBuffer() = 0;
	virtual const IHostMultiChannel<SampleType> &getHostBuffer() const = 0;

    virtual Bool isFix() const throw() = 0;

	UbiMultiChannelBase<SampleType> &getUbiBuffer()
	{
		fff_EXPECT_VALID_OBJ_RET(m_ubiBuffer);
	}

	const UbiMultiChannelBase<SampleType> &getUbiBuffer() const
	{
		fff_EXPECT_VALID_OBJ_RET(m_ubiBuffer);
	}

public:

    Bool hasSameSize(const My &rhs) const
    {
        fff_EXPECT_VALID_THIS();

        return
            getHostBuffer().hasSameSize(rhs.getHostBuffer()) &&
            getUbiBuffer().hasSameSize(rhs.getUbiBuffer());
    }

    Bool hasSameSampleCount(const My &rhs) const
    {
        return
            getHostBuffer().hasSameSampleCount(rhs.getHostBuffer()) &&
            getUbiBuffer().hasSameSampleCount(rhs.getUbiBuffer());
    }

    Bool hasSameChannelCount(const My &rhs) const
    {
        return
            getHostBuffer().hasSameChannelCount(rhs.getHostBuffer());
    }

    virtual void print(logstream &out) const
    {
        out("IUbiMultiChannel");
        !fff_LOG_VAL(out, getHostBuffer());
        !fff_LOG_VAL(out, getUbiBuffer());
        out();
    }

    virtual bool operator!() const throw()
    {
        return
            !m_ubiBuffer;
    }

protected:
    UbiMultiChannelBase<SampleType> m_ubiBuffer;


};

}
}
}
}

#endif