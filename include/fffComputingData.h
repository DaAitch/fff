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

#ifndef __fffcomputingdata_h__included__
#	define __fffcomputingdata_h__included__

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