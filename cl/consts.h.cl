//---------------------------------------------------------+
// fff/cl/consts.h.cl
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
//!	\file		consts.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Some constants.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __consts_h_cl__included__
#define __consts_h_cl__included__

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


//! Zero sample value
#define fff_SMP_ZERO \
	((Sample)0)

//! One sample value
#define fff_SMP_ONE \
	((Sample)1)

//! PI as sample value
#define fff_M_PI \
	((Sample)3.14159265358979323846)

//! typeless zero
#define fff_NULL \
	0

_fff_END_NAMESPACE
_fff_END_NAMESPACE


#endif