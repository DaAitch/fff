//---------------------------------------------------------+
// fff/include/fffComputingMap.h
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
//!	\file		fffComputingMap.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Runtime problem-solution mapping of given
//!				information to allow computation without
//!				overflows, but best usage of resources.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+
#ifndef __fffcomputingmap_h__included__
#	define __fffcomputingmap_h__included__

#	include "_fffBase.h"
#	include "fffComputingData.h"
#	include "fffDevProps.h"

	_fff_BEGIN_NAMESPACE


		/* mapping of the data according
			to the device properties */
		class ComputingMap
		{
		public:
			template<
				class SampleType
			>
			ComputingMap(
				ComputingData<SampleType> &computingData,
				DevProps &deviceProperties,
				UInt throughputFactor
				)
			{
				// computing strategy doesnt know anything about the input size

				// N = T*M
				// L = N   - 2*(M-1)
				// L = T*M - 2*(M-1)
				// L = T*M - 2M-2
				// L = M(T-2) - 2

				m_lb2FftElementCount = _fff_nextLb2Step(computingData.getKernelElementsCount()) + throughputFactor;
				m_localWorkerCount = _fff_NTH_BIT((ULong)getLb2FftElementCount()-1); // N/2 worker <-> each butterfly, one worker

				if(m_localWorkerCount > deviceProperties.getMaxWorkerGroupCount())
					m_localWorkerCount = deviceProperties.getMaxWorkerGroupCount();
			}

			UInt getLb2FftElementCount() const
			{
				return
					m_lb2FftElementCount;
			}

			ULong getLocalWorkerCount() const
			{
				return
					m_localWorkerCount;
			}

		private:
			UInt
				m_lb2FftElementCount;		// N
											/* Size of the fft convolution.
												Also defines size of input data
												to use (equals throughput of
												that iteration) and the size
												of preceding samples (eqals
												state of the fir) */
			ULong
				m_localWorkerCount;


		};


	_fff_END_NAMESPACE

#endif /* ifndef __fcoclcomputingmap_h__included__ */