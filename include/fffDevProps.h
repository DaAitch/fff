
#ifndef __fffdevprops_h__included__
#	define __fffdevprops_h__included__

#	include "_fffBase.h"
#	include "_fffIPrintable.h"

	_fff_BEGIN_NAMESPACE

		/* main properties of a device */
		class DevProps
			: public IPrintable
		{
		public:
			DevProps(
				const cl::Device &device);

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

			void print(
				LogStream &stream) const;

		private:
			UInt
				m_computingUnitsCount;	// amount of physical computing units
			ULong
				m_maxWorkerGroupCount,	// max amount of worker per group
				m_localMemorySize,		// local memory size in bytes
				m_constantMemorySize,	// constant memory size in bytes
				m_globalMemorySize;		// global memory size in bytes
		};

	_fff_END_NAMESPACE

#endif /* ifndef __fffdevprops_h__included__ */