//---------------------------------------------------------+
// fff/include/fffFConvKernel.h
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
//!	\file		fffFConvKernel.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Fast convolution kernel OpenCL interface.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __ffffconvkernel_h__included__
#define __ffffconvkernel_h__included__

#include "../../_intern/_base.h"
#include "../OpenCLEnvironment.h"
#include "../../Buffer/Complex/Ubiquitous/UbiMultiChannel.h"
#include "../Compiler.h"
#include "../RuntimeMapping/Mapper.h"

using namespace fff::Buffer;
using namespace fff::Buffer::Complex;
using namespace fff::Buffer::Complex::Host;
using namespace fff::Buffer::Complex::Device;
using namespace fff::Buffer::Complex::Ubiquitous;

using namespace fff::Computing;
using namespace fff::Computing::RuntimeMapping;

namespace fff {
namespace Computing {
namespace Kernel {

template<
	class SampleType
>
class FastConvolution
    : public KernelBase
{
public:
	typedef
		FastConvolution<SampleType> My;

public:
	FastConvolution(
		const Compiler &compiler,
		IUbiMultiChannel<SampleType> &s,
		const IUbiMultiChannel<SampleType> &x,
		const IUbiMultiChannel<SampleType> &H,
		IUbiMultiChannel<SampleType> &y,
        UInt f
		)
		:
        KernelBase(
            compiler,
            fff_STRINGIFY(fff_KERNEL_FCONV)),
		m_in(
			compiler.getEnv()),
		m_s(
			s),
		m_x(
			x),
		m_H(
			H),
		m_tmp1(
			compiler.getEnv()),
		m_tmp2(
			compiler.getEnv()),
		m_y(
			y)
	{

		fff_RTCLC_ERR_INIT();

        fff_EXPECT_VALID_OBJ(compiler);
		fff_EXPECT_VALID_OBJ(getS());
        fff_EXPECT_VALID_OBJ(getY());
        fff_EXPECT_VALID_OBJ(getX());

        if(getX().isFix())
            fff_EXPECT_TRUE(
                getH().hasSameChannelCount(
                    getX())
            );


		fff_EXPECT_TRUE(
            getH().hasSameChannelCount(
                getS())
        );

        if(getX().isFix())
            fff_EXPECT_TRUE(
                getX().hasSameSize(
                    getY())
            );

        // x, s and y are not directly used in the kernel, but copied before,
        // though does not matter if readable or writable either

        fff_EXPECT_TRUE(
            getH().getUbiBuffer().isReadable());

        ComputingData<SampleType> cd(
            getS().getHostBuffer().getSampleCount()+1);
        DeviceProperties devprops(getCompiler().getEnv().getDevice());
        Mapper map(cd, devprops, f);

		UInt extensionElementsCount = cl::calcOverlapSaveSampleCount(
			getX().getHostBuffer().getSampleCount() +
			getS().getHostBuffer().getSampleCount(),
			map.getLb2N(),
			getS().getHostBuffer().getSampleCount() + 1);

		m_fftCount = 
			fff_CEILDIV(
				extensionElementsCount,
				fff_POW2(map.getLb2N()));

        

        m_lb2W = map.getLb2W();
        m_lb2N = map.getLb2N();

        fff_EXPECT(
            getH().getHostBuffer().getSampleCount(),
            ==,
            fff_POW2(map.getLb2N()));

		m_in.alloc(
			CL_MEM_READ_ONLY,
            getH().getHostBuffer().getChannelCount(),
			getS().getHostBuffer().getSampleCount() +
			getX().getHostBuffer().getSampleCount());
		m_tmp1.alloc(
			CL_MEM_READ_WRITE,
            getH().getHostBuffer().getChannelCount(),
			extensionElementsCount);
		m_tmp2.alloc(
			CL_MEM_READ_WRITE,
            getH().getHostBuffer().getChannelCount(),
			extensionElementsCount);

        UInt param = 0;

		param++; // in real
        param++; // in imag
        param++; // in size
        param++; // H real
        param++; // H imag
        param++; // tmp1 real
        param++; // tmp1 imag
        param++; // tmp2 real
        param++; // tmp2 real
        param++; // y real
        param++; // y imag

   

        param = dontOptimizeArgs(
            getLb2N(),
            getLb2W(),
            getS().getHostBuffer().getSampleCount()+1,
            param);

        fff_EXPECT(
            param,
            ==,
            argsCount());
	}

	void invoke()
	{
        fff_EXPECT_TRUE(
            getH().hasSameChannelCount(
                getX())
        );

        fff_EXPECT(
            getX().getHostBuffer().getSampleCount(),
            <=,
            getY().getHostBuffer().getSampleCount()
        );

        fff_RTCLC_ERR_INIT();

        fff_RTCLC_SEQ_CHECK(
            getKernel().setArg(2,
                getS().getHostBuffer().getSampleCount() +
                getX().getHostBuffer().getSampleCount()));

        const UInt
                sSize = getS().getHostBuffer().getSampleCount(),
                xSize = getX().getHostBuffer().getSampleCount();

		for(
			UInt channel = 0;
			channel < getS().getHostBuffer().getChannelCount();
			++channel)
		{
            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    0, getIn().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    1, getIn().getChannel(channel).getImag()));

            // 2 = xsize

            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    3, getH().getUbiBuffer().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    4, getH().getUbiBuffer().getChannel(channel).getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    5, getTmp1().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    6, getTmp1().getChannel(channel).getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    7, getTmp2().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    8, getTmp2().getChannel(channel).getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    9, getY().getUbiBuffer().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    10, getY().getUbiBuffer().getChannel(channel).getImag()));

			getS().getUbiBuffer().enqueueDeviceUpdate(
				channel);
                
			getS().getUbiBuffer().getChannel(channel).enqueueCopy(
				getIn().getChannel(channel),
				0,
				sSize);

			getX().getUbiBuffer().enqueueDeviceUpdate(
				channel);

			getX().getUbiBuffer().getChannel(channel).enqueueCopy(
				getIn().getChannel(channel),
				sSize,
				xSize);
   
            getH().getUbiBuffer().enqueueDeviceUpdate(channel);

            enqueueNDRange(
                getFftCount() * fff_POW2(getLb2W()),
                fff_POW2(getLb2W()));
                
			getY().getUbiBuffer().enqueueHostUpdate(
				channel);

			getIn().getChannel(channel).enqueueCopy(
				getS().getUbiBuffer().getChannel(channel),
                xSize,
				0,
				sSize);

			getS().getUbiBuffer().enqueueHostUpdate(
				channel);
                
            getCompiler().getEnv().getQueue().flush();


            
		}
	}

    UInt getFftCount() const
    {
        return  
            m_fftCount;
    }

    UInt getLb2W() const
    {
        return
            m_lb2W;
    }

    UInt getLb2N() const
    {
        return
            m_lb2N;
    }

    IUbiMultiChannel<SampleType> &getS()
    {
        fff_EXPECT_VALID_OBJ_RET(m_s);
    }

    const IUbiMultiChannel<SampleType> &getS() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_s);
    }

    IUbiMultiChannel<SampleType> &getY()
    {
        fff_EXPECT_VALID_OBJ_RET(m_y);
    }

    const IUbiMultiChannel<SampleType> &getY() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_y);
    }

    const IUbiMultiChannel<SampleType> &getH() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_H);
    }

    const IUbiMultiChannel<SampleType> &getX() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_x);
    }

    /*
    virtual void print(logstream &out) const
    {
        out("FastConvolution");
        KernelBase::print(out);

        getX().print(out);
        getH().print(out);
        getY().print(out);
        out();
    }*/

    bool operator!() const throw()
    {
        return
            !m_in ||
            !m_tmp1 ||
            !m_tmp2 ||
            !m_x ||
            !m_s ||
            !m_H ||
            !m_y ||
            KernelBase::operator!();
    }
		

private:

    const DevMultiChannel<SampleType> &getIn() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_in);
    }

    DevMultiChannel<SampleType> &getIn()
    {
        fff_EXPECT_VALID_OBJ_RET(m_in);
    }

    const DevMultiChannel<SampleType> &getTmp1() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_tmp1);
    }

    DevMultiChannel<SampleType> &getTmp1()
    {
        fff_EXPECT_VALID_OBJ_RET(m_tmp1);
    }

    const DevMultiChannel<SampleType> &getTmp2() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_tmp2);
    }

    DevMultiChannel<SampleType> &getTmp2()
    {
        fff_EXPECT_VALID_OBJ_RET(m_tmp2);
    }

	DevMultiChannel<SampleType>
		m_in,
		m_tmp1,
		m_tmp2;

	IUbiMultiChannel<SampleType>
		&m_s,
		&m_y;
	const IUbiMultiChannel<SampleType>
		&m_H,
		&m_x;

    UInt
        m_fftCount;

    UInt
        m_lb2W;

    UInt
        m_lb2N;
};

}
}
}

#endif