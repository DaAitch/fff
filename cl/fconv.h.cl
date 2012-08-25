//---------------------------------------------------------+
// fff/cl/fconv.h.cl
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
//!	\file		fconv.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		???
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fconv_h_cl__included__
#define __fconv_h_cl__included__

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

/* fast convolution kernel */
__kernel
__fff_KERNEL_OPTIMIZED_ATTRIBUTE
void fff_KERNEL_FCONV(

	/* size is data_size */
	__global const Sample *g_x_real,
	__global const Sample *g_x_imag,
                   UInt    data_size,

	/* size is _fff_FFT_SIZE */
	__global const Sample *g_H_real,
	__global const Sample *g_H_imag,

	/* size is the correct size for overlap save algorithm */
	__global	Sample	*g_uninit_tmp_real,
	__global	Sample	*g_uninit_tmp_imag,

	__global	Sample	*g_uninit_tmp2_real,
	__global	Sample	*g_uninit_tmp2_imag,

	/* size is data_size - fff_STATE_SIZE */
	__global	Sample	*g_uninit_y_real,
	__global	Sample	*g_uninit_y_imag

    _fff_DONTOPTIMIZE_PARAMDEF

	);

#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif