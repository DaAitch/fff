//---------------------------------------------------------+
// fff/include/_intern/_base.h
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
//!	\file		_base.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Central base definitions including extern
//!				libraries and distinguish whether to
//!				use Host code or Device code for
//!				file type definitions etc.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ___base_h__included__
#define ___base_h__included__

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
//---------------------------------------------------------+

//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
//---------------------------------------------------------+

#ifdef fff_USE_64
#error 64bit doubles does not work yet! Do not define fff_USE_64!
#endif

#ifndef _fff_IS_DEV

#if defined(_WIN32) || defined(_WIN64)
#define _fff_WIN
#else
#error Unknown OS not supported!
#endif


#ifdef _fff_WIN
#define fff_DS '\\'
#else
#define fff_DS '/'
#endif

#endif

#endif