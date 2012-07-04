//---------------------------------------------------------+
// fff/include/fffClEnv.h
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
//!	\file		fffClEnv.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		OpenCL environment and helper methods.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffclenv_h__included__
#	define __fffclenv_h__included__

#	include "_fffBase.h"
#	include "_fffException.h"

_fff_BEGIN_NAMESPACE
	
	class ClEnv
	{
	public:
		ClEnv(
			const cl::Device &device);

		ClEnv(
			cl_device_type deviceType);

		static Devices staticGetDevices();

		cl::Platform &getPlatform()
		{
			return
				m_platform;
		}

		cl::Context &getContext()
		{
			return
				m_context;
		}

		cl::Device &getDevice()
		{
			return
				m_device;
		}

		const cl::CommandQueue &getQueue() const
		{
			return
				m_commandQueue;
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
		cl::Platform
			m_platform;
		cl::Context
			m_context;
		cl::Device
			m_device;
		cl::CommandQueue
			m_commandQueue;
	};

_fff_END_NAMESPACE

#endif