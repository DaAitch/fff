//---------------------------------------------------------+
// fff/cl/dsp.cpp.cl
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
//!	\file		dsp.cpp.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		???
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
#include "fff.h.cl"
#include "dsp.h.cl"
#include "consts.h.cl"
#include "math.h.cl"
//---------------------------------------------------------+

//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
_fff_USING_NAMESPACE
//---------------------------------------------------------+

_fff_BEGIN_NAMESPACE
_fff_BEGIN_NAMESPACE_CL

#ifdef _fff_IMPLEMENT_CLCODE
//---------------------------------------------------------+
		Sample _kaiserWindow(
			UInt sampleIndex,
			UInt kernelSampleCount,
			Sample alpha
			)
		{
			Sample alphaPi = alpha * fff_M_PI;

			// inner part of the nominator
			Sample nomPart = 
				(Sample)(sampleIndex<<1)/
					(Sample)kernelSampleCount -
				(Sample)1;

			// nominator
			Sample nom = alphaPi * sqrt(
				(Sample)1 - 
					nomPart*nomPart);

			// denominator
			Sample denom = alphaPi;

			// kaiser window is the relation
			// between those modified bessel functions
			return
				_I0(nom) /
				_I0(denom);
		}
//---------------------------------------------------------+

//---------------------------------------------------------+
		Sample _I0(
				Sample x
				)
		{
			// half x
			Sample x2 =
				x / (Sample)2;

			// continuous gamma integral as
			// discrete summation
			Sample sum = 0;

			// unroll finite summation
#ifdef _fff_OPTIMIZE
#			pragma unroll
#endif
			for(
				UInt m = 0;
				m < _I0_M;
				++m)
			{
				UInt d =
					factorial(m);

				//   m!*tgamma(m+1)
				// = m!*m!*(m+1)
				d = d*d*(m+1);

				Sample fract = (Sample)(
					pown(x, m<<1) / d);

				sum += fract;
			}

			return sum;

		}
//---------------------------------------------------------+
	
#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE