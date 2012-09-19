//---------------------------------------------------------+
// fff/include/Computing/Kernel/TransferFunction.h
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
//!	\file		TransferFunction.h
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
		const UbiMultiChannel<SampleType> &b,
		const UbiMultiChannel<SampleType> &a,
		UbiMultiChannel<SampleType> &H)
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
            getB().getChannelCount(),
            ==,
            getA().getChannelCount());

        fff_EXPECT(
            getB().getChannelCount(),
            ==,
            getH().getChannelCount());

        fff_EXPECT(
            getA().getSampleLength(),
            ==,
            getB().getSampleLength());

        fff_EXPECT(
            getA().getSampleLength(),
            ==,
            getH().getSampleLength());

        // to be implemented
        /*
        fff_EXPECT_TRUE(
            getCompiler().getWorker().isBlockNOkay(
                getA().getTransferSampleCount()));
                */
        fff_EXPECT_TRUE(
            cl::isMultiple2(getA().getSampleLength()) &&
            cl::isMultiple2(getB().getSampleLength()) &&
            cl::isMultiple2(getH().getSampleLength()));

        fff_EXPECT_TRUE(
            getA().getDev().isReadable());
        fff_EXPECT_TRUE(
            getB().getDev().isReadable());
        fff_EXPECT_TRUE(
            getH().getDev().isWritable() &&
            getH().getDev().isReadable());

		m_tmp.alloc(
			CL_MEM_READ_WRITE,
            getH().getChannelCount(),
			getH().getSampleLength());

        fff_EXPECT_VALID_OBJ(getTmp());

        UInt param = 0;

		param++; // b real
        param++; // b imag
        param++; // a real
        param++; // a imag
        param++; // tmp real
        param++; // tmp imag
        param++; // H real
        param++; // H imag

        UInt lb2N = cl::lb2Multiple2In(getH().getSampleLength());
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
			channel < getB().getChannelCount();
			++channel)
		{
            fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    0, getB().getDev()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    1, getB().getDev()[channel].getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    2, getA().getDev()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    3, getA().getDev()[channel].getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    4, getTmp()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    5, getTmp()[channel].getImag()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    6, getH().getDev()[channel].getReal()));
		    fff_RTCLC_SEQ_CHECK_RET(
			    getKernel().setArg(
				    7, getH().getDev()[channel].getImag()));

			getB().enqueueDeviceUpdate(
				channel);
			getA().enqueueDeviceUpdate(
				channel);

			enqueueNDRange(
                fff_POW2(m_lb2W),
                fff_POW2(m_lb2W));

			getH().enqueueHostUpdate(
				channel);
		}
	}

	UbiMultiChannel<SampleType> &getH()
	{
		fff_EXPECT_VALID_OBJ_RET(m_H);
	}

	const UbiMultiChannel<SampleType> &getH() const
	{
		fff_EXPECT_VALID_OBJ_RET(m_H);
	}

	const UbiMultiChannel<SampleType> &getA() const
	{
		fff_EXPECT_VALID_OBJ_RET(m_a);
	}

	const UbiMultiChannel<SampleType> &getB() const
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

	const UbiMultiChannel<SampleType>
		&m_b,
		&m_a;

	UbiMultiChannel<SampleType>
		&m_H;

	DevMultiChannel<SampleType>
		m_tmp;
};

}
}
}
#endif