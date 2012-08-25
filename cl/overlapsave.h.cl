//---------------------------------------------------------+
// fff/cl/overlapsave.h.cl
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
//!	\file		overlapsave.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		???
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __overlapsave_h_cl__included__
#define __overlapsave_h_cl__included__

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




UInt calcOverlapSaveSampleCount(
	UInt inputSampleCount,
	UInt lb2FftSampleCount,
	UInt kernelSampleCount);

//!
//!	For overlapped redundant copy for performing efficient ffts.
//!	Overlapping size is the state of the FIR. The state size of
//!	a FIR is the kernel size (=transfer function) minus one, because
//!	the first item in the kernel refers to the time-current sample,
//!	and the other kernel item refer to the "kernel_size-1" past
//!	samples.
//!
//!	(Also see the tests)
//!
//!	Difference to the overlap-add method is, that this method
//!	is using enough samples not to get aliasing artifacts with
//!	a smaller fft-throughput. The overlap-add method has overlapped
//!	areas to be added, that aliasing together gets the right value.
//!	Since OpenCL is better to have parallel operations this
//!	method is more fft-work but no linear adding after.
void calcOverlapSaveSampleOffset(
	UInt *fromIndex,	//!< [out]
	UInt *toIndex,		//!< [out]
	UInt  fftGroup,		//!< [in]
	UInt  itemIndex,	//!< [in]
	UInt  lb2FftSize,	//!< [in]
	UInt  kernelSize,	//!< [in]
	UInt  dataSize		//!< [in]
	);

_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif