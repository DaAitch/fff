
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