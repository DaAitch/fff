//---------------------------------------------------------+
// fff/cl/fffConvolution.h.cl
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
//!	\file		fffConvolution.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Convolution OpenCL kernel and
//!				helper functions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
#ifndef __fffconvolution_h_cl__included__
#	define __fffconvolution_h_cl__included__


#	include "../include/_fffBase.h"




//---------------------------------------------------------+
	_fff_BEGIN_NAMESPACE
//---------------------------------------------------------+

//---------------------------------------------------------+
// Numerical constants
//---------------------------------------------------------+

		//! Zero sample value
#		define _fff_SMP_ZERO	((Sample)0)

		//! One sample value
#		define _fff_SMP_ONE		((Sample)1)

		//! PI as sample value
#		define _fff_M_PI    ((Sample)3.14159265358979323846)

		//! typeless zero
#		define _fff_NULL		0
//---------------------------------------------------------+

//---------------------------------------------------------+
// Some helper macros
//---------------------------------------------------------+


		//! calculates \f$ 2^N \f$
#		define _fff_NTH_BIT(N) \
			(1<<(N))

		//! Ceiling division, e.g \f$ 100 / 99 = 2 \f$
#		define _fff_CEILDIV(X, N) \
			(((X)+(N)-1)/(N))

		//! Bit a position M
#		define _fff_BITMASK(M) \
			(_fff_NTH_BIT(M))

		//! Swap two bits of \em X, with width \em W and
		//! at position \em P
#		define _fff_BITSWAPONLY(X,P,W) \
			(	(((X)&_fff_BITMASK(P))<< \
					((W)-((P)<<1)-1)) |	\
				(((X)&_fff_BITMASK((W)-(P)-1))>> \
					((W)-((P)<<1)-1)) )

		//! Swap all bits, equals reversal operation.
		//! \param x Bits to swap
		//! \param w Width in bits
		//! \return Bit reversal of the LSB \c w bits.
		//! \warning Other bits except \c w bits are zero.
		//!		Though no inplace swapping!
		UInt _bit_reversal(
			UInt x,
			UInt w);



		UInt _calcOverlapSaveSampleCount(
			UInt inputSampleCount,
			UInt lb2FftSampleCount,
			UInt kernelSampleCount);

	
		UInt _nextBinStep(
			UInt n);

		UInt _nextLb2Step(
			UInt n);


		/*
			For overlapped redundant copy for performing efficient ffts.
			Overlapping size is the state of the FIR. The state size of
			a FIR is the kernel size (=transfer function) minus one, because
			the first item in the kernel refers to the time-current sample,
			and the other kernel item refer to the "kernel_size-1" past
			samples.

			(Also see the tests)

			Difference to the overlap-add method is, that this method
			is using enough samples not to get aliasing artifacts with
			a smaller fft-throughput. The overlap-add method has overlapped
			areas to be added, that aliasing together gets the right value.
			Since OpenCL is better to have parallel operations this
			method is more fft-work but no linear adding after.
		*/
		void _calcOverlapSaveSampleOffset(
			UInt *fromIndex,
			UInt *toIndex,
			UInt  fftGroup,
			UInt  itemIndex,
			UInt  lb2FftSize,
			UInt  kernelSize,
			UInt  dataSize
			);


		void _fftButterfly(
			UInt   *firstIndex,
			UInt   *secondIndex,
			Sample *twiddleExpPiFactor,
			UInt    layerNum,
			UInt    butterflyNum
			);


		UInt _factorial(
			UInt n
			);


#		define fff_KERNEL_TFUNC		tfuncKernel
#		define fff_KERNEL_FCONV		fconvKernel


#		ifdef _fff_LB2_FFT_SIZE
#			define _fff_FFT_SIZE														\
				(_fff_NTH_BIT(_fff_LB2_FFT_SIZE))
#		endif /* ifdef _fff_LB2_FFT_SIZE */

#		ifdef _fff_KERNEL_SIZE
#			define _fff_STATE_SIZE														\
				((_fff_KERNEL_SIZE)-1)
#		endif /* ifdef _fff_KERNEL_SIZE */


		/* kernels and helper functions */
#		ifdef _fff_IS_DEV

#			ifndef _fff_LB2_FFT_SIZE
#				error _fff_LB2_FFT_SIZE not defined!
#			endif /* ifdef fff_LB2_FFT_SIZE */

#			ifndef _fff_KERNEL_SIZE
#				error _fff_KERNEL_SIZE not defined!
#			endif




#			define _fff_I0_M			5
#			define _fff_KAISER_ALPHA	4

			Sample _kaiserWindow(
				UInt n,
				UInt kernel_size,
				Sample alpha
				);

			Sample _I0(
				Sample x
				);

		


			void _cmul(
				Sample *real,
				Sample *imag,
				Sample complex_exponent,
				Sample factor_real,
				Sample factor_imag
				);

		
//! \todo instead of swapping, change butterfly computation
//!			order to get spectral misordered presentation,
//!			but a valid multiplication with the transfer
//!			function and a correct inverse fft.
			void __fftPerform(
				__global Sample *arranged_real,
				__global Sample *arranged_imag,
				Sample pi_direction,

				UInt butterfly_start,
				UInt butterfly_jump
				);

			void _fft(
				__global Sample *arranged_real,
				__global Sample *arranged_imag,
				UInt butterfly_start,
				UInt butterfly_jump
				);

			void _ifft(
				__global Sample *arranged_real,
				__global Sample *arranged_imag,
				UInt butterfly_start,
				UInt butterfly_jump
				);


			/* calculating transfer function offline */
			__kernel void fff_KERNEL_TFUNC(

				__global	const Sample	*g_re_b,
				__global	const Sample	*g_im_b,
		
				/* size is kernel_size */
				__global	const Sample	*g_re_a,
				__global	const Sample	*g_im_a,

				/* size is _fff_FFT_SIZE */
				__global	Sample	*g_re_tmp,
				__global	Sample	*g_im_tmp,

				__global	Sample	*g_re_H,
				__global	Sample	*g_im_H

				);

		
			/* fast convolution kernel */
			__kernel void fff_KERNEL_FCONV(

				/* size is data_size */
				__global	const Sample	*g_x_real,
				__global	const Sample	*g_x_imag,
							UInt		data_size,

				/* size is _fff_FFT_SIZE */
				__global	const Sample	*g_H_real,
				__global	const Sample	*g_H_imag,

				/* size is the correct size for overlap save algorithm */
				__global	Sample	*g_uninit_tmp_real,
				__global	Sample	*g_uninit_tmp_imag,

				__global	Sample	*g_uninit_tmp2_real,
				__global	Sample	*g_uninit_tmp2_imag,

				/* size is data_size - fff_STATE_SIZE */
				__global	Sample	*g_uninit_y_real,
				__global	Sample	*g_uninit_y_imag

				);


		
#		endif /* ifdef _fff_IS_DEV */

	_fff_END_NAMESPACE

#endif /* ifndef __fffconvolution_h_cl__included__ */