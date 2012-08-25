//---------------------------------------------------------+
// fff/include/fffOpenCLEnvironment.h
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
//!	\file		fffOpenCLEnvironment.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		OpenCL environment and helper methods.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffOpenCLEnvironment_h__included__
#define __fffOpenCLEnvironment_h__included__

#include "../_intern.h"

using namespace fff::_Intern::Exceptions::RtCLC;
using namespace fff::_Intern::Exceptions::Runtime;

namespace fff {
namespace Computing {
	
class OpenCLEnvironment
    :
    public iprintable
{
public:

    OpenCLEnvironment();

	OpenCLEnvironment(
		const ::cl::Device &device);

	OpenCLEnvironment(
		cl_device_type deviceType);

    

	static Devices staticGetDevices();

	const ::cl::Platform &getPlatform() const
	{
		fff_EXPECT_VALID_CLOBJ_RET(
            m_platform);
	}

	const ::cl::Context &getContext() const
	{
        fff_EXPECT_VALID_CLOBJ_RET(
            m_context);
	}

	const ::cl::Device &getDevice() const
	{
        fff_EXPECT_VALID_CLOBJ_RET(
            m_device);
	}

	const ::cl::CommandQueue &getQueue() const
	{
        fff_EXPECT_VALID_CLOBJ_RET(
            m_commandQueue);
	}

    virtual void print(logstream &out) const
    {
        out("OpenCLEnvironment");
        // TODO: implement logstream &operator<<(logstream &, ::cl::...)
        out();
    }

	bool operator!() const
	{
		return !m_platform() ||
			!m_context() ||
			!m_device() ||
			!m_commandQueue();
	}

private:

	void createQueue();

private:
	::cl::Platform
		m_platform;
	::cl::Context
		m_context;
	::cl::Device
		m_device;
	::cl::CommandQueue
		m_commandQueue;
};

}
}

#endif