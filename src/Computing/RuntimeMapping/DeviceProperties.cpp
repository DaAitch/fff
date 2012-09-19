//---------------------------------------------------------+
// fff/src/Computing/RuntimeMapping/DeviceProperties.cpp
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
//!	\file		DeviceProperties.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Device properties like memory sizes.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#include "../../../include/Computing/RuntimeMapping/DeviceProperties.h"
#include "../../../include/_intern.h"

using namespace fff::_Intern::RtCLC;
using namespace fff::_Intern::Exceptions::RtCLC;

namespace fff {
namespace Computing {
namespace RuntimeMapping {

DeviceProperties::DeviceProperties(
	const ::cl::Device &device
	)
{
    fff_EXPECT_VALID_CLOBJ(device);

	fff_RTCLC_ERR_INIT();

	// fetch computing units
	fff_RTCLC_SEQ_CHECK(
		m_computingUnitsCount =
			device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>(fff_RTCLC_ERR_PTR));
	
	// fetch max constant memory size
	fff_RTCLC_SEQ_CHECK(
		m_constantMemorySize =
			device.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>(fff_RTCLC_ERR_PTR));

	// fetch max global memory size
	fff_RTCLC_SEQ_CHECK(
		m_globalMemorySize =
			device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>(fff_RTCLC_ERR_PTR));

	// fetch max local memory size
	fff_RTCLC_SEQ_CHECK(
		m_localMemorySize =
			device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>(fff_RTCLC_ERR_PTR));

	// fetch max worker item per workgroup
	fff_RTCLC_SEQ_CHECK(
		m_maxWorkerGroupCount =
			device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>(fff_RTCLC_ERR_PTR));
}

}
}
}