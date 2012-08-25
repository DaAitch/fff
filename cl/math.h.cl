//---------------------------------------------------------+
// fff/cl/complexmath.h.cl
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
//!	\file		???
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		???
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __complexmath_h_cl__included__
#define __complexmath_h_cl__included__

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




//! Ceiling division, e.g \f$ 100 / 99 = 2 \f$
#define fff_CEILDIV(X, N) \
	(((X)+(N)-1)/(N))

#define _fff_POW2(T, E) \
    ((T)1 << (E))
#define fff_POW2(E) \
    fff_POW2_UINT(E)

#define fff_POW2_USHORT(E) \
    _fff_POW2(UShort, E)
#define fff_POW2_UINT(E) \
    _fff_POW2(UInt, E)
#define fff_POW2_ULONG(E) \
    _fff_POW2(ULong, E)

	
UInt multiple2(
	UInt n);

UInt lb2Multiple2(
	UInt n);

    	
UInt multiple2In(
	UInt n);

UInt lb2Multiple2In(
	UInt n);

Bool isMultiple2(
	UInt n);


		


UInt factorial(
	UInt n //!< [in]
	);


#ifdef _fff_IMPLEMENT_CLCODE
//! \brief Complex multiplication with complex euler factor.
//! Multiplies x_re/im with e^(j*w) and store it to
//! the y_re/im parameter.
void cmul(
	Sample *yRe,	//!< [out]	Re{y}
	Sample *yIm,	//!< [out]	Im{y}
	Sample  w,		//!< [in]	complex exponent
	Sample  xRe,	//!< [in]	Re{x}
	Sample  xIm		//!< [in]	Im{x}
	);
#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif