//---------------------------------------------------------+
// fff/include/Buffer/Complex/Host/IHostMultiChannel.h
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
//!	\file		IHostMultiChannel.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Interface of a multi channel host buffer.
//!				One implementation is the CvHstMChBuf or
//!				the preallocated version, to be treated
//!				equal by the kernel computation.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __ffficvmchbuf_h__included__
#define __ffficvmchbuf_h__included__

#include "../../../_intern/_base.h"

namespace fff {
namespace Buffer {
namespace Complex {
namespace Host {

template<
	class SampleType
>
class IHostMultiChannel
    :
    public iprintable
{
public:
	typedef
		IHostMultiChannel<SampleType> My;

public:
    virtual ~IHostMultiChannel()
    {
    }

	virtual SampleType *getRawReal(
		UInt channelIndex) = 0;
	virtual SampleType *getRawImag(
		UInt channelIndex) = 0;

	virtual const SampleType *getRawReal(
		UInt channelIndex) const = 0;
	virtual const SampleType *getRawImag(
		UInt channelIndex) const = 0;

	virtual UInt getChannelCount() const = 0;
	virtual UInt getSampleCount() const = 0;

	virtual Bool isAllocated() const = 0;

    virtual void init() = 0;

    

    Bool hasSameSize(const My &rhs) const
    {
        return
            hasSameSampleCount(rhs) &&
            hasSameChannelCount(rhs);
    }

    Bool hasSameSampleCount(const My &rhs) const
    {
        return
            getSampleCount() == rhs.getSampleCount();
    }

    Bool hasSameChannelCount(const My &rhs) const
    {
        return
            getChannelCount() == rhs.getChannelCount();
    }

    virtual bool operator!() const throw() = 0;

    virtual void print(logstream &out) const = 0;

};
}
}
}
}
#endif