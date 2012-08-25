//---------------------------------------------------------+
// fff/cl/fconv.cpp.cl
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
//!	\file		fconv.cpp.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Fast convolution.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
#include "fff.h.cl"
#include "fconv.h.cl"
#include "overlapsave.h.cl"
#include "consts.h.cl"
#include "fft.h.cl"
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
/* fast convolution kernel */
__kernel
void fff_KERNEL_FCONV(

	// size is xElemCount
	__global const Sample *g_x_real,
	__global const Sample *g_x_imag,
	               UInt    xElemCount,

	// size is 1<<_fff_OPTIMIZE_PARAM_LB2N
	// as param or macro
	__global const Sample *g_H_real,
	__global const Sample *g_H_imag,

	// size is the correct size to be
	// able to compute all x's
	__global       Sample *g_uninit_tmp_real,
	__global       Sample *g_uninit_tmp_imag,

	__global       Sample *g_uninit_tmp2_real,
	__global       Sample *g_uninit_tmp2_imag,

	// size is xElemCount - M + 1
	__global       Sample *g_uninit_y_real,
	__global       Sample *g_uninit_y_imag


	_fff_DONTOPTIMIZE_PARAMDEF

	)
{
	
	UInt
		// Not corrupted samples because of overlapping
		// is the size of the fft minus the state, though
		// kernel size minus 1.
		// L = N - (M-1)
		unaliasThroughputSize	=
			(1<<_fff_OPTIMIZE_PARAM_LB2N) -
			(_fff_OPTIMIZE_PARAM_M - 1),

		// Length of the "g_uninit_tmp(2)_real/imag" buffer are
		// g * N, with g is the number of groups (in OpenCL case working groups)
		// we need to compute xElemCount samples with the kernel.
		// This offset is the absolute offset in the "g_uninit_tmp(2)_real/imag"
		// buffer, each working group a different offset because the can be
		// computed concurrently.
		offset					= get_group_id(0) * (1<<_fff_OPTIMIZE_PARAM_LB2N),

		// With computing g * N blocks there are some tailing samples of
		// the filter decay which we dont need but compute them anyway.
		unaliasOutputSize		= xElemCount - (_fff_OPTIMIZE_PARAM_M - 1);

	// Absolute offset pointer to be able to compute ffts etc.
	// with no respect to the working group, but with
	// the local id and those pointers.
	__global Sample

		// 1. Stores expanded overlappings from x_real/imag
		//    in arranged order.
		// 2. Compute inplace fft blocks.
		// 3. Multiply and store spectral complex value with
		//    g_H_real/imag.
		*x_real					= g_uninit_tmp_real  + offset,
		*x_imag					= g_uninit_tmp_imag  + offset,

		// 4. Arrange "x_real/imag" in "x2_real/imag"
		// 5. Compute inplace ifft blocks.
		// 6. Copy correct data from "x2_real/imag" to
		//    "g_uninit_y_real/imag"
		*x2_real				= g_uninit_tmp2_real + offset,
		*x2_imag				= g_uninit_tmp2_imag + offset;


	// x1_real/imag holds the convolved data parts
	// last (_fff_OPTIMIZE_PARAM_M - 1) samples
	// are distorted because of filter decay

	// structure of all:
	// | a1|   d1  | a2|   d2  | ... | D | A |
	// \__________/ \__________/ 
	// x2_real/imag x2_real/imag
	// structure after:
	// |   d1  |   d2  |   d3  | ... | D |
	// a: aliased data
	// d: correct data
	// D: shorter korrect data at the end
	//     with A as aliased filter decay
	// A: aliased filter decay



#if __fff_UNROLL_FCONV_EXPAND
#pragma unroll
#endif
	for(
		UInt
			i = 0,
			s = get_local_id(0);
		i < (1<<(_fff_OPTIMIZE_PARAM_LB2N-_fff_OPTIMIZE_PARAM_LB2W));
		++i
		)
	{
		UInt fromIndex, toIndex;

		calcOverlapSaveSampleOffset(
			&fromIndex,
			&toIndex,
			get_group_id(0),
			s,
			_fff_OPTIMIZE_PARAM_LB2N,
			_fff_OPTIMIZE_PARAM_M,
			xElemCount
			);

		// x's might come from third party
		// and cannot be assigned to have a "nice" length
		if(fromIndex < xElemCount)
		{
			g_uninit_tmp_real[toIndex] = g_x_real[fromIndex];
			g_uninit_tmp_imag[toIndex] = g_x_imag[fromIndex];
		}
		else
		{
			g_uninit_tmp_real[toIndex] = fff_SMP_ZERO;
			g_uninit_tmp_imag[toIndex] = fff_SMP_ZERO;
		}

		s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);
	}
			
	barrier(
		CLK_GLOBAL_MEM_FENCE);

	_fft(
		x_real,
		x_imag

		_fff_DONTOPTIMIZE_PARAMLIST

		);

#if __fff_UNROLL_FCONV_MULT
#pragma unroll
#endif
	for(UInt
			i = 0,
			s = get_local_id(0);
		i < (1<<(_fff_OPTIMIZE_PARAM_LB2N-_fff_OPTIMIZE_PARAM_LB2W));
		++i
		)
	{
		Sample x_real_tmp, x_imag_tmp;

		// complex a*b=c
		// cRe = aRe*bRe - aIm*bIm
		// cIm = aRe*bIm + aIm*bRe

		x_real_tmp = 
			(x_real[s] * g_H_real[s] -
				x_imag[s] * g_H_imag[s]);
		x_imag_tmp =
			(x_real[s] * g_H_imag[s] +
				x_imag[s] * g_H_real[s]);

		x_real[s] = x_real_tmp;
		x_imag[s] = x_imag_tmp;

		s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);
	}
        
	barrier(
		CLK_GLOBAL_MEM_FENCE);
		
	_fftArrange(
		x_real,
		x_imag,
		x2_real,
		x2_imag

		_fff_DONTOPTIMIZE_PARAMLIST

		);
		
	_ifft(
		x2_real,
		x2_imag

		_fff_DONTOPTIMIZE_PARAMLIST

		);
		

			


	// work-group <x> should copy d<x>
	for(
		UInt i = get_local_id(0);
		i < unaliasThroughputSize;
		i += (1<<_fff_OPTIMIZE_PARAM_LB2W))
	{
		UInt to = (get_group_id(0) *
			unaliasThroughputSize) +
			i;

		if( to < unaliasOutputSize)
		{
			g_uninit_y_real[to] =
					x2_real[_fff_OPTIMIZE_PARAM_M-1+i];
			g_uninit_y_imag[to] =
					x2_imag[_fff_OPTIMIZE_PARAM_M-1+i];
		}
	}
}
//---------------------------------------------------------+

#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE