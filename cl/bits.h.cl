//---------------------------------------------------------+
// fff/cl/bits.h.cl
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
//!	\file		bits.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Some bit macro and functions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __bits_h_cl__included__
#define __bits_h_cl__included__

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
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



//! Bit a position M
#define fff_BITMASK(M) \
	(1<<(M))

//! Swap two bits of \em X, with width \em W and
//! at position \em P
#define fff_BITSWAPONLY(X,P,W) \
	(	(((X)&fff_BITMASK(P))<< \
			((W)-((P)<<1)-1)) |	\
		(((X)&fff_BITMASK((W)-(P)-1))>> \
			((W)-((P)<<1)-1)) )

//! Swap all bits, equals reversal operation.
//! \return Bit reversal of the LSB \c w bits.
//! \warning Other bits except \c w bits are zero.
//!		Though no inplace swapping!
UInt bit_reversal(
	UInt x,	//!< [in] Bits to swap
	UInt w	//!< [in] Width of the swap (in bits)
);

_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif