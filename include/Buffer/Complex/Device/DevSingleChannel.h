//---------------------------------------------------------+
// fff/include/Buffer/Complex/Device/DevSingleChannel.h
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
//!	\file		DevSingleChannel.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Single channel complex vector device
//!				mapping buffer. No host buffer.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffcvdevschbuf_h__included__
#define __fffcvdevschbuf_h__included__

#include "../../../_intern/_base.h"
#include "../../../Computing/OpenCLEnvironment.h"

using namespace fff::Computing;

namespace fff {
namespace Buffer {
namespace Complex {
namespace Device {

template<
	class SampleType
>
class DevSingleChannel
    :
    public iprintable
{
public:
	typedef SampleType MySample;
	typedef DevSingleChannel<MySample> My;

    DevSingleChannel()
        :
        m_sampleCount(0),
        m_memFlags(0)
    {
    }

	DevSingleChannel(
		const OpenCLEnvironment &env,
		cl_mem_flags flags,
		UInt sampleCount)
		:
		m_env(
			env),
		m_sampleCount(
			0),
		m_memFlags(
			0)
	{
		alloc(
			flags,
			sampleCount);
	}

	DevSingleChannel(
		const OpenCLEnvironment &env)
		:
		m_env(
			env),
		m_sampleCount(
			0),
		m_memFlags(
			0)
	{
	}

	virtual ~DevSingleChannel() throw()
	{
		dealloc();
	}

	void copy(
		const My &rhs)
	{
		dealloc();
		m_env = rhs.getEnv();
		alloc(rhs.getMemFlags(),
			rhs.getSampleCount());

		rhs.enqueueCopy(
			fff_ME);
	}


    My createSubBuffer(
        cl_mem_flags flags,
        UInt sampleOffset,
        UInt sampleCount)
    {
        fff_RTCLC_ERR_INIT();

        cl_buffer_region region;
        region.origin = getSampleSize() * sampleOffset;
        region.size = getSampleSize() * sampleCount;

        fff_EXPECT(
            region.origin + region.size,
            <=,
            getPerBufferSize());

        ::cl::Buffer subBufferRe, subBufferIm;

        fff_RTCLC_SEQ_CHECK(
            subBufferRe = getReal().createSubBuffer(
                flags,
                CL_BUFFER_CREATE_TYPE_REGION,
                &region,
                fff_RTCLC_ERR_PTR));

        fff_RTCLC_SEQ_CHECK(
            subBufferIm = getImag().createSubBuffer(
                flags,
                CL_BUFFER_CREATE_TYPE_REGION,
                &region,
                fff_RTCLC_ERR_PTR));

        My sch;
        sch.m_env = getEnv();
        sch.m_memFlags = flags;
        sch.m_sampleCount = sampleCount;
        sch.m_re = subBufferRe;
        sch.m_im = subBufferIm;

        return sch;
    }


	void alloc(
		cl_mem_flags flags,
		UInt sampleCount)
	{
		dealloc();

		if(sampleCount > 0)
		{
			_alloc(
				sampleCount,
				flags);

			fff_EXPECT_TRUE(
				isAllocated());
		}
		else
			fff_EXPECT_FALSE(
				isAllocated());
	}

	void dealloc() throw()
	{
		if(isAllocated())
			_dealloc();
	}

	Bool isAllocated() const throw()
	{
		return _isAllocated();
	}


	::cl::Buffer &getReal()
	{
        fff_EXPECT_VALID_CLOBJ_RET(m_re);
	}

	const ::cl::Buffer &getReal() const
	{
        fff_EXPECT_VALID_CLOBJ_RET(m_re);
	}

	::cl::Buffer &getImag()
	{
        fff_EXPECT_VALID_CLOBJ_RET(m_im);
	}

	const ::cl::Buffer &getImag() const
	{
        fff_EXPECT_VALID_CLOBJ_RET(m_im);
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
        return sizeof(MySample);
    }

	void enqueueCopy(
		My &dstBuffer,
		UInt offsetSrc,
		UInt offsetDst,
		UInt sampleCount,
        ::cl::Event *evtReal=NULL,
        ::cl::Event *evtImag=NULL) const
	{
		fff_RTCLC_ERR_INIT();

		fff_EXPECT(offsetDst + sampleCount, <=, dstBuffer.getSampleCount());
		fff_EXPECT(offsetSrc + sampleCount, <=, getSampleCount());

        if(sampleCount > 0)
        {

		    fff_RTCLC_SEQ_CHECK_RET(
			    m_env.getQueue().enqueueCopyBuffer(
				    getReal(),
				    dstBuffer.getReal(),
				    offsetSrc * sizeof(MySample),
				    offsetDst * sizeof(MySample),
				    sampleCount * sizeof(MySample),
                    NULL,
                    evtReal));

		    fff_RTCLC_SEQ_CHECK_RET(
			    m_env.getQueue().enqueueCopyBuffer(
				    getImag(),
				    dstBuffer.getImag(),
				    offsetSrc * sizeof(MySample),
				    offsetDst * sizeof(MySample),
				    sampleCount * sizeof(MySample),
                    NULL,
                    evtImag));
        }
	}

	void enqueueCopy(
		My &dstBuffer,
        ::cl::Event *evtReal=NULL,
        ::cl::Event *evtImag=NULL) const
	{
        fff_EXPECT_VALID_THIS();
        fff_EXPECT_VALID_OBJ(dstBuffer);
		fff_EXPECT(getSampleCount(), ==, dstBuffer.getSampleCount());

		enqueueCopy(
			dstBuffer,
			0,
			0,
			getSampleCount(),
            evtReal,
            evtImag);
	}

	void enqueueCopy(
		My &dstBuffer,
		UInt offsetDst,
		UInt sampleCount,
        ::cl::Event *evtReal=NULL,
        ::cl::Event *evtImag=NULL) const
	{
        fff_EXPECT_VALID_THIS();
        fff_EXPECT_VALID_OBJ(dstBuffer);
        
		fff_EXPECT(sampleCount, <=, dstBuffer.getSampleCount() - offsetDst);
		fff_EXPECT(getSampleCount(), >=, sampleCount);

		enqueueCopy(
			dstBuffer,
			0,
			offsetDst,
			sampleCount,
            evtReal,
            evtImag);
	}

	const OpenCLEnvironment &getEnv() const
	{
        fff_EXPECT_VALID_OBJ_RET(m_env);
	}

    OpenCLEnvironment &getEnv()
	{
        fff_EXPECT_VALID_OBJ_RET(m_env);
	}

	::cl_mem_flags getMemFlags() const
	{
		return m_memFlags;
	}

    virtual void print(logstream &out) const
    {
        out("DevSingleChannel");
        !fff_LOG_VAL(out, getSampleCount());
        !fff_LOG_VAL(out, getMemFlags());
        !fff_LOG_VAL(out, isReadable());
        !fff_LOG_VAL(out, isWritable());
        !fff_LOG_VAL(out, getEnv());
        !fff_LOG_VAL(out, getPerBufferSize());
        out();
    }

    bool operator!() const throw()
    {
        return
            !m_env ||
		    !m_re() ||
		    !m_im() ||
		    m_sampleCount==0 ||
		    m_memFlags==0;
    }

    bool operator!=(const My &rhs) const
    {
        return
            !(fff_ME == rhs);
    }

    bool operator==(const My &rhs) const
    {
        fff_EXPECT_VALID_THIS();
        fff_EXPECT_VALID_OBJ(rhs);

        bool ret = true;
        ret &= (getPerBufferSize() == rhs.getPerBufferSize());
        ret &= (getSampleCount() == rhs.getSampleCount());
        ret &= (getReal()() == rhs.getReal()());
        ret &= (getImag()() == rhs.getImag()());
        ret &= (isAllocated() == rhs.isAllocated());
        ret &= (getMemFlags() == rhs.getMemFlags());

        return ret;
    }

    bool isWritable() const throw()
    {
        return
            m_memFlags == CL_MEM_WRITE_ONLY ||
            m_memFlags == CL_MEM_READ_WRITE;
    }

    bool isReadable() const throw()
    {
        return
            m_memFlags == CL_MEM_READ_ONLY ||
            m_memFlags == CL_MEM_READ_WRITE;
    }

    virtual Bool hasSameSize(const My &rhs) const
    {
        if(getSampleCount() != rhs.getSampleCount())
            return False;

        fff_RTCLC_ERR_INIT();

        ::size_t myReSize, myImSize, rhsReSize, rhsImSize;
        

        fff_RTCLC_SEQ_CHECK(
            myReSize = getReal().getInfo<CL_MEM_SIZE>(fff_RTCLC_ERR_PTR));

        fff_RTCLC_SEQ_CHECK(
            myImSize = getImag().getInfo<CL_MEM_SIZE>(fff_RTCLC_ERR_PTR));

        fff_RTCLC_SEQ_CHECK(
            rhsReSize = rhs.getReal().getInfo<CL_MEM_SIZE>(fff_RTCLC_ERR_PTR));

        fff_RTCLC_SEQ_CHECK(
            rhsImSize = rhs.getImag().getInfo<CL_MEM_SIZE>(fff_RTCLC_ERR_PTR));

        return
            myReSize == rhsReSize &&
            myImSize == rhsImSize;
    }

private:

	void _alloc(
		UInt sampleCount,
		cl_mem_flags flags)
	{
		fff_RTCLC_ERR_INIT();

        fff_EXPECT_FALSE(isAllocated());
		fff_EXPECT(sampleCount, >, 0);
	

		m_sampleCount = sampleCount;
        m_memFlags = flags;
	
		fff_RTCLC_SEQ_CHECK(
			m_re = ::cl::Buffer(
				m_env.getContext(),
				flags,
				getPerBufferSize(),
				NULL,
				fff_RTCLC_ERR_PTR));

		fff_RTCLC_SEQ_CHECK(
			m_im = ::cl::Buffer(
				m_env.getContext(),
				flags,
				getPerBufferSize(),
				NULL,
				fff_RTCLC_ERR_PTR));

		fff_EXPECT_TRUE(
			isAllocated());
	}

	void _dealloc() throw()
	{
		m_sampleCount = 0;
        m_memFlags = 0;
        
		m_re = ::cl::Buffer();
		m_im = ::cl::Buffer();
	}

	Bool _isAllocated() const throw()
	{
		return
			m_re() &&
			m_im();
	}

    

private:
	OpenCLEnvironment
		m_env;
	::cl::Buffer
		m_re,
		m_im;
	UInt
		m_sampleCount;

	::cl_mem_flags
		m_memFlags;


};

}
}
}
}

#endif