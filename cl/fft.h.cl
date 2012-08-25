//---------------------------------------------------------+
// fff/cl/fft.h.cl
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
//!	\file		fft.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Fast Fourier Transformation.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fft_h_cl__included__
#define __fft_h_cl__included__

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




inline
void _fftButterfly(
	UInt   *firstIndex,
	UInt   *secondIndex,
	Sample *twiddleExpPiFactor,
	UInt    layerNum,
	UInt    butterflyNum
	);

#ifdef _fff_IMPLEMENT_CLCODE

void _fftArrange(
	__global const Sample *xRe,
	__global const Sample *xIm,
	__global       Sample *yRe,
	__global       Sample *yIm
	
	_fff_DONTOPTIMIZE_PARAMDEF

	);

void __fftPerform(
	__global Sample *xRe,
	__global Sample *xIm

	_fff_DONTOPTIMIZE_PARAMDEF

	);

void __ifftPerform(
	__global Sample *XRe,
	__global Sample *XIm

	_fff_DONTOPTIMIZE_PARAMDEF

	);

void _fft(
	__global Sample *xRe,
	__global Sample *xIm
	         
	_fff_DONTOPTIMIZE_PARAMDEF

	);

void _ifft(
	__global Sample *XRe,
	__global Sample *XIm
	
	_fff_DONTOPTIMIZE_PARAMDEF

	);

__kernel
__fff_KERNEL_OPTIMIZED_ATTRIBUTE
void fff_KERNEL_FFT(

// dont need no arrange buffer
// in optimized mode
#ifdef _fff_DONTOPTIMIZE
	__global const Sample *xRe,
	__global const Sample *xIm,
#endif

	__global       Sample *XRe,
	__global       Sample *XIm

    _fff_DONTOPTIMIZE_PARAMDEF

	);

__kernel
__fff_KERNEL_OPTIMIZED_ATTRIBUTE
void fff_KERNEL_IFFT(

// dont need no arrange buffer
// in optimized mode
#ifdef _fff_DONTOPTIMIZE
	__global const Sample *XRe,
	__global const Sample *XIm,
#endif

	__global       Sample *xRe,
	__global       Sample *xIm

    _fff_DONTOPTIMIZE_PARAMDEF

	);

#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif