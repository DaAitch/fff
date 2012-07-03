
#ifndef __fcoclcomputingdata_h__included__
#	define __fcoclcomputingdata_h__included__

#	include "_fffBase.h"
#	include "_fffException.h"
#	include "_fffIPrintable.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class ComputingData
			: public IPrintable
		{
		public:
			ComputingData(
				UInt kernelElementsCount)
				:
				m_kernelElementsCount(
					kernelElementsCount)
			{
				if(kernelElementsCount == 0)
					fff_THROW("invalid size");

				if(getDataTypeSize() == 0)
					fff_THROW("invalid sample type");
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

			void print(
				LogStream &out) const
			{
				out("TODO");
				out();
			}

		private:
			UInt m_kernelElementsCount;	/* amount of kernel elements */
		};

	_fff_END_NAMESPACE

#endif /* ifndef __fcoclcomputingdata_h__included__ */