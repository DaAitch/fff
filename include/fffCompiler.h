

#ifndef __fffcompiler_h__included__
#	define __fffcompiler_h__included__

#	include "_fffBase.h"
#	include "_fffException.h"
#	include "fffClEnv.h"
#	include "../cl/fffConvolution.h.cl"

	_fff_BEGIN_NAMESPACE

		class Compiler
		{
		public:
			Compiler(
				ClEnv &env,
				String includeDirectory,
				UInt lb2FFtElementsCount,
				UInt kernelElementsCount);
			
			

			cl::Kernel getKernel(
				String kernelName) const;

			cl::Program getProgram() const
			{
				return
					m_program;
			}

			ClEnv &getEnv()
			{
				return
					m_env;
			}

			void build();

			UInt getLb2FftElementsCount() const
			{
				return
					m_lb2FftElementsCount;
			}

			UInt getFftElementsCount() const
			{
				return
					_fff_NTH_BIT(getLb2FftElementsCount());
			}

			UInt getKernelElementsCount() const
			{
				return
					m_kernelElementsCount;
			}

			UInt getStateElementsCount() const
			{
				return
					getKernelElementsCount() - 1;
			}

		protected:
		private:
			cl::Program
				m_program;
			ClEnv
				&m_env;
			String
				m_includeDirectory;
			UInt
				m_lb2FftElementsCount,
				m_kernelElementsCount;
		};
	_fff_END_NAMESPACE

#endif