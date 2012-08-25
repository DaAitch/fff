//---------------------------------------------------------+
// fff/include/fffConvKernel.h
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
//!	\file		fffConvKernel.h
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
		const IUbiMultiChannel<SampleType> &x,
		const IUbiMultiChannel<SampleType> &h,
		IUbiMultiChannel<SampleType> &y)
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

        fff_EXPECT_TRUE(getX().getHostBuffer().isAllocated());
        fff_EXPECT_TRUE(getX().getUbiBuffer().isReadable());
        
        fff_EXPECT_TRUE(getH().getHostBuffer().isAllocated());
        fff_EXPECT_TRUE(getH().getUbiBuffer().isReadable());

        fff_EXPECT_TRUE(getY().getHostBuffer().isAllocated());
        fff_EXPECT_TRUE(getY().getUbiBuffer().isWritable());

        UInt channelsX, channelsY, channelsH;

        channelsX = m_x.getHostBuffer().getChannelCount();
        channelsY = m_y.getHostBuffer().getChannelCount();
        channelsH = m_h.getHostBuffer().getChannelCount();

        fff_EXPECT(channelsX, ==, channelsY);
        fff_EXPECT(channelsX, ==, channelsH);

        fff_EXPECT_TRUE(
            compiler.getWorker().isMOkay(
                getH().getHostBuffer().getSampleCount()));
        
		fff_EXPECT(
            getX().getHostBuffer().getSampleCount() +
			getH().getHostBuffer().getSampleCount() - 1,
            ==,
			getY().getHostBuffer().getSampleCount());


		fff_RTCLC_ERR_INIT();

        UInt param=0;

		param++; // x
        param++; // x size
        param++; // h
        param++; // y


        param = dontOptimizeArgs(
            0,
            0,
            getH().getHostBuffer().getSampleCount(),
            param);

        fff_EXPECT(argsCount(), ==, param);
	}

    const IUbiMultiChannel<SampleType> &getX() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_x);
    }

    const IUbiMultiChannel<SampleType> &getH() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_h);
    }

    const IUbiMultiChannel<SampleType> &getY() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_y);
    }

    IUbiMultiChannel<SampleType> &getY()
    {
        fff_EXPECT_VALID_OBJ_RET(m_y);
    }



	void invoke()
	{
        fff_RTCLC_ERR_INIT();

        fff_EXPECT_VALID_THIS();

		for(
			UInt channel = 0;
			channel < getX().getHostBuffer().getChannelCount();
			++channel)
		{
            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    0, getX().getUbiBuffer().getChannel(channel).getReal()));
            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    1, getX().getHostBuffer().getSampleCount()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    2, getH().getUbiBuffer().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    3, getY().getUbiBuffer().getChannel(channel).getReal()));

			getX().getUbiBuffer().enqueueDeviceUpdate(
				channel);
			getH().getUbiBuffer().enqueueDeviceUpdate(
				channel);

            // concurrent, let opencl decide which
            // W to use
			enqueueNDRange(
                getX().getHostBuffer().getSampleCount(),
                0);

			getY().getUbiBuffer().enqueueHostUpdate(
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

	const IUbiMultiChannel<SampleType>
		&m_x,
		&m_h;
	IUbiMultiChannel<SampleType>
		&m_y;
};

}
}
}
#endif