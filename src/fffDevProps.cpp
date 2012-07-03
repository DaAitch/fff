
#include "../include/fffDevProps.h"


_fff_NAMESPACE_PREFIX DevProps::DevProps(
	const cl::Device &device
	)
{
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

void _fff_NAMESPACE_PREFIX DevProps::print(
	LogStream &stream) const
{
	stream("Device Properties");
	~(stream >> "Computing Units" << m_computingUnitsCount);
	~(stream >> "Constant Memory" << m_constantMemorySize);
	~(stream >> "Local Memory" << m_localMemorySize);
	~(stream >> "Global Memory" << m_globalMemorySize);
	~(stream >> "Max. Worker per group" << m_maxWorkerGroupCount);
	stream();
}