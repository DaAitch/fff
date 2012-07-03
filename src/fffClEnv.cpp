/*----------------------------------------------------------
 *    The Fast Filtering Framework implements an LTI filter
 *    with Khronos Group's OpenCL.
 *    Copyright (C) 2012  Philipp Renoth
 *----------------------------------------------------------
 *    This program is free software: you can redistribute
 *    it and/or modify it under the terms of the
 *    GNU General Public License as published by the
 *    Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the
 *    GNU General Public License along with this program.
 *    If not, see <http://www.gnu.org/licenses/>.
 *--------------------------------------------------------*/

#include "../include/fffClEnv.h"



_fff_NAMESPACE_PREFIX ClEnv::ClEnv(
	const cl::Device &device)
{
	fff_RTCLC_ERR_INIT();

	Devices devices(1);
	devices[0] = device;

	cl::Platform platform;

	fff_RTCLC_SEQ_CHECK(
		platform = device.getInfo<CL_DEVICE_PLATFORM>(fff_RTCLC_ERR_PTR));

	cl_context_properties contextProps[] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)platform(),
		NULL
	};

	fff_RTCLC_SEQ_CHECK(
		m_context = cl::Context(
			devices,
			contextProps,
			NULL,
			NULL,
			fff_RTCLC_ERR_PTR));

	assert(
		m_context());

	createQueue();
}

_fff_NAMESPACE_PREFIX ClEnv::ClEnv(
	cl_device_type deviceType)
{
	fff_RTCLC_ERR_INIT();

	fff_RTCLC_SEQ_CHECK(
		m_platform = cl::Platform::get(
			fff_RTCLC_ERR_PTR));

	cl_context_properties contextProps[] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)m_platform(),
		NULL
	};

	fff_RTCLC_SEQ_CHECK(
		m_context = cl::Context(
		deviceType,
		(cl_context_properties *)&contextProps,
		NULL,
		fff_RTCLC_ERR_PTR));

	Devices devices;

	fff_RTCLC_SEQ_CHECK(
		devices = m_context.getInfo<CL_CONTEXT_DEVICES>(
			fff_RTCLC_ERR_PTR));

	if(devices.size()>0)
		m_device = devices[0];
	else
		fff_THROW("no such device");

	createQueue();
}

Devices _fff_NAMESPACE_PREFIX ClEnv::staticGetDevices()
{
	fff_RTCLC_ERR_INIT();

	Platforms platforms;
			
	fff_RTCLC_SEQ_CHECK_RET(
		cl::Platform::get(&platforms));

	Devices devices;

	for(
		Platforms::const_iterator it = platforms.begin();
		it != platforms.end();
		++it)
	{
		Devices platformDevices;
		fff_RTCLC_SEQ_CHECK_RET(
			it->getDevices(CL_DEVICE_TYPE_ALL, &platformDevices));

		for(
			Devices::const_iterator dit = platformDevices.begin();
			dit != platformDevices.end();
			++dit)
		{
			devices.push_back(
				*dit);
		}
	}
			
	return devices;
}

void _fff_NAMESPACE_PREFIX ClEnv::createQueue()
{
	fff_RTCLC_ERR_INIT();

	fff_RTCLC_SEQ_CHECK(
		m_commandQueue = cl::CommandQueue(
			m_context,
			m_device,
			0,
			fff_RTCLC_ERR_PTR));

	assert(
		m_commandQueue());
}