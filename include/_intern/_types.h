//---------------------------------------------------------+
// fff/include/_intern/__types.h
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
//!	\file		__types.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Central type definitions (also OpenCL).
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ___types_h__included__
#define ___types_h__included__

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "_ns.h"
//---------------------------------------------------------+

//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
//---------------------------------------------------------+

#ifndef _fff_IS_DEV

#include <CL/cl.hpp>
_fff_BEGIN_NAMESPACE
typedef cl_bool	Bool;
typedef cl_char	Char;
typedef cl_uchar	UChar;
typedef cl_short	Short;
typedef cl_ushort	UShort;
typedef cl_int	Int;
typedef cl_uint	UInt;
typedef cl_long	Long;
typedef cl_ulong	ULong;
typedef cl_float	Float;
typedef cl_double	Double;
_fff_END_NAMESPACE
#else
typedef unsigned int	Bool;
typedef char			Char;
typedef unsigned char	UChar;
typedef short			Short;
typedef unsigned short	UShort;
typedef int				Int;
typedef unsigned int	UInt;
typedef long			Long;
typedef unsigned long	ULong;
typedef half			Half;
typedef float			Float;
typedef double			Double;
#endif


#ifndef _fff_IS_DEV
#include <string>
_fff_BEGIN_NAMESPACE
typedef std::string String;
_fff_END_NAMESPACE

#include <sstream>
_fff_BEGIN_NAMESPACE
typedef std::stringstream StringStream;
_fff_END_NAMESPACE

template<class T>
fff::StringStream &operator+(fff::StringStream &lhs, T rhs)
{
    lhs << rhs;
    return lhs;
}

#include <vector>
_fff_BEGIN_NAMESPACE
typedef std::vector<::cl::Device> Devices;
typedef std::vector<::cl::Platform> Platforms;
_fff_END_NAMESPACE
#endif

_fff_BEGIN_NAMESPACE

typedef Float	Sample32;
typedef Double	Sample64;

#ifdef fff_USE_64
typedef Sample64	Sample;
#else
typedef Sample32	Sample;
#endif


#ifndef _fff_IS_DEV
static const Bool False = CL_FALSE;
static const Bool True = CL_TRUE;
#else
#define False 0
#define True 1
#endif

_fff_END_NAMESPACE


#endif