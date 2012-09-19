//---------------------------------------------------------+
// fff/include/Computing/Kernel/Convolution.h
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
//!	\file		Convolution.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Discrete (not fast) convolution kernel.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffconvkernel_h__included__
#define __fffconvkernel_h__included__

#include "../../_intern.h"
#include "../OpenCLEnvironment.h"
#include "../Compiler.h"
#include "../../Buffer/Complex/Ubiquitous/UbiMultiChannel.h"
#include "KernelBase.h"
#include "../../../cl/_.h.cl"

using namespace fff::Buffer::Complex::Ubiquitous;
using namespace fff::Computing;

namespace fff {
namespace Computing {
namespace Kernel {

template<
	class SampleType
>
class Convolution
    :
    public KernelBase
{
public:

Convolution(
		const Compiler &compiler,
		const UbiMultiChannel<SampleType> &x,
		const UbiMultiChannel<SampleType> &h,
		UbiMultiChannel<SampleType> &y)
		:
        KernelBase(
            compiler,
            fff_STRINGIFY(
				fff_KERNEL_DCONV)),
		m_x(
			x),
		m_h(
			h),
		m_y(
			y)
	{
        fff_EXPECT_VALID_OBJ(compiler);
        fff_EXPECT_VALID_OBJ(x);
        fff_EXPECT_VALID_OBJ(h);
        fff_EXPECT_VALID_OBJ(y);



        
        
		


		fff_RTCLC_ERR_INIT();

        UInt param=0;

		param++; // x
        param++; // x size
        param++; // h
        param++; // y


        param = dontOptimizeArgs(
            0,
            0,
            getH().getSampleLength(),
            param);

        fff_EXPECT(argsCount(), ==, param);
	}

    const UbiMultiChannel<SampleType> &getX() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_x);
    }

    const UbiMultiChannel<SampleType> &getH() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_h);
    }

    const UbiMultiChannel<SampleType> &getY() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_y);
    }

    UbiMultiChannel<SampleType> &getY()
    {
        fff_EXPECT_VALID_OBJ_RET(m_y);
    }



	void invoke()
	{
        fff_EXPECT_TRUE(getX().getDev().isReadable());
        fff_EXPECT_TRUE(getH().getDev().isReadable());
        fff_EXPECT_TRUE(getY().getDev().isWritable());

        fff_EXPECT(getX().getChannelCount(), ==, getH().getChannelCount());
        fff_EXPECT(getX().getChannelCount(), ==, getY().getChannelCount());

        fff_EXPECT(
            getX().getSampleLength() +
			getH().getSampleLength() - 1,
            <=,
			getY().getSampleLength());

        fff_RTCLC_ERR_INIT();

        fff_EXPECT_VALID_THIS();

		for(
			UInt channel = 0;
			channel < getX().getChannelCount();
			++channel)
		{
            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    0, getX().getDev()[channel].getReal()));
            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    1, getX().getSampleLength()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    2, getH().getDev()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    3, getY().getDev()[channel].getReal()));

			getX().enqueueDeviceUpdate(
				channel);
			getH().enqueueDeviceUpdate(
				channel);

            // concurrent, let opencl decide which
            // W to use
			enqueueNDRange(
                getX().getSampleLength() +
                getH().getSampleLength() - 1,
                0);

			getY().enqueueHostUpdate(
				channel);
		}
	}

    
    bool operator!() const throw()
    {
        return
            KernelBase::operator!() ||
            !m_x ||
            !m_h ||
            !m_y;
    }

    virtual void print(logstream &out) const
    {
        out("Convolution");
        KernelBase::print(out);
        !fff_LOG_VAL(out, getX());
        !fff_LOG_VAL(out, getH());
        !fff_LOG_VAL(out, getY());
        out();
    }

private:

	const UbiMultiChannel<SampleType>
		&m_x,
		&m_h;
	UbiMultiChannel<SampleType>
		&m_y;
};

}
}
}
#endif