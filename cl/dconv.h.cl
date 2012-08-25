//---------------------------------------------------------+
// fff/cl/dconv.h.cl
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
//!	\file		dconv.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Discrete direct Convolution.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __dconv_h_cl__included__
#define __dconv_h_cl__included__

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
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

Sample dconv(
#ifdef _fff_IS_DEV
    __global
#endif
    const Sample *x,
          UInt    xsize,
#ifdef _fff_IS_DEV
    __global
#endif
    const Sample *h,
          UInt    hsize,
          UInt    i
    );


#ifdef _fff_IMPLEMENT_CLCODE

__kernel
void fff_KERNEL_DCONV(
	__global const Sample *x,
                   UInt    xsize,
	__global const Sample *h,
	__global       Sample *y

    _fff_DONTOPTIMIZE_PARAMDEF

	);

#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif