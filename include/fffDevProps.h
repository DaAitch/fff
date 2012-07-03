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