//---------------------------------------------------------+
// fff/cl/fft.cpp.cl
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
//!	\file		fft.cpp.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Fast Fourier Transformation.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
#include "fff.h.cl"
#include "math.h.cl"
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
	

void _fftButterfly(
	UInt   *firstIndex,
	UInt   *secondIndex,
	Sample *twiddleExpPiFactor,
	UInt    layerNum,
	UInt    butterflyNum
	)
{
	// Participating crossing butterflies
	// in the corresponding layer.
	// "1" means no crossing.
	UInt crossingButterfliesCount =
		1<<layerNum;

	// Group of the butterfly is the butterfly
	// number divided by the crossing butterflies.
	// Optimized: Shift, no division.
	UInt groupNum =
		butterflyNum >> layerNum;

	// butterfly number in this group
	UInt groupButterflyNum =
		butterflyNum % crossingButterfliesCount;

	// first item is after the crossing butterflies
	// in the specific group
	UInt tmpFirstIndex =
		// jump the first groups
		(groupNum
		// each group its crossing butterflies with
		// two items each butterfyl
		// Old:
		//* (crossingButterfliesCount<<1))
		// Optimization:
		<< (layerNum+1))
		// plus the group intern offset
		+ groupButterflyNum;

	(*firstIndex) = tmpFirstIndex;

	// distance of two indices of one butterfly
	// are one item each crossing butterfly
	(*secondIndex) = tmpFirstIndex +
		crossingButterfliesCount;

	// twiddle factor exponent w of the complex
	// euler function.
	(*twiddleExpPiFactor) =
		(Sample)groupButterflyNum /
		(Sample)crossingButterfliesCount;
}
//---------------------------------------------------------+



	
#ifdef _fff_IMPLEMENT_CLCODE
//---------------------------------------------------------+
void __fftPerform(
	__global Sample *xRe,
	__global Sample *xIm
    
	_fff_DONTOPTIMIZE_PARAMDEF

	)
{
#if __fff_UNROLL_FFT_LAYER
#pragma unroll
#endif
	for(
		UInt layer = 0;
		layer < _fff_OPTIMIZE_PARAM_LB2N;
		++layer
		)
	{
#if __fff_UNROLL_FFT_BUTTERFLY
#pragma unroll
#endif
        
		for(
			UInt
				butterfly = 0,
				s = get_local_id(0);
			butterfly < 1<<(_fff_OPTIMIZE_PARAM_LB2N-1-
			_fff_OPTIMIZE_PARAM_LB2W);
			++butterfly
			)
		{
			UInt first, second;
			Sample exp;
			_fftButterfly(
				&first,
				&second,
				&exp,
				layer,
				s
				);
                
			Sample
				cRe,  cIm,
				x0Re, x0Im,
				x1Re, x1Im;

			x0Re = xRe[first];
			x0Im = xIm[first];
			x1Re = xRe[second];
			x1Im = xIm[second];

			cmul(
				&cRe,
				&cIm,
				-fff_M_PI * exp,
				x1Re,
				x1Im
				);

			// butterfly
			// y0 = x0 + t*x1
			xRe[first] = x0Re + cRe;
			xIm[first] = x0Im + cIm;

			// y1 = x0 - t*x1
			xRe[second] = x0Re - cRe;
			xIm[second] = x0Im - cIm;

			s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);
            
		}
			
			

		barrier(CLK_GLOBAL_MEM_FENCE);
			
	}
}
//---------------------------------------------------------+

//---------------------------------------------------------+
void __ifftPerform(
	__global Sample *XRe,
	__global Sample *XIm

	_fff_DONTOPTIMIZE_PARAMDEF

	)
{



#if __fff_UNROLL_IFFT_LAYER
#pragma unroll
#endif
	for(
		UInt layer = 0;
		layer < _fff_OPTIMIZE_PARAM_LB2N;
		++layer
		)
	{
#if __fff_UNROLL_IFFT_BUTTERFLY
#pragma unroll
#endif

		for(
			UInt
				butterfly = 0,
				s = get_local_id(0);
			butterfly < 1<<(_fff_OPTIMIZE_PARAM_LB2N-1-
			_fff_OPTIMIZE_PARAM_LB2W);
			++butterfly
			)
		{
			UInt first, second;
			Sample exp;
			_fftButterfly(
				&first,
				&second,
				&exp,
#ifdef _fff_OPTIMIZE
				_fff_OPTIMIZE_PARAM_LB2N-layer,
#else
				layer,
#endif
				s
				);

			Sample
				cRe,  cIm,
				x0Re, x0Im,
				x1Re, x1Im;

			x0Re = XRe[first];
			x0Im = XIm[first];
			x1Re = XRe[second];
			x1Im = XIm[second];

			cmul(
				&cRe,
				&cIm,
				+fff_M_PI * exp,
				x1Re,
				x1Im
				);

			// butterfly
			// y0 = x0 + t*x1
			XRe[first] = x0Re + cRe;
			XIm[first] = x0Im + cIm;

			// y1 = x0 - t*x1
			XRe[second] = x0Re - cRe;
			XIm[second] = x0Im - cIm;

			s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);

		}
			
			

		barrier(CLK_GLOBAL_MEM_FENCE);
			
	}
}
//---------------------------------------------------------+

//---------------------------------------------------------+
		void _fft(
			__global Sample *xRe,
			__global Sample *xIm

			_fff_DONTOPTIMIZE_PARAMDEF

			)
		{
			__fftPerform(
				xRe,
				xIm

				_fff_DONTOPTIMIZE_PARAMLIST

				);

                
#if __fff_UNROLL_FFT_FACTOR
#pragma unroll
#endif
			for(
				UInt
					i = 0,
					s = get_local_id(0);
				i < 1<<(_fff_OPTIMIZE_PARAM_LB2N-_fff_OPTIMIZE_PARAM_LB2W);
				++i)
			{
				xRe[s] /= (Sample)(1<<_fff_OPTIMIZE_PARAM_LB2N);
				xIm[s] /= (Sample)(1<<_fff_OPTIMIZE_PARAM_LB2N);

				s+=(1<<_fff_OPTIMIZE_PARAM_LB2W);
			}

			barrier(CLK_GLOBAL_MEM_FENCE);
		}
//---------------------------------------------------------+

//---------------------------------------------------------+
		void _ifft(
			__global Sample *XRe,
			__global Sample *XIm

			_fff_DONTOPTIMIZE_PARAMDEF

			)
		{
			__ifftPerform(
				XRe,
				XIm

				_fff_DONTOPTIMIZE_PARAMLIST

   			);
		}
//---------------------------------------------------------+

//---------------------------------------------------------+
void _fftArrange(
		__global const Sample *xRe,
		__global const Sample *xIm,
		__global       Sample *yRe,
		__global       Sample *yIm

		_fff_DONTOPTIMIZE_PARAMDEF

	)
{

#if __fff_UNROLL_FFT_ARRANGE
#pragma unroll
#endif
	for(
		UInt
			i = 0,
			s = get_local_id(0);
		i < (1<<(_fff_OPTIMIZE_PARAM_LB2N-_fff_OPTIMIZE_PARAM_LB2W));
		++i)
	{
		UInt to = bit_reversal(
			s,
			_fff_OPTIMIZE_PARAM_LB2N);
			
		yRe[to] = xRe[s];
		yIm[to] = xIm[s];

		s += (1<<_fff_OPTIMIZE_PARAM_LB2W);
	}
    
	barrier(CLK_GLOBAL_MEM_FENCE);
}
//---------------------------------------------------------+




//---------------------------------------------------------+
__kernel void fff_KERNEL_FFT(

#ifdef _fff_DONTOPTIMIZE
	__global const Sample *xRe,
	__global const Sample *xIm,
#endif

	__global       Sample *XRe,
	__global       Sample *XIm

	_fff_DONTOPTIMIZE_PARAMDEF

	)
{
#ifdef _fff_DONTOPTIMIZE
		_fftArrange(
			xRe,
			xIm,
			XRe,
			XIm

			_fff_DONTOPTIMIZE_PARAMLIST

			);
#endif

		_fft(
			XRe,
			XIm

			_fff_DONTOPTIMIZE_PARAMLIST

			);
}

//---------------------------------------------------------+

//---------------------------------------------------------+
__kernel void fff_KERNEL_IFFT(

#ifdef _fff_DONTOPTIMIZE
	__global const Sample *XRe,
	__global const Sample *XIm,
#endif

	__global       Sample *xRe,
	__global       Sample *xIm

	_fff_DONTOPTIMIZE_PARAMDEF

	)
{

#ifdef _fff_DONTOPTIMIZE
		_fftArrange(
			XRe,
			XIm,
			xRe,
			xIm

			_fff_DONTOPTIMIZE_PARAMLIST

			);
#endif

		_ifft(
			xRe,
			xIm
			
			_fff_DONTOPTIMIZE_PARAMLIST

			);
}

//---------------------------------------------------------+

#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE