//---------------------------------------------------------+
// fff/cl/bits.cpp.cl
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
//!	\file		bits.cpp.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Some bit macros and functions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
#include "bits.h.cl"
#include "fff.h.cl"
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


UInt bit_reversal(
	UInt x, 
	UInt w
	)
{
	UInt out=0; // bit reversal result

	// go through each bit
#if __fff_UNROLL_BITS_REVERSAL
#pragma unroll
#endif
#ifdef _fff_OPTIMIZE
	for(UInt i=0; i < (sizeof(UInt)<<3)>>1; ++i)
		out |= fff_BITSWAPONLY(
			x, i, sizeof(UInt)<<3);
#else
	for(UInt i=0; i < ( (w+1) >> 1); ++i)
		out |= fff_BITSWAPONLY(
			x, i, w);
#endif
		
		
#ifdef _fff_OPTIMIZE
	return
		out>>((sizeof(UInt)<<3)-w);
#else
	return
		out;
#endif
	
}

_fff_END_NAMESPACE
_fff_END_NAMESPACE