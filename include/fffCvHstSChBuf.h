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

#ifndef __fffcvhstschbuf_h__included__
#	define __fffcvhstschbuf_h__included__

#	include "_fffBase.h"
#	include "fffCv.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class CvHstSChBuf
			: public Cv<SampleType>
		{
		public:
			typedef
				SampleType MySample;
			typedef
				CvHstSChBuf<MySample> My;

			CvHstSChBuf(
				UInt sampleCount = 0)
				: Cv(sampleCount)
			{
			}
		};

	_fff_END_NAMESPACE

#endif