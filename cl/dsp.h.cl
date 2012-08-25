//---------------------------------------------------------+
// fff/cl/dsp.h.cl
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
//!	\file		dsp.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Digital signal processing.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __dsp_h_cl__included__
#define __dsp_h_cl__included__

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../include/_intern.h"
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

#define _I0_M			5
#define _KAISER_ALPHA	4

Sample _kaiserWindow(
	UInt n,
	UInt kernel_size,
	Sample alpha
	);

Sample _I0(
	Sample x
	);
#endif

_fff_END_NAMESPACE
_fff_END_NAMESPACE

#endif