//---------------------------------------------------------+
// fff/include/fffComputingData.h
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
//!	\file		fffComputingData.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Information of computation data.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffcomputingdata_h__included__
#	define __fffcomputingdata_h__included__

#	include "../../_intern.h"

using namespace fff::_Intern;

namespace fff {
namespace Computing {
namespace RuntimeMapping {

template<
	class SampleType
>
class ComputingData
	: public iprintable
{
public:
    typedef ComputingData<SampleType> My;
public:
	ComputingData(
		UInt kernelElementsCount)
		:
		m_kernelElementsCount(
			kernelElementsCount)
	{
        fff_EXPECT(m_kernelElementsCount, >, 0);
        fff_EXPECT(sizeof(SampleType), >, 0);
	}

	UInt getDataTypeSize() const
	{
		return
			sizeof(SampleType);
	}

	UInt getKernelElementsCount() const
	{
		return
			m_kernelElementsCount;
	}

    bool operator!() const throw()
    {
        return
            false;
    }

	virtual void print(logstream &out) const
    {
        out("ComputingData");
        !fff_LOG_VAL(out, getDataTypeSize());
        !fff_LOG_VAL(out, getKernelElementsCount());
        out();
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

        bool ret = true;
        ret &= (getDataTypeSize() == rhs.getDataTypeSize());
        ret &= (getKernelElementsCount() == rhs.getKernelElementsCount());

        return ret;
    }

private:
	UInt m_kernelElementsCount;	/* amount of kernel elements */
};

}
}
}

#endif /* ifndef __fcoclcomputingdata_h__included__ */