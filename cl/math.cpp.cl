//---------------------------------------------------------+
// fff/cl/complexmath.cpp.cl
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
//!	\file		complexmath.cpp.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Complex math.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
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

//---------------------------------------------------------+
UInt multiple2(
	UInt n)
{
	return
		1<<lb2Multiple2(n);
}
//---------------------------------------------------------+

//---------------------------------------------------------+
// TODO TEST
UInt multiple2In(
	UInt n)
{
	return
		1<<lb2Multiple2In(n);
}
//---------------------------------------------------------+

//---------------------------------------------------------+
UInt lb2Multiple2(
	UInt n)
{
#ifdef _fff_UNROLL
#pragma unroll
#endif
	for(
		UInt b = 0;
		b < (sizeof(UInt)<<3);
		++b)
	{
		if(n < ((UInt)1<<b))
			return b;
	}

	// overflow
	return (UInt)-1;
		
}
//---------------------------------------------------------+

//---------------------------------------------------------+
// TODO TEST
UInt lb2Multiple2In(
	UInt n)
{
#ifdef _fff_UNROLL
#pragma unroll
#endif
	for(
		UInt b = 0;
		b < (sizeof(UInt)<<3);
		++b)
	{
		if(n <= ((UInt)1<<b))
			return b;
	}

	// overflow
	return (UInt)-1;
		
}
//---------------------------------------------------------+

//---------------------------------------------------------+
Bool isMultiple2(
		UInt n)
{
	return
		multiple2In(n) == n ?
			True :
			False;
}
//---------------------------------------------------------+

//---------------------------------------------------------+
UInt factorial(
		UInt n
		)
{
	++n;
	UInt res=1;
	while(--n>1)
		res*=n;
	return res;
}
//---------------------------------------------------------+



#ifdef _fff_IMPLEMENT_CLCODE

//---------------------------------------------------------+
void cmul(
	Sample *yRe,
	Sample *yIm,
	Sample  w,
	Sample  xRe,
	Sample  xIm
	)
{
	// euler's identity formula
	Sample mIm, mRe;
	mIm = sincos(w, &mRe);

	// complex multiplication
	(*yRe) =
			(xRe * mRe)
		- (xIm * mIm);
	(*yIm) =
			(xRe * mIm)
		+ (xIm * mRe);
}
//---------------------------------------------------------+

#endif
		
_fff_END_NAMESPACE
_fff_END_NAMESPACE