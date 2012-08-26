//---------------------------------------------------------+
// fff/include/fffCvHstMChBufPreallocated.h
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
//!	\file		fffCvHstMChBufPreallocated.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Multi channel complex vector host buffer,
//!				extern allocated.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffcvhstmchbufpreallocated_h__included__
#	define __fffcvhstmchbufpreallocated_h__included__

#	include "../../../_intern/_base.h"
#	include "IHostMultiChannel.h"
#	include "../../Vector.h"
#include <vector>

namespace fff {
namespace Buffer {
namespace Complex {
namespace Host {

	template<
		class SampleType
	>
	class HostMultiChannelPreallocated :
		public IHostMultiChannel<SampleType>
	{
	public:
		typedef
			SampleType MySample;
		typedef
			Vector<SampleType> MySamples;
		typedef
			std::vector<MySamples> MyMChSamples;
		typedef
			HostMultiChannelPreallocated<MySample> My;

	public:
        HostMultiChannelPreallocated(
            UInt channelCount=0,
            UInt sampleCount=0)
		:
		m_channelCount(0),
		m_sampleCount(0),
		m_channelSamples(NULL)
		{
			alloc(
                channelCount,
                sampleCount);
		}
		
		void alloc(
			UInt channelCount,
			UInt sampleCount)
		{
			dealloc();

			if(channelCount > 0 && sampleCount > 0)
			{

				_alloc(
					channelCount,
					sampleCount);

				fff_EXPECT_TRUE(
				    isAllocated());

				initImag();

				fff_EXPECT_TRUE(
				    isAllocated());
			}
		}

		void dealloc()
		{
			if(isAllocated())
				_dealloc();

		}

		void initImag()
		{
			fff_EXPECT_TRUE(
				isAllocated());

			_initImag();
		}

        virtual void init()
        {
            fff_EXPECT_VALID_THIS();

            for(UInt channel = 0;
                channel < getChannelCount();
                ++channel)
            {
                for(UInt sample = 0;
                    sample < getSampleCount();
                    ++sample)
                {
                    getRawReal(channel)[sample] = (SampleType)0;
                    getRawImag(channel)[sample] = (SampleType)0;
                }
            }
        }

		Bool isAllocated() const throw()
		{
			return
                _isAllocated();
		}
			

		UInt getChannelCount() const
		{
			return m_channelCount;
		}

		UInt getSampleCount() const
		{
			return m_sampleCount;
		}

        UInt getImagSampleCount() const
        {
            return m_imagSampleCount;
        }

		void setPointer(
			MySample **real)
		{
			m_channelSamples = real;
		}

		void clearPointer()
		{
			m_channelSamples = NULL;
		}

		const MySample *getRawReal(
			UInt channel) const
		{
            fff_EXPECT(channel, <, getChannelCount());
			return m_channelSamples[channel];
		}

		const SampleType *getRawImag(
			UInt channel) const
		{
            fff_EXPECT(channel, <, getChannelCount());
			return m_imagSamples[channel].getSamples();
		}

		MySample *getRawReal(
			UInt channel)
		{
            fff_EXPECT(channel, <, getChannelCount());
			return m_channelSamples[channel];
		}

		MySample *getRawImag(
			UInt channel)
		{
            fff_EXPECT(channel, <, getChannelCount());
			return m_imagSamples[channel].getSamples();
		}

        virtual void print(logstream &out) const
        {
            out("HostMultiChannelPreallocated");
            !fff_LOG_VAL(out, getSampleCount());
            !fff_LOG_VAL(out, getChannelCount());
            out();
        }

        virtual bool operator!() const throw()
        {
            return
                false;
        }

	private:

		void _alloc(
			UInt channelCount,
			UInt sampleCount)
		{
			fff_EXPECT(channelCount, >, 0);
            fff_EXPECT(sampleCount, >, 0);

			m_channelCount = channelCount;
			m_sampleCount = sampleCount;

			m_imagSamples.resize(
				getChannelCount());
		
			for(
				UInt channel = 0;
				channel < getChannelCount();
				++channel)
			{
				m_imagSamples[channel] = MySamples(getSampleCount());
			}

			fff_EXPECT_TRUE(isAllocated());
		}

		void _initImag()
		{
			fff_EXPECT_TRUE(isAllocated());

			for(
				UInt channel = 0;
				channel < getChannelCount();
				++channel)
			{
				m_imagSamples[channel].init();
			}
		}


		void _dealloc()
		{
			fff_EXPECT_TRUE(isAllocated());

			m_imagSamples.clear();
	
			m_channelCount = 0;
			m_sampleCount = 0;

			fff_EXPECT_FALSE(isAllocated());
		}

		Bool _isAllocated() const throw()
		{
			return
				m_imagSamples.size()>0;
		}

	private:

		MySample	**m_channelSamples;
		MyMChSamples m_imagSamples;

		UInt
			m_channelCount,
			m_sampleCount;
	};
}
}
}
}


#endif