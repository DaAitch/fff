
//---------------------------------------------------------+
// fff/cl/tf.h.cl
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
//!	\file		tf.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Transfer function.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __tf_h_cl__included__
#define __tf_h_cl__included__

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

#ifdef _fff_IMPLEMENT_CLCODE

//! \brief Offline calculation of the FIR's H-Function.
//! Since the filter is known before calculating the
//! convolutions it is better to compute H offline
//! and just get fourier factors of the input x's and
//! multply them spectral and get the continuous domain.
__kernel
__fff_KERNEL_OPTIMIZED_ATTRIBUTE
void fff_KERNEL_TFUNC(

	// size is:
	// - default sizeb
	// - optimized _fff_OPTIMIZE_PARAM_LB2N
	__global const Sample *bRe,
	__global const Sample *bIm,
		
	// size is:
	// - default sizea
	// - optimized _fff_OPTIMIZE_PARAM_LB2N
	__global const Sample *aRe,
	__global const Sample *aIm,

	// size is 1<<_fff_OPTIMIZE_PARAM_LB2N
	// as param or macro
	__global       Sample *ARe,
	__global       Sample *AIm,

	// size is 1<<_fff_OPTIMIZE_PARAM_LB2N
	// as param or macro
	__global       Sample *HRe,
	__global       Sample *HIm

	_fff_DONTOPTIMIZE_PARAMDEF

	);

#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif