//---------------------------------------------------------+
// fff/cl/fff.h.cl
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
//!	\file		fff.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		FFF definitions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fff_h_cl__included__
#define __fff_h_cl__included__

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





#define fff_KERNEL_TFUNC			tfuncKernel
#define fff_KERNEL_FCONV			fconvKernel
#define fff_KERNEL_DCONV			dconvKernel

#define fff_KERNEL_FFT				fftKernel
#define fff_KERNEL_IFFT				ifftKernel

#define fff_KERNEL_DFT				dftKernel
#define fff_KERNEL_IDFT				idftKernel





/* kernels and helper functions */
#if defined(_fff_OPTIMIZE) && defined(_fff_IS_DEV)

// check LB2N param
#ifndef _fff_OPTIMIZE_PARAM_LB2N
#error optimization _fff_OPTIMIZE_PARAM_LB2N log2(fft size) param not defined!
#endif

// check LB2W param
#ifndef _fff_OPTIMIZE_PARAM_LB2W
#error optimization _fff_OPTIMIZE_PARAM_LB2W log2(workgroup size) param not defined!
#endif

// check M param
#ifndef _fff_OPTIMIZE_PARAM_M
#error optimization _fff_OPTIMIZE_PARAM_M kernel size param not defined!
#endif

// required work size let the compiler optimize
#define __fff_KERNEL_OPTIMIZED_ATTRIBUTE \
		__attribute__((reqd_work_group_size(1<<_fff_OPTIMIZE_PARAM_LB2W, 1, 1)))

#define _fff_DONTOPTIMIZE_PARAMDEF
#define _fff_DONTOPTIMIZE_PARAMLIST

#else

// unknown worker count
#define __fff_KERNEL_OPTIMIZED_ATTRIBUTE

// instead of "ifndef _fff_OPTIMIZE" it's better
// to check "ifdef _fff_DONTOPTIMIZE"
#define _fff_DONTOPTIMIZE

#define _fff_DONTOPTIMIZE_PARAMDEF \
	, UInt _fff_OPTIMIZE_PARAM_LB2N \
	, UInt _fff_OPTIMIZE_PARAM_LB2W \
	, UInt _fff_OPTIMIZE_PARAM_M

#define _fff_DONTOPTIMIZE_PARAMLIST \
	, _fff_OPTIMIZE_PARAM_LB2N \
	, _fff_OPTIMIZE_PARAM_LB2W \
	, _fff_OPTIMIZE_PARAM_M

#endif

#ifdef _fff_IS_DEV
#define _fff_IMPLEMENT_CLCODE
#endif

#ifdef _fff_UNROLL


// UNROLL FFT SWITCH -----
// compiling takes 1min
#define __fff_UNROLL_FFT_LAYER          1
#define __fff_UNROLL_FFT_BUTTERFLY      1
#define __fff_UNROLL_FFT_FACTOR         1
#define __fff_UNROLL_FFT_ARRANGE        1
#define __fff_UNROLL_IFFT_LAYER         1
#define __fff_UNROLL_IFFT_BUTTERFLY     1
#define __fff_UNROLL_IFFT_SWAP          1
// -----------------------

// (to be implemented)
// UNROLL DFT SWITCH ----- (DONT! unroll N^^2)
// compiling takes 500MiB RAM
// not knowing when it ends.
#define __fff_UNROLL_DFT_OUTER          0
#define __fff_UNROLL_DFT_INNER          0
#define __fff_UNROLL_DFT_FACTOR         0
// -----------------------

// UNROLL FCONV SWITCH ---
// compiling takes 10sec
#define __fff_UNROLL_FCONV_EXPAND       1
#define __fff_UNROLL_FCONV_MULT         1
// -----------------------

// UNROLL TRANSFER FUNCTION
#define __fff_UNROLL_TFUNC              1


#define __fff_UNROLL_BITS_REVERSAL      1

#endif


_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif