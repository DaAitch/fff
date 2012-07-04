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
#	define __ffftfunckernel_h__incldued__

#	include "_fffBase.h"
#	include "fffClEnv.h"
#	include "fffCompiler.h"
#	include "fffComputingMap.h"
#	include "fffCvUbiMChBuf.h"
#	include "fffCvDevSChBuf.h"
#	include "fffCompiler.h"
#	include "fffComputingMap.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class TFuncKernel
		{
		public:
			typedef
				SampleType MySample;
			typedef
				CvUbiMChBuf<MySample> MyUbiMChBuf;
			typedef
				CvDevSChBuf<MySample> MyDevSChBuf;
		public:

			TFuncKernel(
				ClEnv &env,
				const Compiler &compiler,
				const ComputingMap &computingMap,
				const MyUbiMChBuf &b,
				const MyUbiMChBuf &a,
				MyUbiMChBuf &H)
				:
				m_env(
					env),
				m_b(
					b),
				m_a(
					a),
				m_tmp(
					env),
				m_H(
					H),
				m_computingMap(
					computingMap)
			{
				if(!b.isAllocated())
					fff_THROW("not allocated zeros");
				if(!a.isAllocated())
					fff_THROW("not allocated poles");
				if(!H.isAllocated())
					fff_THROW("not allocated spectral transfer function");
	
				if((a.getMultiChannel().getChannelCount() !=
					b.getMultiChannel().getChannelCount()) ||
					(a.getMultiChannel().getChannelCount() !=
					H.getMultiChannel().getChannelCount()))
					fff_THROW("different channel count");

				if(b.getMultiChannel().getSampleCount() != compiler.getKernelElementsCount())
					fff_THROW("different kernel size");

				if(H.getMultiChannel().getSampleCount() != compiler.getFftElementsCount())
					fff_THROW("different fft size");

				m_tmp.alloc(
					CL_MEM_READ_WRITE,
					m_H.getMultiChannel().getSampleCount());

				assert(
					m_b.isAllocated() &&
					m_a.isAllocated() &&
					m_H.isAllocated() &&
					m_tmp.isAllocated());


				fff_RTCLC_ERR_INIT();

				m_kernel = compiler.getKernel(
					fff_STRINGIFY(
						fff_KERNEL_TFUNC));

				if(!m_kernel())
					fff_THROW("invalid kernel");

				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						0, m_b.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						1, m_b.getImag()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						2, m_a.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						3, m_a.getImag()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						4, m_tmp.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						5, m_tmp.getImag()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						6, m_H.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						7, m_H.getImag()));
			}








			void invoke()
			{
				fff_RTCLC_ERR_INIT();

				assert(
					m_kernel());

				if(!m_b.isAllocated())
					fff_THROW("zeros not allocated");

				if(!m_a.isAllocated())
					fff_THROW("poles not allocated");

				if(!m_H.isAllocated())
					fff_THROW("spectral output not allocated");

				assert(
					m_tmp.isAllocated());

				for(
					UInt channel = 0;
					channel < m_b.getMultiChannel().getChannelCount();
					++channel)
				{
					m_b.enqueueDeviceUpdate(
						channel);
					m_a.enqueueDeviceUpdate(
						channel);

					fff_RTCLC_SEQ_CHECK_RET(
						m_env.getQueue().enqueueNDRangeKernel(
							m_kernel,
							cl::NullRange,
							cl::NDRange(m_computingMap.getLocalWorkerCount()),
							cl::NDRange(m_computingMap.getLocalWorkerCount())));

					m_H.enqueueHostUpdate(
						channel);
				}
			}

			void invokeAndWait()
			{
				invoke();
				wait();
			}

			void wait() const
			{
				fff_RTCLC_ERR_INIT();

				fff_RTCLC_SEQ_CHECK_RET(
					m_env.getQueue().finish());
			}

			MyUbiMChBuf &getH()
			{
				return m_H;
			}

			const MyUbiMChBuf &getH() const
			{
				return m_H;
			}

			const MyUbiMChBuf &getA() const
			{
				return m_a;
			}

			const MyUbiMChBuf &getB() const
			{
				return m_b;
			}

		private:
		private:
			ClEnv
				&m_env;

			cl::Kernel
				m_kernel;

			const MyUbiMChBuf
				&m_b,
				&m_a;
			MyUbiMChBuf
				&m_H;
			MyDevSChBuf
				m_tmp;

			ComputingMap
				m_computingMap;
		};

	_fff_END_NAMESPACE
#endif