//---------------------------------------------------------+
// fff/include/Buffer/Complex/ComplexVector.h
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
//!	\file		ComplexVector.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Complex vector.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __complexvector_h__included__
#define __complexvector_h__included__

#include "../../_intern/_base.h"
#include "../Vector.h"

namespace fff {
namespace Buffer {
namespace Complex {

template<
    class SampleType
>
class ComplexVector
    :
    public iprintable
{
public:
	typedef
		SampleType MySample;
	typedef
		ComplexVector<MySample> My;
	typedef
		Vector<MySample> MySamples;
	typedef
		MySample Gain;

public:
	ComplexVector(
		UInt sampleCount=0)
		:
		m_sampleCount(
			0)
	{
		alloc(
			sampleCount);

		init();
	}

	ComplexVector(
		const My &rhs,
		UInt newSize)
		: m_re(rhs.m_re, newSize),
		m_im(rhs.m_im, newSize),
		m_sampleCount(rhs.m_sampleCount)
	{
	}

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

	bool operator!() const
	{
		return
			!m_re ||
            !m_im ||
            m_sampleCount == 0;
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

    void printValues(ostream &out)
    {
        for(
            UInt i = 0;
            i < getSampleCount();
            ++i)
        {
            out << "[";
            out << getReal()[i];
            out << ", ";
            out << getImag()[i];
            out << "]" << std::endl;
        }
    }

	My &operator*=(
		MySample factor)
	{
		getReal() *= factor;
		getImag() *= factor;

		return
            fff_ME;
	}

	My &operator/=(
		MySample divider)
	{
		getReal() /= divider;
		getImag() /= divider;

		return
            fff_ME;
	}

    My &operator+=(
        const My &rhs)
    {
        getReal() += rhs.getReal();
        getImag() += rhs.getImag();

        return
            fff_ME;
    }

    My &operator-=(
        const My &rhs)
    {
        getReal() -= rhs.getReal();
        getImag() -= rhs.getImag();

        return
            fff_ME;
    }

    virtual void print(logstream &out) const
    {
        out("ComplexVector");
        !fff_LOG_VAL(out, getSampleCount());
        !fff_LOG_VAL(out, getReal());
        !fff_LOG_VAL(out, getImag());
        out();
    }

    // TODO
	My operator*(const My &rhs) const
	{
		if(getSampleCount() != rhs.getSampleCount())
			fff_THROW("different sample count");

		My r(getSampleCount());
		for(
			UInt i = 0;
			i < getSampleCount();
			++i)
		{
			r.getReal()[i] = getReal()[i]*rhs.getReal()[i] -
				getImag()[i]*rhs.getImag()[i];
			r.getImag()[i] = getReal()[i]*rhs.getImag()[i] +
				getImag()[i]*rhs.getReal()[i];
		}

		return r;
	}


private:
	MySamples
		m_re,
		m_im;
	UInt
		m_sampleCount;
};
}
}
}
#endif