/*----------------------------------------------------------
 *    The Fast Filtering Framework implements an LTI filter
 *    with Khronos Group's OpenCL.
 *    Copyright (C) 2012  Philipp Renoth
 *----------------------------------------------------------
 *    This program is free software: you can redistribute
 *    it and/or modify it under the terms of the
 *    GNU General Public License as published by the
 *    Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the
 *    GNU General Public License along with this program.
 *    If not, see <http://www.gnu.org/licenses/>.
 *--------------------------------------------------------*/

//----------------------------------------------------------
#include "../cl/fffConvolution.h.cl"
//----------------------------------------------------------



//----------------------------------------------------------
_fff_BEGIN_NAMESPACE
//----------------------------------------------------------


//----------------------------------------------------------
	UInt _bit_reversal(
		UInt x, 
		UInt w)
	{
		UInt out=0; // bit reversal result

		// go through each bit
		//! \todo check if \c{width >> 1} works
		for(UInt i=0; i < ( (w+1) >> 1); ++i)
			out |= _fff_BITSWAPONLY(
				x, i, w);
		
		return
			out;
	}
//----------------------------------------------------------

//----------------------------------------------------------
	UInt _next_bin_step(
		UInt n)
	{
		return
			1<<_next_lb2_step(n);
	}
//----------------------------------------------------------

//----------------------------------------------------------
	UInt _next_lb2_step(
		UInt n)
	{
		//! \bug for n=0
		--n;
		UInt b = 0;
		while(n)
		{
			n >>= 1;
			++b;
		}
		return b;
	}
//----------------------------------------------------------

//----------------------------------------------------------
	UInt _calcOverlapSaveSize(
		UInt inputSampleCount,
		UInt lb2FftSampleCount,
		UInt kernelSampleCount)
	{
		UInt fftSampleCount =
			1<<lb2FftSampleCount;

		// state is the kernel sample count minus one
		UInt stateSampleCount =
			kernelSampleCount - 1;

		// throughput is the fft sample count without
		// the state sample count
		UInt throughputSampleCount =
			fftSampleCount -
			stateSampleCount;
		
		// Let \em p be the thoughput percentage.
		// If there are N input samples we get
		// \f$ M = N / p, M \ge N\f$
		UInt fftBlockCount =
			_fff_CEILDIV(
				inputSampleCount,
				throughputSampleCount);

		// since only full ffts can be calculated
		// this is additionally overhead
		return
			fftBlockCount * fftSampleCount;
	}
//----------------------------------------------------------

//----------------------------------------------------------
	void _calcOverlapSaveOffset(
		UInt *fromIndex,
		UInt *toIndex,
		UInt fftGroupNum,
		UInt itemIndex,
		UInt lb2FftSampleCount,
		UInt kernelSampleCount,
		UInt dataSampleCount
		)
	{
		UInt fftSampleCount =
			1<<lb2FftSampleCount;

		UInt stateSampleCount =
			kernelSampleCount - 1;

		UInt throughputSampleCount =
			fftSampleCount - (kernelSampleCount - 1);

		// corresponding swap partner
		UInt groupIndex = 
			_bit_reversal(itemIndex, lb2FftSampleCount);

		// base offset of the group
		UInt groupOffset =
			fftGroupNum * throughputSampleCount;
		
		// origin data index
		(*fromIndex) =
			groupOffset + groupIndex;

		// linearized from array with
		// redundant overlappings
		(*toIndex) = itemIndex +
			fftGroupNum * fftSampleCount;

	}
//----------------------------------------------------------

//----------------------------------------------------------
	void _fft_butterfly(
		UInt *firstIndex,
		UInt *secondIndex,
		Sample *twiddleExpPiFactor,
		UInt layerNum,
		UInt butterflyNum
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
			//* (crossingButterfliesCount<<1)
			// \b Optimization:
			<< (layerNum<<1))
			// plus the group intern offset
			+ groupButterflyNum;

		(*firstIndex) = tmpFirstIndex;

		// distance of two indices of one butterfly
		// are one item each crossing butterfly
		(*secondIndex) = tmpFirstIndex +
			crossingButterfliesCount;

		// twiddle \c w factor of the complex
		// euler function. \f$ e^{w \cdot \pi}
		(*twiddleExpPiFactor) =
			(Sample)groupButterflyNum /
			(Sample)crossingButterfliesCount;
	}
//----------------------------------------------------------

//----------------------------------------------------------
	UInt _factorial(
			UInt n
			)
	{
		UInt res=1;
		while(n-->1)
			res*=n;
		return res;
	}
//----------------------------------------------------------







	#ifdef _fff_IS_DEV


		/* debugging */
	#	ifdef fff_DEBUGKERNEL


			/* locks a mutex */
			void __fff_dbg_mutex_lock(
				__global struct fffDebugKernel *debugKernel
				)
			{
				/* wait for changing MUTEXRELEASE
					to MUTEXLOCK to get the lock */
				while(
					atomic_xchg(
						&debugKernel->write_mutex,
						__fff_DBG_MUTEXLOCK
					) == __fff_DBG_MUTEXRELEASE)
				{
				}

			}

			/* release a mutex */
			void __fff_dbg_mutex_release(
				__global struct fffDebugKernel *debugKernel
				)
			{
				atomic_xchg(
					&debugKernel->write_mutex,
					__fff_DBG_MUTEXRELEASE
				);
			}



			Int __fff_dbg_err_raise(
				__global struct fffDebugKernel *debugKernel,
				Int err,
				UInt linenum
				)
			{
				__fff_dbg_mutex_lock(
					debugKernel
					);

				if(debugKernel->errnum == fff_ERR_NONE)
				{
					debugKernel->errnum = err;
					debugKernel->linenum = linenum;
					debugKernel->global_id = get_global_id(0);
					debugKernel->local_id = get_local_id(0);
					debugKernel->group_id = get_group_id(0);

					debugKernel->causal_errors = 0;
				}
				else
					debugKernel->causal_errors++;

				__fff_dbg_mutex_release(
					debugKernel
					);

				return err;
			}

			/* creates a global barrier, then
				fetch current error number.
				Is fff_ERR_NONE, if no error occuried */
			Int __fff_dbg_err_barrier(
				__global struct fffDebugKernel *debugKernel
				)
			{
				/* synchronize all worker to stop here */
				barrier(
					CLK_GLOBAL_MEM_FENCE
					);

				/* readonly synchronized */
				return debugKernel->errnum;
			}

	#	endif



//----------------------------------------------------------
		void _cmul(
			Sample *real,
			Sample *imag,
			Sample complex_exponent,
			Sample factor_real,
			Sample factor_imag
			)
		{
			Sample mul_imag, mul_real;
			mul_imag = sincos(complex_exponent, &mul_real);

			(*real) = factor_real*mul_real - factor_imag*mul_imag;
			(*imag) = factor_real*mul_imag + factor_imag*mul_real;
		}
//----------------------------------------------------------
		
//----------------------------------------------------------
		Sample _kaiser_window(
			UInt n,
			UInt kernel_size,
			Sample alpha
			)
		{
			Sample nom, nom_part, denom, alphaPi;

			alphaPi = alpha * _fff_M_PI;

			nom_part = 
				(Sample)(n<<1)/
					(Sample)kernel_size -
				(Sample)1;
			nom = alphaPi * sqrt(
				(Sample)1 - 
					nom_part*nom_part);

			denom = alphaPi;

			return
				_i0(nom) /
				_i0(denom);
		}
//----------------------------------------------------------

//----------------------------------------------------------
		Sample _fff_i0(
				Sample x
				)
		{
			Sample x2 =
				x / (Sample)2;

			Sample sum = 0;

			for(
				UInt m = 0;
				m < _fff_I0_M;
				++m)
			{
				UInt d =
					_fff_factorial(m);

				//   m!*tgamma(m+1)
				// = m!*m!*(m+1)
				d = d*d*(m+1);

				Sample fract =
					pown(x, m<<1) / d;

				sum += fract;
			}

			return sum;

		}
//----------------------------------------------------------
	

//----------------------------------------------------------
		void __fft_perform(
			__global Sample *arranged_real,
			__global Sample *arranged_imag,
			Sample pi_direction,

			UInt butterfly_start,
			UInt butterfly_jump
			)
		{
			for(
				UInt layer = 0;
				layer < _fff_LB2_FFT_SIZE;
				++layer
				)
			{
				for(
					UInt butterfly = butterfly_start;
					butterfly < (_fff_FFT_SIZE >> 1);
					butterfly += butterfly_jump
					)
				{
					UInt first, second;
					Sample exponent;
					_fft_butterfly(
						&first,
						&second,
						&exponent,
						layer,
						butterfly
						);

					Sample c_real, c_imag;
					Sample x0_real, x0_imag;
					Sample x1_real, x1_imag;

					x0_real = arranged_real[first];
					x0_imag = arranged_imag[first];
					x1_real = arranged_real[second];
					x1_imag = arranged_imag[second];

					_fff_cmul(
						&c_real,
						&c_imag,
						pi_direction * exponent,
						x1_real,
						x1_imag
						);

					// butterfly
					// y0 = x0 + t*x1
					// y1 = x0 - t*x1
					arranged_real[first] = x0_real + c_real;
					arranged_imag[first] = x0_imag + c_imag;

					arranged_real[second] = x0_real - c_real;
					arranged_imag[second] = x0_imag - c_imag;

				}
			
			

				fff_GLOBAL_BARRIER();
			
			}
		}
//----------------------------------------------------------

//----------------------------------------------------------
		void _fft(
			__global Sample *arranged_real,
			__global Sample *arranged_imag,
					UInt butterfly_start,
					UInt butterfly_jump
			)
		{
			__fft_perform(
				arranged_real,
				arranged_imag,
				-_fff_M_PI,		// MINUS PI !!!
				butterfly_start,
				butterfly_jump
				);
		}
//----------------------------------------------------------

//----------------------------------------------------------
		void _ifft(
			__global Sample *arranged_real,
			__global Sample *arranged_imag,
					UInt butterfly_start,
					UInt butterfly_jump
			)
		{
			__fft_perform(
				arranged_real,
				arranged_imag,
				_fff_M_PI,		// PLUS PI !!!
				butterfly_start,
				butterfly_jump
   			);
		}
//----------------------------------------------------------

//----------------------------------------------------------
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

			)
		{
			// zeros
			for(
				UInt gid = fff_GLOBAL_ID();
				gid < _fff_FFT_SIZE;
				gid += fff_GLOBAL_SIZE())
			{
				UInt from = _bit_reversal(
					gid,
					_fff_LB2_FFT_SIZE);

				if(from < _fff_KERNEL_SIZE)
				{
					g_re_H[gid] = g_re_b[from];
					g_im_H[gid] = g_im_b[from];
					g_re_tmp[gid] = g_re_a[from];
					g_im_tmp[gid] = g_im_a[from];
				}
				else
				{
					g_re_H[gid] = _fff_SMP_ZERO;
					g_im_H[gid] = _fff_SMP_ZERO;
					g_re_tmp[gid] = _fff_SMP_ZERO;
					g_im_tmp[gid] = _fff_SMP_ZERO;
				}
			}

			fff_GLOBAL_BARRIER();
		
			_fff_fft(
				g_re_H,
				g_im_H,
				fff_GLOBAL_ID(),
				fff_GLOBAL_SIZE()
				);

			_fff_fft(
				g_re_tmp,
				g_im_tmp,
				fff_GLOBAL_ID(),
				fff_GLOBAL_SIZE()
				);

			fff_GLOBAL_BARRIER();

			// H = Hb / Ha (complex division !!!)
			for(
				UInt gid = fff_GLOBAL_ID();
				gid < _fff_FFT_SIZE;
				gid += fff_GLOBAL_SIZE())
			{
				Sample denom =
					g_re_tmp[gid]*g_re_tmp[gid] +
					g_im_tmp[gid]*g_im_tmp[gid];

				Sample nom1 =
					g_re_H[gid]*g_re_tmp[gid] +
					g_im_H[gid]*g_im_tmp[gid];

				Sample nom2 =
					g_im_H[gid]*g_re_tmp[gid] -
					g_re_H[gid]*g_im_tmp[gid];

				g_re_H[gid] = nom1 / denom;
				g_im_H[gid] = nom2 / denom;

			}
		}
//----------------------------------------------------------

//----------------------------------------------------------
		/* fast convolution kernel */
		__kernel void fff_KERNEL_FCONV(

			/* size is data_size */
			__global	const Sample	*g_x_real,
			__global	const Sample	*g_x_imag,
					UInt	data_size,

			/* size is _fff_FFT_SIZE */
			__global	const Sample	*g_H_real,
			__global	const Sample	*g_H_imag,

			/* size is the correct size for overlap save algorithm */
			__global	Sample	*g_uninit_tmp_real,
			__global	Sample	*g_uninit_tmp_imag,

			__global	Sample	*g_uninit_tmp2_real,
			__global	Sample	*g_uninit_tmp2_imag,

			/* size is data_size */
			__global	Sample	*g_uninit_y_real,
			__global	Sample	*g_uninit_y_imag

			)
		{
			//UInt tmpSize = fff_calcOverlapSaveSize(data_size, fff_LB2_FFT_SIZE, kernel_size);
			//if(fff_GROUP_ID()>2)return;
		
		
			UInt
				unaliasThroughputSize	= _fff_FFT_SIZE - _fff_STATE_SIZE,			// not corrupted samples
				offset					= fff_GROUP_ID() * _fff_FFT_SIZE,	// block offset
				unaliasOutputSize		= data_size - _fff_STATE_SIZE;				// not corrupted output samples

			__global Sample
				*x_real					= g_uninit_tmp_real  + offset,	//  fft-Block real
				*x_imag					= g_uninit_tmp_imag  + offset,  //  fft-Block imag
				*x2_real				= g_uninit_tmp2_real + offset,  // ifft-Block real
				*x2_imag				= g_uninit_tmp2_imag + offset;  // ifft-Block imag

			for(
				UInt fftBlockItem = fff_LOCAL_ID();
				fftBlockItem < _fff_FFT_SIZE;
				fftBlockItem += fff_LOCAL_SIZE()
				)
			{
			
				UInt fromIndex, toIndex;

				_calcOverlapSaveOffset(
					&fromIndex,
					&toIndex,
					fff_GROUP_ID(),
					fftBlockItem,
					_fff_LB2_FFT_SIZE,
					_fff_KERNEL_SIZE,
					data_size
					);

				if(fromIndex < data_size)
				{
					g_uninit_tmp_real[toIndex] = g_x_real[fromIndex];
					g_uninit_tmp_imag[toIndex] = g_x_imag[fromIndex];
				}
				else
				{
					g_uninit_tmp_real[toIndex] = _fff_SMP_ZERO;
					g_uninit_tmp_imag[toIndex] = _fff_SMP_ZERO;
				}
			}
		
			fff_GLOBAL_BARRIER();
		
			_fff_fft(
				x_real,
				x_imag,
				fff_LOCAL_ID(),
				fff_LOCAL_SIZE()
				);
		
			fff_GLOBAL_BARRIER();

			for(UInt i = fff_LOCAL_ID();
				i < _fff_FFT_SIZE;
				i += fff_LOCAL_SIZE()
				)
			{
				Sample x_re, x_im;
				Sample x_real_tmp, x_imag_tmp;

				x_re = x_real[i] * _fff_FFT_SIZE;
				x_im = x_imag[i] * _fff_FFT_SIZE;

				x_real_tmp = 
					(x_re * g_H_real[i] -
					x_im * g_H_imag[i]);
				x_imag_tmp =
					(x_re * g_H_imag[i] +
					x_im * g_H_real[i]);

				x_real[i] = x_real_tmp;
				x_imag[i] = x_imag_tmp;

			}
        
			fff_GLOBAL_BARRIER();
		

			for(
				UInt fftItem = fff_LOCAL_ID();
				fftItem < _fff_FFT_SIZE;
				fftItem += fff_LOCAL_SIZE())
			{
				UInt to = _bit_reversal(
					fftItem,
					_fff_LB2_FFT_SIZE);
			
				x2_real[to] = x_real[fftItem];
				x2_imag[to] = x_imag[fftItem];
			
			}

			fff_GLOBAL_BARRIER();

		
			_fff_ifft(
				x2_real,
				x2_imag,
				fff_LOCAL_ID(),
				fff_LOCAL_SIZE()
				);
		
			fff_GLOBAL_BARRIER();
		
			for(UInt i = fff_LOCAL_ID();
				i < unaliasThroughputSize;
				i += fff_LOCAL_SIZE())
			{
				UInt to;
				to = (fff_GROUP_ID() *
					unaliasThroughputSize) +
					i;

				if( to < unaliasOutputSize)
				{
					g_uninit_y_real[to] =
							x2_real[_fff_STATE_SIZE+i] / (Sample)(_fff_FFT_SIZE*_fff_KERNEL_SIZE);
					g_uninit_y_imag[to] =
							x2_imag[_fff_STATE_SIZE+i] / (Sample)(_fff_FFT_SIZE*_fff_KERNEL_SIZE);
				}
			}
		}
//----------------------------------------------------------

		
	#endif /* ifdef _fff_OPENCLCODE */

_fff_END_NAMESPACE