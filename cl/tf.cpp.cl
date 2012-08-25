//---------------------------------------------------------+
// fff/cl/tf.cpp.cl
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
//!	\file		tf.cpp.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		IIR Transfer function calculation.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
#include "tf.h.cl"
#include "fff.h.cl"
#include "fft.h.cl"
#include "consts.h.cl"
#include "bits.h.cl"
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
/* calculating transfer function offline */
__kernel
void fff_KERNEL_TFUNC(
	__global const Sample *bRe,
	__global const Sample *bIm,
	__global const Sample *aRe,
	__global const Sample *aIm,
	__global       Sample *ARe,
	__global       Sample *AIm,
	__global       Sample *HRe,
	__global       Sample *HIm

	_fff_DONTOPTIMIZE_PARAMDEF

	)
{


#ifdef __fff_UNROLL_TFUNC
#pragma unroll
#endif
	for(
		UInt
            i = 0,
		    s = get_local_id(0);
		i < (1<<(_fff_OPTIMIZE_PARAM_LB2N-_fff_OPTIMIZE_PARAM_LB2W));
		++i)
	{
		UInt j = bit_reversal(
			s,
			_fff_OPTIMIZE_PARAM_LB2N);

		// copy zeros
		HRe[s] = bRe[j];
		HIm[s] = bIm[j];

		// copy poles
		ARe[s] = aRe[j];
		AIm[s] = aIm[j];
        
		s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);
	}

	barrier(CLK_GLOBAL_MEM_FENCE);
	
	_fft(
		HRe,
		HIm

		_fff_DONTOPTIMIZE_PARAMLIST

		);
    
	_fft(
		ARe,
		AIm

		_fff_DONTOPTIMIZE_PARAMLIST

		);


	// H = Hb / Ha (complex division !!!)
	for(
		UInt
            i = 0,
		    s = get_local_id(0);
		i < (1<<(_fff_OPTIMIZE_PARAM_LB2N-_fff_OPTIMIZE_PARAM_LB2W));
		++i)
	{
		Sample denom =
			ARe[s]*ARe[s] +
			AIm[s]*AIm[s];

		Sample nom1 =
			HRe[s]*ARe[s] +
			HIm[s]*AIm[s];

		Sample nom2 =
			HIm[s]*ARe[s] -
			HRe[s]*AIm[s];

		HRe[s] = nom1 / denom;
		HIm[s] = nom2 / denom;

		s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);
	}
}
//---------------------------------------------------------+

#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE