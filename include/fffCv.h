//---------------------------------------------------------+
// fff/include/fffCv.h
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
//!	\file		fffCv.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Complex vector.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffcv_h__included__
#	define __fffcv_h__included__

#	include "_fffBase.h"
#	include "fffV.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class Cv
		{
		public:
			typedef
				SampleType MySample;
			typedef
				Cv<MySample> My;
			typedef
				V<MySample> MySamples;
			typedef
				MySample Gain;

		public:
			Cv(
				UInt sampleCount=0)
				:
				m_sampleCount(
					0)
			{
				alloc(
					sampleCount);

				init();
			}

			_fff_DEVELHOST_IMPL_DEFAULT_COPYCTOR()
			_fff_DEVELHOST_IMPL_DEFAULT_ASSIGNOP()


			void init()
			{
				m_re.init();
				m_im.init();
			}

			void dealloc()
			{
				m_re.dealloc();
				m_im.dealloc();

				m_sampleCount = 0;
			}

			void alloc(UInt sampleCount)
			{
				dealloc();

				m_re.alloc(sampleCount);
				m_im.alloc(sampleCount);

				m_sampleCount = sampleCount;

				init();
			}

			Bool isAllocated() const
			{
				return
					m_re.isAllocated() &&
					m_im.isAllocated();
			}

			Bool operator!() const
			{
				return
					m_re.getLength() != getLength() ||
					m_im.getLength() != getLength();
			}

			MySamples &getReal()
			{
				return m_re;
			}

			const MySamples &getReal() const
			{
				return m_re;
			}

			MySamples &getImag()
			{
				return m_im;
			}

			const MySamples &getImag() const
			{
				return m_im;
			}

			UInt getSampleCount() const
			{
				return m_sampleCount;
			}

			MySample calcSpectralRMS() const
			{
				MySample rms = (MySample)0;

				for(
					UInt i = 0;
					i < getSampleCount();
					++i)
				{
					MySample
						re = getReal()[i],
						im = getImag()[i];

					// sqrt(|X|)*sqrt(|X|) == 
					rms += re*re+im*im;
				}

				return sqrt(rms) / (MySample)getSampleCount();
			}

			MySample calcMaxGain() const
			{
				MySample gain = (MySample)0.f;

				for(
					UInt i = 0;
					i < getSampleCount();
					++i)
				{
					MySample re, im;
					re = getReal()[i];
					im = getImag()[i];
					gain = max(gain, re*re+im*im);
				}

				return sqrt(gain);
			}

			My &operator*=(
				MySample factor)
			{
				getReal() *= factor;
				getImag() *= factor;

				return *this;
			}

			My &operator/=(
				MySample divider)
			{
				getReal() /= divider;
				getImag() /= divider;

				return *this;
			}


		private:
			MySamples
				m_re,
				m_im;
			UInt
				m_sampleCount;
		};

	_fff_END_NAMESPACE
#endif