//---------------------------------------------------------+
// fff/include/fffTransformKernel.h
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
//!	\file		fffTransformKernel.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Computing spectral to continuous samples
//!					or the other way round.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __ffftransformkernel_h__included__
#	define __ffftransformkernel_h__included__

#	include "../../_intern/_base.h"
#	include "../OpenCLEnvironment.h"
#	include "../../Buffer/Complex/Ubiquitous/UbiMultiChannel.h"
#	include "../Compiler.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class Transformation
            :
            public KernelBase
		{
		public:
			typedef
				Transformation<SampleType> My;

		public:
			Transformation(
				const Compiler &compiler,
				IUbiMultiChannel<SampleType> &continuous,
				IUbiMultiChannel<SampleType> &spectral,
				Bool cont2spec = True
				)
				:
                KernelBase(
                    compiler,
                    cont2spec ?
                    fff_STRINGIFY(
							fff_KERNEL_FFT) 
                    :
                    fff_STRINGIFY(
							fff_KERNEL_IFFT)
                    ),
				m_cont2spec(
					cont2spec),
				m_continuousBuf(
					continuous),
				m_spectralBuf(
					spectral)
			{
                fff_EXPECT_VALID_OBJ(compiler);
                fff_EXPECT_VALID_OBJ(continuous);
                fff_EXPECT_VALID_OBJ(spectral);

                fff_EXPECT_TRUE(getContinuous().getHostBuffer().isAllocated());
                fff_EXPECT_TRUE(getSpectral().getHostBuffer().isAllocated());

                fff_EXPECT_TRUE(getCompiler().getWorker().isBlockNOkay(
                    getContinuous().getHostBuffer().getSampleCount()));

                fff_EXPECT_TRUE(getCompiler().getWorker().isBlockNOkay(
                    getSpectral().getHostBuffer().getSampleCount()));

                fff_EXPECT(
                    getContinuous().getHostBuffer().getChannelCount(),
                    ==,
                    getSpectral().getHostBuffer().getChannelCount()
                );

                fff_EXPECT(
                    getContinuous().getHostBuffer().getSampleCount(),
                    ==,
                    getSpectral().getHostBuffer().getSampleCount()
                );

                fff_EXPECT_TRUE(
                    cl::isMultiple2(getContinuous().getHostBuffer().getSampleCount())
                    &&
                    cl::isMultiple2(getSpectral().getHostBuffer().getSampleCount())
                );


                fff_EXPECT_TRUE(getFrom().getUbiBuffer().isReadable());
                fff_EXPECT_TRUE(getTo().getUbiBuffer().isReadable());
                fff_EXPECT_TRUE(getTo().getUbiBuffer().isWritable());

                fff_EXPECT(
                    getContinuous().getHostBuffer().getChannelCount(),
                    ==,
                    getSpectral().getHostBuffer().getChannelCount()
                );

                fff_EXPECT(
                    getContinuous().getHostBuffer().getSampleCount(),
                    ==,
                    getSpectral().getHostBuffer().getSampleCount()
                );

				fff_EXPECT_VALID_CLOBJ(getKernel());

                UInt param = 0;

                fff_RTCLC_ERR_INIT();

                if(!getCompiler().getWorker().isOptimized())
                {
					fff_RTCLC_SEQ_CHECK_RET(
						getKernel().setArg(
							param++, getFrom().getUbiBuffer().getReal()));
					fff_RTCLC_SEQ_CHECK_RET(
						getKernel().setArg(
							param++, getFrom().getUbiBuffer().getImag()));
                }

				fff_RTCLC_SEQ_CHECK_RET(
					getKernel().setArg(
						param++, getTo().getUbiBuffer().getReal()));
				fff_RTCLC_SEQ_CHECK_RET(
					getKernel().setArg(
						param++, getTo().getUbiBuffer().getImag()));

                UInt lb2BlockN = cl::lb2Multiple2In(getFrom().getHostBuffer().getSampleCount());

                m_lb2W = smartLb2WorkerLimit(lb2BlockN-1);

                param = dontOptimizeArgs(
                    lb2BlockN,
                    m_lb2W,
                    0,
                    param);

                fff_EXPECT(param, ==, argsCount());

			}

			void invoke()
			{
				fff_RTCLC_ERR_INIT();

				for(
					UInt channel = 0;
					channel < getContinuous().getHostBuffer().getChannelCount();
					++channel)
				{
                    // copy From to Host
                    getFrom().getUbiBuffer().enqueueDeviceUpdate(channel);

                    // if optimized
                    if(getCompiler().getWorker().isOptimized())
                    {
                        // we dont need no From, so
                        // copy From -> To
                        getFrom().getUbiBuffer().enqueueCopy(
                            getTo().getUbiBuffer());
                    }

                    enqueueNDRange(
                        fff_POW2(m_lb2W),
                        fff_POW2(m_lb2W)
                        );

					getTo().getUbiBuffer().enqueueHostUpdate(
						channel);
				}
			}

			

            IUbiMultiChannel<SampleType> &getSpectral()
            {
                fff_EXPECT_VALID_OBJ_RET(m_spectralBuf);
            }

            const IUbiMultiChannel<SampleType> &getSpectral() const
            {
                fff_EXPECT_VALID_OBJ_RET(m_spectralBuf);
            }

            IUbiMultiChannel<SampleType> &getContinuous()
            {
                fff_EXPECT_VALID_OBJ_RET(m_continuousBuf);
            }

            const IUbiMultiChannel<SampleType> &getContinuous() const
            {
                fff_EXPECT_VALID_OBJ_RET(m_continuousBuf);
            }

		
            Bool isResultSpectral() const
            {
                return
                    m_cont2spec;
            }

            IUbiMultiChannel<SampleType> &getFrom()
            {
                return
                    isResultSpectral() ?
                        getContinuous() :
                        getSpectral();
            }

            const IUbiMultiChannel<SampleType> &getFrom() const
            {
                return
                    isResultSpectral() ?
                        getContinuous() :
                        getSpectral();
            }

            IUbiMultiChannel<SampleType> &getTo()
            {
                return
                    isResultSpectral() ?
                        getSpectral() :
                        getContinuous();
            }

            const IUbiMultiChannel<SampleType> &getTo() const
            {
                return
                    isResultSpectral() ?
                        getSpectral() :
                        getContinuous();
            }

            bool operator!() const throw()
            {
                return
                    !m_spectralBuf ||
                    !m_continuousBuf ||
                    KernelBase::operator!();
            }

		private:

			IUbiMultiChannel<SampleType>
				&m_spectralBuf,
				&m_continuousBuf;

			Bool
				m_cont2spec;

            UInt m_lb2W;
		};

	_fff_END_NAMESPACE

#endif