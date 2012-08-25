//---------------------------------------------------------+
// fff/include/fffCvHstMChBuf.h
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
//!	\file		fffCvHstMChBuf.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Multi channel complex vector host buffer.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __hostmultichannel_h__included__
#define __hostmultichannel_h__included__

#	include "../../../_intern/_base.h"
#	include "HostSingleChannel.h"
#	include "IHostMultiChannel.h"
#	include "../ComplexVector.h"


namespace fff {
namespace Buffer {
namespace Complex {
namespace Host {

template<
	class SampleType
>
class HostMultiChannel
	: public IHostMultiChannel<SampleType>
{
public:
	typedef
		SampleType	MySample;
	typedef
		HostMultiChannel<MySample> My;
	typedef
		HostSingleChannel<MySample> MySamples;
	typedef
		std::vector<MySamples> MyChannels;
		

public:
	HostMultiChannel()
		:
		m_sampleCount(0),
		m_channelSamples(0)
	{
		fff_EXPECT_FALSE(isAllocated());
	}

	HostMultiChannel(
		UInt channelCount,
		UInt sampleCount)
		:
        m_sampleCount(0),
		m_channelSamples(0)
	{
		fff_EXPECT_FALSE(isAllocated());

		alloc(
			channelCount,
			sampleCount);
	}

	virtual ~HostMultiChannel()
	{
		dealloc();
	}
		
	void alloc(
		UInt channelCount,
		UInt sampleCount)
	{
		dealloc();

		fff_EXPECT_FALSE(isAllocated());

		if(channelCount > 0 && sampleCount > 0)
		{
			_alloc(
				channelCount,
				sampleCount);

			fff_EXPECT_TRUE(isAllocated());
		}
	}

	void dealloc()
	{
		if(isAllocated())
			_dealloc();

		fff_EXPECT_FALSE(isAllocated());
	}


	void init()
	{
		fff_EXPECT_TRUE(isAllocated());

		_init();

		fff_EXPECT_TRUE(isAllocated());
	}

	Bool isAllocated() const throw()
	{
		return
            _isAllocated();
	}

	const MySamples &operator[](
		UInt channelIndex) const
	{
        fff_EXPECT(channelIndex, <, getChannelCount());
		return
			m_channelSamples[channelIndex];
	}

	MySamples &operator[](
		UInt channelIndex)
	{
        fff_EXPECT(channelIndex, <, getChannelCount());
		return
			m_channelSamples[channelIndex];
	}
			

	UInt getChannelCount() const
	{
		return
			m_channelSamples.size();
	}

	UInt getSampleCount() const
	{
		return
			m_sampleCount;
	}

	MySample *getRawReal(
		UInt channelIndex)
	{
        fff_EXPECT(channelIndex, <, getChannelCount());
		return
			fff_ME[channelIndex].getReal().getSamples();
	}

	MySample *getRawImag(
		UInt channelIndex)
	{
        fff_EXPECT(channelIndex, <, getChannelCount());
		return
			fff_ME[channelIndex].getImag().getSamples();
	}

	const MySample *getRawReal(
		UInt channelIndex) const
	{
        fff_EXPECT(channelIndex, <, getChannelCount());
		return
			fff_ME[channelIndex].getReal().getSamples();
	}

	const MySample *getRawImag(
		UInt channelIndex) const
	{
        fff_EXPECT(channelIndex, <, getChannelCount());
		return
			fff_ME[channelIndex].getImag().getSamples();
	}

    My &operator+=(
        const My &rhs)
    {
        fff_EXPECT(
            rhs.getChannelCount(),
            ==,
            getChannelCount());

        for(
            UInt channel = 0;
            channel < getChannelCount();
            ++channel)
        {
            fff_ME[channel] += rhs[channel];
        }

        return
            fff_ME;
    }

    My &operator-=(
        const My &rhs)
    {
        fff_EXPECT(
            rhs.getChannelCount(),
            ==,
            getChannelCount());

        for(
            UInt channel = 0;
            channel < getChannelCount();
            ++channel)
        {
            fff_ME[channel] -= rhs[channel];
        }

        return
            fff_ME;
    }
    
	My &operator*=(
		MySample factor)
	{
		for(
			UInt channel = 0;
			channel < getChannelCount();
			++channel)
		{
			fff_ME[channel] *= factor;
		}

		return fff_ME;
	}
    
    
	My &operator/=(
		MySample factor)
	{
		for(
			UInt channel = 0;
			channel < getChannelCount();
			++channel)
		{
			fff_ME[channel] /= factor;
		}

		return fff_ME;
	}
    
    
	void normalize()
	{

		MySample maxGain = 0.f;
				
		for(
			UInt channel = 0;
			channel < getChannelCount();
			++channel)
		{
			maxGain = max(
				maxGain, fff_ME[channel].calcMaxGain());
		}

		fff_ME /= maxGain;
	}
    

    virtual void print(logstream &out) const
    {
        out("HostMultiChannel");
        !fff_LOG_VAL(out, getSampleCount());
        !fff_LOG_VAL(out, getChannelCount());
        out();
    }

    bool operator!() const throw()
    {
        return
            m_channelSamples.size() == 0 ||
            m_sampleCount == 0;
    }



private:

		

	void _alloc(
		UInt channelCount,
		UInt sampleCount)
	{
		fff_EXPECT_FALSE(isAllocated());

		fff_EXPECT(channelCount, >, 0);
        fff_EXPECT(sampleCount, >, 0);

		m_sampleCount = sampleCount;
		m_channelSamples.resize(channelCount);

		for(
			UInt channel = 0;
			channel < getChannelCount();
			++channel)
		{
			m_channelSamples[channel] = 
				HostSingleChannel<MySample>(sampleCount);
		}

		fff_EXPECT_TRUE(isAllocated());
	}

	void _init()
	{
		fff_EXPECT_TRUE(isAllocated());

		for(
			UInt channel = 0;
			channel < getChannelCount();
			++channel)
		{
			m_channelSamples[channel].init();
		}
	}

	void _dealloc()
	{
		fff_EXPECT_TRUE(isAllocated());

		for(
			UInt channel = 0;
			channel < getChannelCount();
			++channel)
		{
			m_channelSamples[channel].dealloc();
		}

		m_channelSamples.clear();
		m_sampleCount = 0;

		fff_EXPECT_FALSE(isAllocated());
	}

	Bool _isAllocated() const throw()
	{
		return
			m_channelSamples.size() != 0;
	}

private:

	MyChannels
		m_channelSamples;
	UInt
		m_sampleCount;
};

	}
	}
	}
	}

#endif