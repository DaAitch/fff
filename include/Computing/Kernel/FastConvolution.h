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

#include "../Events.h"

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
        UInt ssize,
        UInt xsize,
		const UbiMultiChannel<SampleType> &H,
        UInt f
		)
		:
        KernelBase(
            compiler,
            fff_STRINGIFY(fff_KERNEL_FCONV)),
        m_useIn1(
            True),
		m_in1(
			compiler.getEnv()),
        m_in2(
			compiler.getEnv()),
        m_y(
            compiler.getEnv()),
		m_H(
			H),
		m_tmp1(
			compiler.getEnv()),
		m_tmp2(
			compiler.getEnv()),
        m_ssize(
            ssize),
        m_xsize(
            xsize)
	{

		fff_RTCLC_ERR_INIT();

        fff_EXPECT_VALID_OBJ(compiler);

        // x, s and y are not directly used in the kernel, but copied before,
        // though does not matter if readable or writable either

        fff_EXPECT_TRUE(
            getH().getDev().isReadable());

        ComputingData<SampleType> cd(
            m_ssize + 1);
        DeviceProperties devprops(getCompiler().getEnv().getDevice());
        Mapper map(cd, devprops, f);

		UInt extensionElementsCount = cl::calcOverlapSaveSampleCount(
			xsize + m_ssize,
			map.getLb2N(),
			m_ssize + 1);

		m_fftCount = 
			fff_CEILDIV(
				extensionElementsCount,
				fff_POW2(map.getLb2N()));

        

        m_lb2W = map.getLb2W();
        m_lb2N = map.getLb2N();

        fff_EXPECT(
            getH().getSampleLength(),
            ==,
            fff_POW2(map.getLb2N()));

		m_in1.alloc(
			CL_MEM_READ_ONLY,
            getH().getChannelCount(),
			m_ssize +
			m_xsize);

        m_in2.alloc(
			CL_MEM_READ_ONLY,
            getH().getChannelCount(),
			m_ssize +
			m_xsize);

        m_y.alloc(
            CL_MEM_WRITE_ONLY,
            getH().getChannelCount(),
            m_xsize);

        H.enqueueDeviceUpdate();


		m_tmp1.alloc(
			CL_MEM_READ_WRITE,
            getH().getChannelCount(),
			extensionElementsCount);
		m_tmp2.alloc(
			CL_MEM_READ_WRITE,
            getH().getChannelCount(),
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
            m_ssize +1,
            param);

        fff_EXPECT(
            param,
            ==,
            argsCount());
	}

    void invoke()
    {
        throw;
    }

	void invoke(
        UInt inputSampleCount,
        IHostMultiChannel<SampleType> &x,
        IHostMultiChannel<SampleType> &y)
	{
        fff_RTCLC_ERR_INIT();

        fff_RTCLC_SEQ_CHECK(
            getKernel().setArg(2,
                inputSampleCount + m_ssize));

        UbiMultiChannel<SampleType> ux(x, getCurrentIn());
        UbiMultiChannel<SampleType> uy(y, getY());

        ux.setSampleLength(inputSampleCount);
        uy.setSampleLength(inputSampleCount);

        ux.setDevOffset(m_ssize);

        UInt channelCount = getH().getChannelCount();

        // 1. copy x & run
		for(
			UInt channel = 0;
			channel < channelCount;
			++channel)
		{

            ux.enqueueDeviceUpdate(channel);

            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    0, getCurrentIn()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    1, getCurrentIn()[channel].getImag()));

            // 2 = xsize

            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    3, getH().getDev()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    4, getH().getDev()[channel].getImag()));
            
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    5, getTmp1()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    6, getTmp1()[channel].getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    7, getTmp2()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    8, getTmp2()[channel].getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    9, getY()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    10, getY()[channel].getImag()));
            
            enqueueNDRange(
                getFftCount() * fff_POW2(getLb2W()),
                fff_POW2(getLb2W()));

            getCompiler().getEnv().getQueue().flush();
                
		}

        // getCompiler().getEnv().getQueue().flush();

        Events e(channelCount<<1);

        for(UInt channel = 0;
            channel < channelCount;
            ++channel)
        {
            ::cl::Event eR, eI;
                uy.enqueueHostUpdate(
				    channel,
                    &eR, &eI);

            e.m_events[(channel<<1)    ] = eR;
            e.m_events[(channel<<1) + 1] = eI;
        }

        getCompiler().getEnv().getQueue().flush();

        for(UInt channel = 0;
            channel < channelCount;
            ++channel)
        {
            
			getCurrentIn()[channel].enqueueCopy(
				getOtherIn()[channel],
                inputSampleCount,
				0,
				m_ssize);
        }

        switchIn();

        e.wait();
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

    const UbiMultiChannel<SampleType> &getH() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_H);
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
            !m_in1 ||
            !m_in2 ||
            !m_tmp1 ||
            !m_tmp2 ||
            !m_H ||
            KernelBase::operator!();
    }

    const DevMultiChannel<SampleType> &getIn1() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_in1);
    }

    DevMultiChannel<SampleType> &getIn1()
    {
        fff_EXPECT_VALID_OBJ_RET(m_in1);
    }

    const DevMultiChannel<SampleType> &getIn2() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_in2);
    }

    DevMultiChannel<SampleType> &getIn2()
    {
        fff_EXPECT_VALID_OBJ_RET(m_in2);
    }

    const DevMultiChannel<SampleType> &getY() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_y);
    }

    DevMultiChannel<SampleType> &getY()
    {
        fff_EXPECT_VALID_OBJ_RET(m_y);
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

    DevMultiChannel<SampleType> &getCurrentIn()
    {
        return
            m_useIn1 ?
            getIn1() : getIn2();
    }

    const DevMultiChannel<SampleType> &getCurrentIn() const
    {
        return
            m_useIn1 ?
            getIn1() : getIn2();
    }

    DevMultiChannel<SampleType> &getOtherIn()
    {
        return
            m_useIn1 ?
            getIn2() : getIn1();
    }

    const DevMultiChannel<SampleType> &getOtherIn() const
    {
        return
            m_useIn1 ?
            getIn2() : getIn1();
    }

    void switchIn()
    {
        m_useIn1 =
            !m_useIn1;
    }

private:

    Bool
        m_useIn1;

	DevMultiChannel<SampleType>
		m_in1,
        m_in2,
        m_y,
		m_tmp1,
		m_tmp2;

	const UbiMultiChannel<SampleType>
		&m_H;

    UInt
        m_fftCount;

    UInt
        m_lb2W;

    UInt
        m_lb2N,
        m_xsize,
        m_ssize;
};

}
}
}

#endif