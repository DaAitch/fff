//---------------------------------------------------------+
// fff/cl/dft.cpp.cl
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
//!	\file		dft.cpp.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		discrete fourier transformation
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
#include "fff.h.cl"
#include "consts.h.cl"
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

void __dftPerform(
	__global const Sample *xRe,
	__global const Sample *xIm,

	__global       Sample *yRe,
	__global       Sample *yIm,

	               Sample  piDir

	_fff_DONTOPTIMIZE_PARAMDEF

	)
{

#if __fff_UNROLL_DFT_OUTER
#pragma unroll
#endif
	for(
		UInt
			i = 0,
			s = get_global_id(0);
		i < (1<<(_fff_OPTIMIZE_PARAM_LB2N-_fff_OPTIMIZE_PARAM_LB2W));
		++i)
	{
		Sample sRe, sIm;

		sRe = (Sample)0;
		sIm = (Sample)0;

#if __fff_UNROLL_DFT_INNER
#pragma unroll
#endif
		for(
			UInt j = 0;
			j < (1<<_fff_OPTIMIZE_PARAM_LB2N);
			++j)
		{
			Sample aRe, aIm;

			aRe = (Sample)0;
			aIm = (Sample)0;

			cmul(
				&aRe,
				&aIm,
				piDir * (Sample)2 * (Sample)i * (Sample)j /
					(Sample)(1<<_fff_OPTIMIZE_PARAM_LB2N),
				xRe[j],
				xIm[j]);

			sRe += aRe;
			sIm += aIm;
		}

		yRe[i] = sRe;
		yIm[i] = sIm;

		s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);

	}

	barrier(CLK_GLOBAL_MEM_FENCE);
}

void _dft(
	__global const Sample *xRe,
	__global const Sample *xIm,

	__global       Sample *XRe,
	__global       Sample *XIm

	_fff_DONTOPTIMIZE_PARAMDEF

	)
{
	__dftPerform(
		xRe,
		xIm,
		XRe,
		XIm,
		-fff_M_PI

		_fff_DONTOPTIMIZE_PARAMLIST
		);

#if __fff_UNROLL_DFT_FACTOR
#pragma unroll
#endif
	for(
		UInt i = 0,
		s = get_global_id(0);
		i < (1<<(_fff_OPTIMIZE_PARAM_LB2N-_fff_OPTIMIZE_PARAM_LB2W));
		++i)
	{
		XRe[i] /= (Sample)(1<<_fff_OPTIMIZE_PARAM_LB2N);
		XIm[i] /= (Sample)(1<<_fff_OPTIMIZE_PARAM_LB2N);

		s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);
	}

	barrier(CLK_GLOBAL_MEM_FENCE);
}

void _idft(
	__global const Sample *XRe,
	__global const Sample *XIm,

	__global       Sample *xRe,
	__global       Sample *xIm

	_fff_DONTOPTIMIZE_PARAMDEF
	
	)
{
	__dftPerform(
		XRe,
		XIm,
		xRe,
		xIm,
		+fff_M_PI

		_fff_DONTOPTIMIZE_PARAMLIST
		);
}

__kernel	
void fff_KERNEL_DFT(
	__global const Sample *xRe,
	__global const Sample *xIm,

	__global       Sample *XRe,
	__global       Sample *XIm

	_fff_DONTOPTIMIZE_PARAMDEF

)
{
	_dft(
		xRe,
		xIm,
		XRe,
		XIm

		_fff_DONTOPTIMIZE_PARAMLIST
		);
}

__kernel
void fff_KERNEL_IDFT(
	__global const Sample *XRe,
	__global const Sample *XIm,

	__global       Sample *xRe,
	__global       Sample *xIm

	_fff_DONTOPTIMIZE_PARAMDEF

)
{
	_idft(
		XRe,
		XIm,
		xRe,
		xIm

		_fff_DONTOPTIMIZE_PARAMLIST
		);
}

#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE