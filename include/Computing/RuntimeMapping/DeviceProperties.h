//---------------------------------------------------------+
// fff/include/fffDevProps.h
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
//!	\file		fffDevProps.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Device properties like memory sizes.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffdevprops_h__included__
#	define __fffdevprops_h__included__

#	include "../../_intern.h"

using namespace fff::_Intern;
using namespace fff::_Intern::Exceptions;
using namespace fff::_Intern::Exceptions::Runtime;

namespace fff {
namespace Computing {
namespace RuntimeMapping {

/* main properties of a device */
class DeviceProperties
	: public iprintable
{
public:
    typedef DeviceProperties My;
public:
	DeviceProperties(
		const ::cl::Device &device);

	UInt getComputingUnitsCount() const
	{
		return
			m_computingUnitsCount;
	}

	ULong getMaxWorkerGroupCount() const
	{
		return
			m_maxWorkerGroupCount;
	}

	ULong getLocalMemorySize() const
	{
		return
			m_localMemorySize;
	}

	ULong getConstantMemorySize() const
	{
		return
			m_constantMemorySize;
	}

	ULong getGlobalMemorySize() const
	{
		return
			m_globalMemorySize;
	}

    bool operator!() const throw()
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

        bool ret = true;

        ret &= (getComputingUnitsCount() == rhs.getComputingUnitsCount());
        ret &= (getConstantMemorySize() == rhs.getConstantMemorySize());
        ret &= (getGlobalMemorySize() == rhs.getGlobalMemorySize());
        ret &= (getLocalMemorySize() == rhs.getLocalMemorySize());
        ret &= (getMaxWorkerGroupCount() == rhs.getMaxWorkerGroupCount());

        return ret;
    }

	virtual void print(logstream &out) const
    {
        out("DeviceProperties");
        !fff_LOG_VAL(out, getComputingUnitsCount());
        !fff_LOG_VAL(out, getConstantMemorySize());
        !fff_LOG_VAL(out, getGlobalMemorySize());
        !fff_LOG_VAL(out, getLocalMemorySize());
        !fff_LOG_VAL(out, getMaxWorkerGroupCount());
        out();
    }

private:
	UInt
		m_computingUnitsCount;	// amount of physical computing units
	ULong
		m_maxWorkerGroupCount,	// max amount of worker per group
		m_localMemorySize,		// local memory size in bytes
		m_constantMemorySize,	// constant memory size in bytes
		m_globalMemorySize;		// global memory size in bytes
};

}
}
}

#endif /* ifndef __fffdevprops_h__included__ */