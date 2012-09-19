//---------------------------------------------------------+
// fff/include/Computing/Kernel/Transformation.h
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
//!	\file		Transformation.h
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
				UbiMultiChannel<SampleType> &continuous,
				UbiMultiChannel<SampleType> &spectral,
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

                fff_EXPECT_TRUE(getContinuous().getHost().isAllocated());
                fff_EXPECT_TRUE(getSpectral().getHost().isAllocated());

                // to be implemented
                /*
                fff_EXPECT_TRUE(getCompiler().getWorker().isBlockNOkay(
                    getContinuous().getHostBuffer().getSampleCount()));

                fff_EXPECT_TRUE(getCompiler().getWorker().isBlockNOkay(
                    getSpectral().getHostBuffer().getSampleCount()));
                    */

                fff_EXPECT(
                    getContinuous().getChannelCount(),
                    ==,
                    getSpectral().getChannelCount()
                );

                fff_EXPECT(
                    getContinuous().getSampleLength(),
                    ==,
                    getSpectral().getSampleLength()
                );

                fff_EXPECT_TRUE(
                    cl::isMultiple2(getContinuous().getSampleLength())
                    &&
                    cl::isMultiple2(getSpectral().getSampleLength())
                );


                fff_EXPECT_TRUE(getFrom().getDev().isReadable());
                fff_EXPECT_TRUE(getTo().getDev().isReadable());
                fff_EXPECT_TRUE(getTo().getDev().isWritable());

                fff_EXPECT(
                    getContinuous().getChannelCount(),
                    ==,
                    getSpectral().getChannelCount()
                );

                fff_EXPECT(
                    getContinuous().getSampleLength(),
                    ==,
                    getSpectral().getSampleLength()
                );

				fff_EXPECT_VALID_CLOBJ(getKernel());

                UInt param = 0;

                fff_RTCLC_ERR_INIT();

				param++; // from real
                param++; // from imag
                param++; // to real
                param++; // to imag

                UInt lb2BlockN = cl::lb2Multiple2In(getFrom().getSampleLength());

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
					channel < getContinuous().getChannelCount();
					++channel)
				{
                    fff_RTCLC_SEQ_CHECK_RET(
					    getKernel().setArg(
						    0, getFrom().getDev()[channel].getReal()));
				    fff_RTCLC_SEQ_CHECK_RET(
					    getKernel().setArg(
						    1, getFrom().getDev()[channel].getImag()));

				    fff_RTCLC_SEQ_CHECK_RET(
					    getKernel().setArg(
						    2, getTo().getDev()[channel].getReal()));
				    fff_RTCLC_SEQ_CHECK_RET(
					    getKernel().setArg(
						    3, getTo().getDev()[channel].getImag()));

                    // copy From to Host
                    getFrom().enqueueDeviceUpdate(channel);

                    // to be implemented
                    /*
                    // if optimized
                    if(getCompiler().getWorker().isOptimized())
                    {
                        // we dont need no From, so
                        // copy From -> To
                        getFrom().getDev()[channel].enqueueCopy(
                            getTo().getDev()[channel]);
                    }
                    */

                    enqueueNDRange(
                        fff_POW2(m_lb2W),
                        fff_POW2(m_lb2W)
                        );

					getTo().enqueueHostUpdate(
						channel);
				}
			}

			

            UbiMultiChannel<SampleType> &getSpectral()
            {
                fff_EXPECT_VALID_OBJ_RET(m_spectralBuf);
            }

            const UbiMultiChannel<SampleType> &getSpectral() const
            {
                fff_EXPECT_VALID_OBJ_RET(m_spectralBuf);
            }

            UbiMultiChannel<SampleType> &getContinuous()
            {
                fff_EXPECT_VALID_OBJ_RET(m_continuousBuf);
            }

            const UbiMultiChannel<SampleType> &getContinuous() const
            {
                fff_EXPECT_VALID_OBJ_RET(m_continuousBuf);
            }

		
            Bool isResultSpectral() const
            {
                return
                    m_cont2spec;
            }

            UbiMultiChannel<SampleType> &getFrom()
            {
                return
                    isResultSpectral() ?
                        getContinuous() :
                        getSpectral();
            }

            const UbiMultiChannel<SampleType> &getFrom() const
            {
                return
                    isResultSpectral() ?
                        getContinuous() :
                        getSpectral();
            }

            UbiMultiChannel<SampleType> &getTo()
            {
                return
                    isResultSpectral() ?
                        getSpectral() :
                        getContinuous();
            }

            const UbiMultiChannel<SampleType> &getTo() const
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

			UbiMultiChannel<SampleType>
				&m_spectralBuf,
				&m_continuousBuf;

			Bool
				m_cont2spec;

            UInt m_lb2W;
		};

	_fff_END_NAMESPACE

#endif