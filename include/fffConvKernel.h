
#ifndef __fcoclconvkernel_h__included__
#	define __fcoclconvkernel_h__included__

#	include "_fffBase.h"
#	include "fffClEnv.h"
#	include "fffCvHstMChBuf.h"
#	include "fffCvUbiMChBuf.h"
#	include "fffCvDevSChBuf.h"
#	include "fffCompiler.h"
#	include "fffComputingMap.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class ConvKernel
		{
		public:
			typedef
				SampleType MySample;
			typedef
				ConvKernel<MySample> My;
			typedef
				CvUbiMChBuf<MySample> MyUbiMChBuf;
			typedef
				CvDevSChBuf<MySample> MyDevSChBuf;
		public:
			ConvKernel(
				ClEnv &env,
				const Compiler &compiler,
				const ComputingMap &computingMap,
				MyUbiMChBuf &physicalStateBuf,
				const MyUbiMChBuf &physicalNewInBuf,
				const MyUbiMChBuf &spectralTFuncBuf,
				MyUbiMChBuf &physicalOutputBuf
				)
				:
				m_env(
					env),
				m_physicalInputBuf(
					env),
				m_physicalStateBuf(
					physicalStateBuf),
				m_physicalNewInBuf(
					physicalNewInBuf),
				m_spectralTFuncBuf(
					spectralTFuncBuf),
				m_transformExtensionBuf(
					env),
				m_inverseTransformExtensionBuf(
					env),
				m_physicalOutputBuf(
					physicalOutputBuf)
			{

				fff_RTCLC_ERR_INIT();

				if(!physicalNewInBuf.isAllocated())
					fff_THROW("not allocated new in buffer");

				if(!physicalStateBuf.isAllocated())
					fff_THROW("not allocated state buffer");

				if(!spectralTFuncBuf.isAllocated())
					fff_THROW("not allocated tfunc buffer");

				if(!physicalOutputBuf.isAllocated())
					fff_THROW("not allocated output buffer");

				if(physicalNewInBuf.getMultiChannel().getSampleCount() !=
					physicalOutputBuf.getMultiChannel().getSampleCount())
					fff_THROW("different size");

	


				m_kernel = compiler.getKernel(
					fff_STRINGIFY(
						fff_KERNEL_FCONV));

				if(!m_kernel())
					fff_THROW("invalid kernel");

				UInt extensionElementsCount = _fff_calcOverlapSaveSize(
					physicalNewInBuf.getMultiChannel().getSampleCount() +
					physicalStateBuf.getMultiChannel().getSampleCount(),
					computingMap.getLb2FftElementCount(),
					physicalStateBuf.getMultiChannel().getSampleCount() + 1);

				UInt fftCount = 
					_fff_CEILDIV(
						extensionElementsCount,
						_fff_NTH_BIT(computingMap.getLb2FftElementCount()));

				m_globalRange = cl::NDRange(
					fftCount * computingMap.getLocalWorkerCount());
				m_localRange = cl::NDRange(
					computingMap.getLocalWorkerCount());

				m_physicalInputBuf.alloc(
					CL_MEM_READ_ONLY,
					physicalStateBuf.getMultiChannel().getSampleCount() +
					physicalNewInBuf.getMultiChannel().getSampleCount());
				m_transformExtensionBuf.alloc(
					CL_MEM_READ_WRITE,
					extensionElementsCount);
				m_inverseTransformExtensionBuf.alloc(
					CL_MEM_READ_WRITE,
					extensionElementsCount);

				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						0, m_physicalInputBuf.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						1, m_physicalInputBuf.getImag()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						2, m_physicalInputBuf.getSampleCount()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						3, m_spectralTFuncBuf.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						4, m_spectralTFuncBuf.getImag()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						5, m_transformExtensionBuf.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						6, m_transformExtensionBuf.getImag()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						7, m_inverseTransformExtensionBuf.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						8, m_inverseTransformExtensionBuf.getImag()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						9, m_physicalOutputBuf.getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					m_kernel.setArg(
						10, m_physicalOutputBuf.getImag()));
			}

			void invoke()
			{
				fff_RTCLC_ERR_INIT();

				for(
					UInt channel = 0;
					channel < m_physicalStateBuf.getMultiChannel().getChannelCount();
					++channel)
				{
					m_physicalStateBuf.enqueueDeviceUpdate(
						channel);
		
					m_physicalStateBuf.enqueueCopy(
						m_physicalInputBuf,
						0,
						m_physicalStateBuf.getMultiChannel().getSampleCount());

					m_physicalNewInBuf.enqueueDeviceUpdate(
						channel);

					m_physicalNewInBuf.enqueueCopy(
						m_physicalInputBuf,
						m_physicalStateBuf.getMultiChannel().getSampleCount(),
						m_physicalNewInBuf.getMultiChannel().getSampleCount());

					fff_RTCLC_SEQ_CHECK_RET(
						m_env.getQueue().enqueueNDRangeKernel(
							m_kernel,
							cl::NullRange,
							m_globalRange,
							m_localRange));

					m_physicalOutputBuf.enqueueHostUpdate(
						channel);

					m_physicalInputBuf.enqueueCopy(
						m_physicalStateBuf,
						m_physicalInputBuf.getSampleCount() -
						m_physicalStateBuf.getMultiChannel().getSampleCount() + 1,
						0,
						m_physicalStateBuf.getMultiChannel().getSampleCount() - 1);

					m_physicalStateBuf.enqueueHostUpdate(
						channel);
				}
			}

			void wait() const
			{
				fff_RTCLC_ERR_INIT();

				fff_RTCLC_SEQ_CHECK_RET(
					m_env.getQueue().finish());
			}

			void invokeAndWait()
			{
				invoke();
				wait();
			}
		

		private:
		private:

			ClEnv
				&m_env;

			cl::Kernel
				m_kernel;

			MyDevSChBuf
				m_physicalInputBuf,
				m_transformExtensionBuf,
				m_inverseTransformExtensionBuf;

			MyUbiMChBuf
				&m_physicalStateBuf,
				&m_physicalOutputBuf;
			const MyUbiMChBuf
				&m_spectralTFuncBuf,
				&m_physicalNewInBuf;
			

			cl::NDRange
				m_globalRange,
				m_localRange;
		};

	_fff_END_NAMESPACE

#endif