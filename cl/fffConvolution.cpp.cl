
#include "../cl/fffConvolution.h.cl"


UInt _fff_NAMESPACE_PREFIX _fff_bit_reversal(
	const UInt bits, 
	const UInt width)
{
	UInt out=0;

	for(UInt i=0; i < ( (width+1) >> 1); ++i)
		out |= _fff_BITSWAPONLY(bits, i, width);
	
	return out;
}

UInt _fff_NAMESPACE_PREFIX _fff_calcOverlapSaveSize(
		UInt input_size,
		UInt lb2_fft_size,
		UInt kernel_size)
{
	UInt state =
		kernel_size - 1;
	UInt throughPut =
		(1<<lb2_fft_size) -
		state;

	UInt ffts =
		_fff_CEILDIV(
			input_size,
			throughPut);

	return ffts *
		(1<<lb2_fft_size);
}

UInt _fff_NAMESPACE_PREFIX _fff_next_bin_step(
	UInt n)
{
	return _fff_NTH_BIT(
		_fff_next_lb2_step(n));
}

UInt _fff_NAMESPACE_PREFIX _fff_next_lb2_step(
	UInt n)
{
	--n;
	UInt b = 0;
	while(n)
	{
		n >>= 1;
		++b;
	}
	return b;
}


void _fff_NAMESPACE_PREFIX _fff_calcOverlapSaveOffset(
	UInt  *from_index,		/* global from element index */
	UInt  *to_index,		/* global to element index */
	UInt fft_group,		/* group of the fft */
	UInt item_index,		/* item index of the group */
	UInt lb2_fftSize,		/* lb(N), N=FFT_SIZE */
	UInt kernel_size,		/* size of the kernel */
	UInt data_size
	)
{
	// For calculating the bit reversal
	// we need the lb(N) value. Calculating
	// the N has runtime O(1) with bitshifting.
	// The other way round calculating lb(N) of
	// N needs to be calculating iterative.
	UInt fftSize =
		_fff_NTH_BIT(lb2_fftSize);

	// Since the first kernel item is refering to the time-
	// current sample of the input, this filter is a
	// "kernel_size-1" state filter, though "kernel_size" order.
	UInt stateSize =
		kernel_size - 1;

	// Each iteration of the complete fft the throughput size
	// is the FFT_SIZE without the first "stateSize" samples.
	// That is because first sample of the fft depends on the
	// "stateSize" past samples.
	UInt fftThroughputSize =
		fftSize - stateSize;

	// Index in the group
	UInt groupIndex = 
		_fff_bit_reversal(item_index, lb2_fftSize);

	// Offset for the groups on the left
	UInt groupOffset =
		fft_group * fftThroughputSize;

	// theoretical max. group size
	/*UInt theoMaxGroupSize =
		data_size - groupOffset;*/
	
	// if there is less data than we need
	// to compute the fft, we need to
	// cyclicly pad data
	/*groupIndex %= theoMaxGroupSize;*/
		
	// From index is the bit reversal of the index item
	// padding right for kernel (=state of the fir)
	// at the specific fft group
	(*from_index) =
		groupOffset + groupIndex;

	// Linearized from array with redundant overlappings
	(*to_index) = item_index +
		fft_group * fftSize;

}

void _fff_NAMESPACE_PREFIX _fff_fft_butterfly(
	UInt *elem_first,			/* first element of the butterfly */
	UInt *elem_second,			/* second element of the butterfly */
	Sample *euler_piFactorExponent,	/* complex euler exponent of the
											product PI: e^^(2*pi*euler_piFactorExponent) */
	UInt layer,					/* layer of fft iteration, starts at 0 */
	UInt butterfly				/* number of the buttefly */
	)
{
	UInt crossingButterflies =
		_fff_NTH_BIT(layer);
	UInt group =
		butterfly >> layer; // butterfly / crossingButterflies
	UInt groupButterfly =
		butterfly % crossingButterflies;

	UInt first =
		group * (crossingButterflies<<1) +
		groupButterfly;

	(*elem_first) = first;
	(*elem_second) = first + crossingButterflies;

	(*euler_piFactorExponent) =
		(Sample)groupButterfly /
		(Sample)crossingButterflies;
}

UInt _fff_NAMESPACE_PREFIX _fff_factorial(
		UInt n
		)
{
	UInt res=1;
	while(n-->1)
		res*=n;
	return res;
}








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




	void _fff_cmul(
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

		
	Sample _fff_kaiser_window(
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
			_fff_i0(nom) /
			_fff_i0(denom);
	}

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

	


	void __fff_fft_perform(
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
				_fff_fft_butterfly(
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

    void _fff_fft(
		__global Sample *arranged_real,
		__global Sample *arranged_imag,
				UInt butterfly_start,
				UInt butterfly_jump
		)
	{
		__fff_fft_perform(
			arranged_real,
			arranged_imag,
			-_fff_M_PI,
			butterfly_start,
			butterfly_jump
			);
	}

	void _fff_ifft(
		__global Sample *arranged_real,
		__global Sample *arranged_imag,
				UInt butterfly_start,
				UInt butterfly_jump
		)
	{
		__fff_fft_perform(
			arranged_real,
			arranged_imag,
			_fff_M_PI,
			butterfly_start,
			butterfly_jump
   	    );
	}


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
			UInt from = _fff_bit_reversal(
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

			_fff_calcOverlapSaveOffset(
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
			UInt to = _fff_bit_reversal(
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


		
#endif /* ifdef _fff_OPENCLCODE */