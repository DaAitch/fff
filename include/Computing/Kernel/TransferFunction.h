//---------------------------------------------------------+
// fff/include/fffTFuncKernel.h
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
//!	\file		fffTFuncKernel.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Transfer function kernel gets the spectral
//!				representation of the continuous transfer
//!				function of the IIR.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __ffftfunckernel_h__included__
#	define __ffftfunckernel_h__included__

#	include "../../_intern/_base.h"
#	include "../../Buffer/Complex/Device/DevMultiChannel.h"
#include "../../Buffer/Complex/Ubiquitous/UbiMultiChannel.h"
#include "KernelBase.h"

using namespace fff::Buffer::Complex::Ubiquitous;

namespace fff {
namespace Computing {
namespace Kernel {

template<
	class SampleType
>
class TransferFunction
    : public KernelBase
{
public:

	TransferFunction(
		const Compiler &compiler,
		const IUbiMultiChannel<SampleType> &b,
		const IUbiMultiChannel<SampleType> &a,
		IUbiMultiChannel<SampleType> &H)
		:
        KernelBase(
            compiler,
            fff_STRINGIFY(fff_KERNEL_TFUNC)
            ),
		m_b(
			b),
		m_a(
			a),
		m_tmp(
			compiler.getEnv()),
		m_H(
			H)
	{
        fff_EXPECT_VALID_OBJ(compiler);
        fff_EXPECT_VALID_OBJ(getB());
        fff_EXPECT_VALID_OBJ(getA());
        fff_EXPECT_VALID_OBJ(getH());

        fff_EXPECT(
            getB().getHostBuffer().getChannelCount(),
            ==,
            getA().getHostBuffer().getChannelCount());

        fff_EXPECT(
            getB().getHostBuffer().getChannelCount(),
            ==,
            getH().getHostBuffer().getChannelCount());

        fff_EXPECT(
            getA().getHostBuffer().getSampleCount(),
            ==,
            getB().getHostBuffer().getSampleCount());

        fff_EXPECT(
            getA().getHostBuffer().getSampleCount(),
            ==,
            getH().getHostBuffer().getSampleCount());

        fff_EXPECT_TRUE(
            getCompiler().getWorker().isBlockNOkay(
                getA().getHostBuffer().getSampleCount()));

        fff_EXPECT_TRUE(
            cl::isMultiple2(getA().getHostBuffer().getSampleCount()) &&
            cl::isMultiple2(getB().getHostBuffer().getSampleCount()) &&
            cl::isMultiple2(getH().getHostBuffer().getSampleCount()));

        fff_EXPECT_TRUE(
            getA().getUbiBuffer().isReadable());
        fff_EXPECT_TRUE(
            getB().getUbiBuffer().isReadable());
        fff_EXPECT_TRUE(
            getH().getUbiBuffer().isWritable() &&
            getH().getUbiBuffer().isReadable());

		m_tmp.alloc(
			CL_MEM_READ_WRITE,
            getH().getHostBuffer().getChannelCount(),
			getH().getHostBuffer().getSampleCount());

        fff_EXPECT_VALID_OBJ(getTmp());

		fff_RTCLC_ERR_INIT();

        UInt param = 0;

		param++; // b real
        param++; // b imag
        param++; // a real
        param++; // a imag
        param++; // tmp real
        param++; // tmp imag
        param++; // H real
        param++; // H imag

        UInt lb2N = cl::lb2Multiple2In(getH().getHostBuffer().getSampleCount());
        m_lb2W = smartLb2WorkerLimit(lb2N-1);

        param = dontOptimizeArgs(
            lb2N,
            m_lb2W,
            0,
            param);
        
        fff_EXPECT(
            param,
            ==,
            argsCount());
	}

	void invoke()
	{
        fff_RTCLC_ERR_INIT();
		for(
			UInt channel = 0;
			channel < getB().getHostBuffer().getChannelCount();
			++channel)
		{
            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    0, getB().getUbiBuffer().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    1, getB().getUbiBuffer().getChannel(channel).getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    2, getA().getUbiBuffer().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    3, getA().getUbiBuffer().getChannel(channel).getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    4, getTmp().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    5, getTmp().getChannel(channel).getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    6, getH().getUbiBuffer().getChannel(channel).getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    7, getH().getUbiBuffer().getChannel(channel).getImag()));

			getB().getUbiBuffer().enqueueDeviceUpdate(
				channel);
			getA().getUbiBuffer().enqueueDeviceUpdate(
				channel);

			enqueueNDRange(
                fff_POW2(m_lb2W),
                fff_POW2(m_lb2W));

			getH().getUbiBuffer().enqueueHostUpdate(
				channel);
		}
	}

	IUbiMultiChannel<SampleType> &getH()
	{
		fff_EXPECT_VALID_OBJ_RET(m_H);
	}

	const IUbiMultiChannel<SampleType> &getH() const
	{
		fff_EXPECT_VALID_OBJ_RET(m_H);
	}

	const IUbiMultiChannel<SampleType> &getA() const
	{
		fff_EXPECT_VALID_OBJ_RET(m_a);
	}

	const IUbiMultiChannel<SampleType> &getB() const
	{
		fff_EXPECT_VALID_OBJ_RET(m_b);
	}

    bool operator!() const throw()
    {
        return
            !m_b ||
            !m_a ||
            !m_H ||
            !m_tmp ||
            KernelBase::operator!();
    }

private:
    const DevSingleChannel<SampleType> &getTmp() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_tmp);
    }

    DevMultiChannel<SampleType> &getTmp()
    {
        fff_EXPECT_VALID_OBJ_RET(m_tmp);
    }
private:

    UInt m_lb2W;

	const IUbiMultiChannel<SampleType>
		&m_b,
		&m_a;

	IUbiMultiChannel<SampleType>
		&m_H;

	DevMultiChannel<SampleType>
		m_tmp;
};

}
}
}
#endif