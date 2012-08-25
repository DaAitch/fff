//---------------------------------------------------------+
// fff/cl/overlapsave.cpp.cl
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
//!	\file		overlapsave.cpp.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		???
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
#include "fff.h.cl"
#include "math.h.cl"
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


//---------------------------------------------------------+
UInt calcOverlapSaveSampleCount(
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
		fff_CEILDIV(
			inputSampleCount,
			throughputSampleCount);

	// since only full ffts can be calculated
	// there is additionally overhead
	return
		fftBlockCount * fftSampleCount;
}
//---------------------------------------------------------+

//---------------------------------------------------------+
void calcOverlapSaveSampleOffset(
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
		fftSampleCount - stateSampleCount;

	// corresponding swap partner
	UInt groupIndex = 
		bit_reversal(itemIndex, lb2FftSampleCount);

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
//---------------------------------------------------------+

_fff_END_NAMESPACE
_fff_END_NAMESPACE