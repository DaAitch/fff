
/****************************************************************************************/
/**
  * Host and device interface definitions.
  *
  * Since it's better/easier to have one opencl file to compile
  * the opencl kernels and data types of host and device are
  * defined within this file.
  * 
  * Preprocessor conventions:
  *		- fff_IS_HOST has to be defined with 1 to gather the host code,
  *			else if 0 opencl definitions with the kernels,
  *			if not set a compile error appears
  *		- fff_SAMPLE_DATATYPE has to be defined with the preferred
  *			data type for processing (float, double)
  *		- (optional) fff_KERNELDEBUG has to be defined whether each
  *			kernel returns an array of error codes of the workers
  *			to debug them
  * 
  * Access path conventions:
  *		- fff_* can be accessed from every path
  *			interface for remote project
  *		- _fff_* should only be accessed from fff_* paths
  *			e.g. fff-own debugging, helper methods
  *		- __fff_* should only be accessed from _fff_* paths
  *			e.g. intern debugging, low-level helper methods
  * 
  * Other conventions:
  *		- Since all the code should be OpenCL C99 style there must not be
  *			any references (e.g. &variable), but only pointers (e.g. *variable)
  * 
  *	Author:		Philipp Renoth <renoth@6ft.us>
  * Date:		2012-01-12
  *
  *
  *
  *
  *
  *
  *
  */
/****************************************************************************************/

#ifndef __fffconvolution_h_cl__included__
#	define __fffconvolution_h_cl__included__


#	include "../include/_fffBase.h"

/****************************************************************************************/
/* Check preprocessor definitions *******************************************************/
/****************************************************************************************/


/****************************************************************************************/
/* Begin with file definitions **********************************************************/
/****************************************************************************************/




	_fff_BEGIN_NAMESPACE


	
	
#	define _fff_UI_ZERO			((UInt)0)
#	define _fff_UI_ONE			((UInt)1)
#	define _fff_SMP_ZERO			((Sample)0)
#	define _fff_SMP_ONE			((Sample)1)
#	define _fff_M_PI				((Sample)3.14159265358979323846)
#	define _fff_NULL				0

/****************************************************************************************/
#	define _fff_NTH_BIT(N)														\
		(_fff_UI_ONE << (N))



	/* integer division: divide X by N with respect to the remainder */
#	define _fff_CEILDIV(X, N)													\
		(( (X) + (N)-_fff_UI_ONE ) / (N))

#	define _fff_BITMASK(M)														\
		(_fff_NTH_BIT(M))

#	define _fff_BITSWAPONLY(X,B,N)												\
		(	(((X)&_fff_BITMASK(B))						<<						\
				((N)-((B)<<_fff_UI_ONE)-_fff_UI_ONE)) |							\
			(((X)&_fff_BITMASK((N)-(B)-_fff_UI_ONE))		>>						\
				((N)-((B)<<_fff_UI_ONE)-_fff_UI_ONE)) )

	/* swap all bits according to the length */
	UInt _fff_bit_reversal(
		const UInt bits, 
		const UInt width);



	UInt _fff_calcOverlapSaveSize(
		UInt input_size,
		UInt lb2_fft_size,
		UInt kernel_size);

	
	UInt _fff_next_bin_step(
		UInt n);

	UInt _fff_next_lb2_step(
		UInt n);


	/*
		For overlapped redundant copy for performing efficient ffts.
		Overlapping size is the state of the FIR. The state size of
		a FIR is the kernel size (=transfer function) minus one, because
		the first item in the kernel refers to the time-current sample,
		and the other kernel item refer to the "kernel_size-1" past
		samples.

		(Also see the test)

		Difference to the overlap-add method is, that this method
		is using enough samples not to get aliasing artifacts with
		a smaller fft-throughput. The overlap-add method has overlapped
		areas to be added, that aliasing together gets the right value.
		Since OpenCL is better to have parallel operations this
		method is more fft-work but no linear adding after.
	*/
	void _fff_calcOverlapSaveOffset(
		UInt  *from_index,		/* global from element index */
		UInt  *to_index,			/* global to element index */
		UInt fft_group,			/* group of the fft */
		UInt item_index,		/* item index of the group */
		UInt lb2_fftSize,		/* log_2(N), N=fff_FFT_SIZE */
		UInt kernel_size,		/* size of the kernel */
		UInt data_size
		);

	void _fff_fft_butterfly(
		UInt *elem_first,			/* first element of the butterfly */
		UInt *elem_second,			/* second element of th butterfly */
		Sample *euler_piFactorExponent,	/* complex euler exponent of the
												factory PI: e^^(pi*euler_piFactorExponent) */
		UInt layer,					/* layer of fft iteration, starts at 0 */
		UInt butterfly				/* number of the butterfly */
		);


	UInt _fff_factorial(
		UInt n
		);


	/*
	
	*/
	
	
	

	/* host and opencl code */

#	define fff_KERNEL_TFUNC		fff_tfunc
#	define fff_KERNEL_FCONV		fff_fconv


#	ifdef _fff_LB2_FFT_SIZE
#		define _fff_FFT_SIZE														\
			(_fff_NTH_BIT(_fff_LB2_FFT_SIZE))
#	endif /* ifdef _fff_LB2_FFT_SIZE */

#	ifdef _fff_KERNEL_SIZE
#		define _fff_STATE_SIZE														\
			((_fff_KERNEL_SIZE)-1)
#	endif /* ifdef _fff_KERNEL_SIZE */


	/* kernels and helper functions */
#	ifdef _fff_IS_DEV

#		ifndef _fff_LB2_FFT_SIZE
#			error _fff_LB2_FFT_SIZE not defined!
#		endif /* ifdef fff_LB2_FFT_SIZE */

#		ifndef _fff_KERNEL_SIZE
#			error _fff_KERNEL_SIZE not defined!
#		endif

#		define fff_LOCAL_BARRIER() \
			barrier(CLK_LOCAL_MEM_FENCE)
#		define fff_GLOBAL_BARRIER() \
			barrier(CLK_GLOBAL_MEM_FENCE)

#		define fff_LOCAL_ID() \
			get_local_id(0)
#		define fff_LOCAL_SIZE() \
			get_local_size(0)
#		define fff_GLOBAL_ID() \
			get_global_id(0)
#		define fff_GLOBAL_SIZE() \
			get_global_size(0)
#		define fff_GROUP_ID() \
			get_group_id(0)
#		define fff_NUM_GROUPS() \
			get_num_groups(0)




#		define _fff_I0_M			5
#		define _fff_KAISER_ALPHA	4

		Sample _fff_kaiser_window(
			UInt n,
			UInt kernel_size,
			Sample alpha
			);

		Sample _fff_i0(
			Sample x
			);

		


		void _fff_cmul(
			Sample *real,
			Sample *imag,
			Sample complex_exponent,
			Sample factor_real,
			Sample factor_imag
			);

		

		void __fff_fft_perform(
			__global Sample *arranged_real,
			__global Sample *arranged_imag,
			Sample pi_direction,

            UInt butterfly_start,
            UInt butterfly_jump
			);

		void _fff_fft(
			__global Sample *arranged_real,
			__global Sample *arranged_imag,
			UInt butterfly_start,
            UInt butterfly_jump
			);

		void _fff_ifft(
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


		
#	endif /* ifdef _fff_IS_DEV */



	_fff_END_NAMESPACE

#endif /* ifndef __fffconv_h_cl__included__ */