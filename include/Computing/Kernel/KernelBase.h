//---------------------------------------------------------+
// fff/include/Computing/Kernel/KernelBase.h
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
//!	\file		KernelBase.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Multi channel complex vector host buffer,
//!				extern allocated.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __kernelbase_h__included__
#	define __kernelbase_h__included__

#	include "../../_intern.h"
#	include "../OpenCLEnvironment.h"
#	include "../Compiler.h"
#include "../RuntimeMapping/DeviceProperties.h"

using namespace fff::_Intern::Exceptions::RtCLC;
using namespace fff::Computing::RuntimeMapping;

namespace fff {
namespace Computing {
namespace Kernel {

// NDRange cases [global] [local]
// - concurrent:     N      NULL  (unknown amount of groups, though no barriers allowed!)
// - one group:      N      W=N   (1 group)
// - more groups:  W*g=N     W    (g Groups)

// Always limited by the max worker group value!
// algorithm to assign:
//
// input N, g, maxW:
// W=0
// if g>0 then
//  W = N/g
//  assert(N%g==0)
//  while(W > maxW)
//   W=(W+1) / 2   // divide by two, rounding up
//
//  N = g*W
//  run NDRange(N, W)
// else
//  run NDRange(N, NULL)
// endif
//
//
// Additional there are algorithms, like fft
// where we dont need N, but N/2 worker, since
// there are N/2 butterflies with same complex
// coefficients, though introducing parameter
// "lb2ItemsPerWorker".


		class KernelBase
            :
            public iprintable
		{
		public:
			KernelBase(
				const Compiler &compiler,
				const String &kernelName);

            ~KernelBase() throw()
            {
            }

        protected:
			UInt argsCount() const;

            void enqueueNDRange(
                UInt global,
                UInt local);

			UInt dontOptimizeArgs(
                UInt lb2BlockN,
                UInt lb2W,
                UInt M,
				UInt startArgIndex);

            UInt smartWorkerLimit(UInt items) const;
            UInt smartLb2WorkerLimit(UInt lb2Items) const;

            

        public:
            virtual void invoke() = 0;

            void wait() const
			{
				fff_RTCLC_ERR_INIT();

				fff_RTCLC_SEQ_CHECK_RET(
					getCompiler().getEnv().getQueue().finish());
			}

			void invokeAndWait()
			{
				invoke();
				wait();
			}

			const Compiler &getCompiler() const
			{
                fff_EXPECT_VALID_OBJ_RET(m_compiler);
			}

			::cl::Kernel &getKernel()
			{
                fff_EXPECT_VALID_CLOBJ_RET(m_kernel);
			}

            const ::cl::Kernel &getKernel() const
			{
                fff_EXPECT_VALID_CLOBJ_RET(m_kernel);
			}

            String getKernelName() const
            {
                return
                    m_kernelName;
            }

            const DeviceProperties &getDeviceProperties() const
            {
                fff_EXPECT_VALID_OBJ_RET(m_devprops);
            }

            bool operator!() const throw()
            {
                return !m_compiler ||
                    m_kernel() == 0;
            }

            virtual void print(logstream &out) const
            {
                out("KernelBase");
                !fff_LOG_VAL(out, getKernelName());
                !fff_LOG_VAL(out, getCompiler());
                !fff_LOG_VAL(out, argsCount());
                out();
            }

		private:

            void _enqueueNDRange(
                UInt global,
                UInt local);

			const Compiler 
				&m_compiler;

			::cl::Kernel
				m_kernel;

            String
                m_kernelName;

            DeviceProperties
                m_devprops;
		};

}
}
}

#endif