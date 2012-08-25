//---------------------------------------------------------+
// fff/include/fffV.h
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
//!	\file		fffV.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Linear memory vector to can be copied at
//!				once, no mapping or facade.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __vector_h__included__
#define __vector_h__included__

#include "../_intern.h"

namespace fff {
namespace Buffer {

template<
	class SampleType
>
class Vector
    :
    public iprintable
{
public:
	typedef
		SampleType MySample;
	typedef
		Vector<MySample> My;

public:
	Vector(
		UInt sampleCount=0)
		:
		m_samples(NULL),
		m_sampleCount(0)
	{
        fff_EXPECT_FALSE(isAllocated());
		alloc(
			sampleCount);
	}

	Vector(
		const My &rhs)
		:
		m_samples(NULL),
		m_sampleCount(0)
	{
		copy(
			rhs);
	}

	Vector(
		const My &rhs,
		UInt newSize)
		:
		m_samples(NULL),
		m_sampleCount(0)
	{
		copy(
			rhs,
			newSize);
	}

	virtual ~Vector()
	{
		dealloc();
	}

	My &operator=(
		const My &rhs)
	{
		copy(
			rhs);
		return fff_ME;
	}

	void copy(
		const My &rhs)
	{
		alloc(
			rhs.getSampleCount());

		for(
			UInt i = 0;
			i < rhs.getSampleCount();
			++i)
		{
			m_samples[i] = rhs[i];
		}
	}

	void copy(
		const My &rhs,
		UInt newSize)
	{
		alloc(
			newSize);

		for(
			UInt i = 0;
			i < rhs.getSampleCount() && i < newSize;
			++i)
		{
			m_samples[i] = rhs[i];
		}
	}

	const MySample &operator[](
		UInt sampleIndex) const
	{
        fff_EXPECT_TRUE(isAllocated());
        fff_EXPECT(sampleIndex, <, getSampleCount());

		return
			m_samples[sampleIndex];
	}
	MySample &operator[](
		UInt sampleIndex)
	{
        fff_EXPECT_TRUE(isAllocated());
		fff_EXPECT(sampleIndex, <, getSampleCount());

		return
			m_samples[sampleIndex];
	}

	UInt getSampleCount() const
	{
		return
			m_sampleCount;
	}

	void alloc(UInt sampleCount)
	{
		dealloc();

		if(sampleCount > 0)
			_alloc(sampleCount);

		init();
	}

	void dealloc()
	{
		if(isAllocated())
			_dealloc();

		fff_EXPECT_FALSE(
			isAllocated());
	}

	void init()
	{
		for(
			UInt i = 0;
			i < getSampleCount();
			++i)
		{
			m_samples[i] = (MySample)0;
		}
	}

	Bool isAllocated() const
	{
		return m_samples != NULL;
	}

	My &operator*=(
		MySample factor)
	{
		for(
			UInt i = 0;
			i < getSampleCount();
			++i)
		{
			m_samples[i] *= factor;
		}

		return
			fff_ME;
	}

	My &operator/=(
		MySample divider)
	{
		for(
			UInt i = 0;
			i < getSampleCount();
			++i)
		{
			m_samples[i] /= divider;
		}

		return
			fff_ME;
	}

    My &operator+=(
        const My &rhs)
    {
        fff_EXPECT(
            getSampleCount(),
            ==,
            rhs.getSampleCount());

        for(
			UInt i = 0;
			i < getSampleCount();
			++i)
		{
			fff_ME[i] += rhs[i];
		}

		return
			fff_ME;
    }

    My &operator-=(
        const My &rhs)
    {
        fff_EXPECT(
            getSampleCount(),
            ==,
            rhs.getSampleCount());

        for(
			UInt i = 0;
			i < getSampleCount();
			++i)
		{
			fff_ME[i] -= rhs[i];
		}

		return
			fff_ME;
    }

	MySample calcContinuousRMS() const
	{
		MySample rms = 0.f;

		for(
			UInt i = 0;
			i < getSampleCount();
			++i)
		{
			rms += m_samples[i] * m_samples[i];
		}

		return sqrt(rms / (MySample)getSampleCount());
	}

	MySample *getSamples()
	{
		return
			m_samples;
	}

	const MySample *getSamples() const
	{
		return
			m_samples;
	}

    bool operator!() const throw()
    {
        return
            m_sampleCount == 0 ||
            m_samples == NULL;
    }

    virtual void print(logstream &out) const
    {
        out("Vector");
        !fff_LOG_VAL(out, getSampleCount());
        out();
    }

private:

	void _alloc(UInt sampleCount)
	{
		fff_EXPECT_FALSE(
			isAllocated());

		m_samples = new MySample[sampleCount];
		m_sampleCount = sampleCount;

        fff_EXPECT_TRUE(
			isAllocated());
	}

	void _dealloc()
	{
		fff_EXPECT_TRUE(
			isAllocated());

		delete [] m_samples;
		m_samples = NULL;
		m_sampleCount = 0;

        fff_EXPECT_FALSE(
			isAllocated());
	}



private:
	MySample
		*m_samples;
	UInt
		m_sampleCount;
};
}
}

#endif