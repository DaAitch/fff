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

#ifndef __ffficvmchbuf_h__included__
#	define __ffficvmchbuf_h__included__

#	include "_fffBase.h"

	template<
		class SampleType
	>
	class ICvMChBuf
	{
	public:
		typedef
			SampleType MySample;

	public:

		virtual MySample *getRawReal(
			UInt channelIndex) = 0;
		virtual MySample *getRawImag(
			UInt channelIndex) = 0;

		virtual const MySample *getRawReal(
			UInt channelIndex) const = 0;
		virtual const MySample *getRawImag(
			UInt channelIndex) const = 0;

		virtual UInt getChannelCount() const = 0;
		virtual UInt getSampleCount() const = 0;

		virtual Bool isAllocated() const = 0;

	};
#endif