
#ifndef __fcocltfunccomputer_h__included__
#	define __fcocltfunccomputer_h__included__
/*
#	include "fcoclConv.h"
#	include "fcoclComputingMap.h"
#	include "fcoclComplexSamplesMultiChannel.h"
#	include "fcoclComplexBuffer.h"

	class fcoclTFuncComputer
	{
	public:
		fcoclTFuncComputer(
			cl::Context &context,
			cl::CommandQueue &commandQueue,
			cl::Kernel kernel,
			const fcoclComplexSamplesMultiChannel &physicalTFunc,
			const fcoclComputingMap &computingMap);

		~fcoclTFuncComputer();

		void wait();
		void compute();

		fcoclComplexSamplesMultiChannel &getSpectralTFunc()
		{
			return m_spectralTFunc;
		}

	protected:
	private:
	private:
		cl::CommandQueue m_commandQueue;
		cl::Kernel m_kernel;

		fcoclComplexSamplesMultiChannel
			m_physicalTFunc,
			m_spectralTFunc;

		fcoclComplexBuffer
			m_physicalTFuncBuf,
			m_spectralTFuncBuf;

		fcoclComputingMap m_computingMap;
	};
	*/
#endif