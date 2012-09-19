//---------------------------------------------------------+
// fff/include/Computing/RuntimeMapping/Mapper.h
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
//!	\file		Mapper.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Runtime problem-solution mapping of given
//!				information to allow computation without
//!				overflows, but best usage of resources.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+
#ifndef __computingmap_h__included__
#define __computingmap_h__included__

#include "../../_intern.h"
#include "ComputingData.h"
#include "DeviceProperties.h"
#include "../../../cl/_.h.cl"

namespace fff {
namespace Computing {
namespace RuntimeMapping {


/* mapping of the data according
	to the device properties */
class Mapper
    :
    public iprintable
{
public:
    typedef Mapper My;
public:
	template<
		class SampleType
	>
	Mapper(
		const ComputingData<SampleType> &computingData,
		const DeviceProperties &deviceProperties,
		UInt throughputFactor
		)
        :
        m_f(
            throughputFactor)
	{
        fff_EXPECT_VALID_OBJ(computingData);
        fff_EXPECT_VALID_OBJ(deviceProperties);

		
        staticCalculate(
            m_lb2N,
            m_lb2W,
            computingData.getKernelElementsCount(),
            m_f,
            (UInt)deviceProperties.getMaxWorkerGroupCount());
	}

    static void staticCalculate(
        UInt &lb2N,
        UInt &lb2W,
        UInt M,
        UInt f,
        UInt maxW)
    {
        // computing strategy doesnt know anything about the input size

		lb2N = cl::lb2Multiple2(M) + f;
		lb2W = lb2N - 1; // N/2 worker <-> each butterfly, one worker

        if((ULong)fff_POW2(lb2W) > maxW)
            lb2W = cl::lb2Multiple2(maxW)-1;
    }

    UInt getLb2N() const
    {
        return 
            m_lb2N;
    }

	UInt getLb2W() const
	{
		return
			m_lb2W;
	}

    UInt getF() const
    {
        return 
            m_f;
    }

    virtual void print(logstream &out) const
    {
        out("Mapper");
        !fff_LOG_VAL(out, getLb2N());
        !fff_LOG_VAL(out, getLb2W());
        !fff_LOG_VAL(out, getF());
        out();
    }

    bool operator!() const
    {
        return
            false;
    }

    bool operator!=(const My &rhs) const
    {
        return
            !(fff_ME == rhs);
    }

    bool operator==(const My &rhs) const
    {
        fff_EXPECT_VALID_THIS();
        fff_EXPECT_VALID_OBJ(rhs);

        return
            getLb2N() == rhs.getLb2N() &&
            getLb2W() == rhs.getLb2W() &&
            getF() == rhs.getF();
    }

private:
	UInt
		m_lb2N,		// N
									/* Size of the fft convolution.
										Also defines size of input data
										to use (equals throughput of
										that iteration) and the size
										of preceding samples (equals
										state of the IR) */
		m_lb2W;

    UInt
        m_f;

};


}
}
}

#endif /* ifndef __fcoclcomputingmap_h__included__ */