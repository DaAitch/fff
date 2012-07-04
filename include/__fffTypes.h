//---------------------------------------------------------+
// fff/include/__fffTypes.h
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
//!	\file		__fffTypes.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Central type definitions (also OpenCL).
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ____ffftypes_h__included__
#	define ____ffftypes_h__included__

#	include "_fffBase.h"

#	ifndef _fff_IS_DEV
#		include <CL/cl.hpp>

#	endif

	_fff_BEGIN_NAMESPACE

#		ifndef _fff_IS_DEV
			typedef ::cl_bool	Bool;
			typedef ::cl_char	Char;
			typedef ::cl_uchar	UChar;
			typedef ::cl_short	Short;
			typedef ::cl_ushort	UShort;
			typedef ::cl_int	Int;
			typedef ::cl_uint	UInt;
			typedef ::cl_long	Long;
			typedef ::cl_ulong	ULong;
			typedef ::cl_float	Float;
			typedef ::cl_double	Double;
#		else
			typedef unsigned int	Bool;
			typedef char			Char;
			typedef unsigned char	UChar;
			typedef short			Short;
			typedef unsigned short	UShort;
			typedef int				Int;
			typedef unsigned int	UInt;
			typedef long			Long;
			typedef unsigned long	ULong;
			typedef float			Float;
			typedef double			Double;
#		endif


#		ifndef _fff_IS_DEV
			typedef ::std::string String;
			typedef ::std::stringstream StringStream;

			typedef ::std::vector<cl::Device> Devices;
			typedef ::std::vector<cl::Platform> Platforms;

#		endif


		typedef Float	Sample32;
		typedef Double	Sample64;

#		ifdef fff_USE_64
			typedef Sample64	Sample;
#		else
			typedef Sample32	Sample;
#		endif


#	ifndef _fff_IS_DEV
		static const Bool False = CL_FALSE;
		static const Bool True = CL_TRUE;
#	else
#		define False 0
#		define True 1
#	endif




	_fff_END_NAMESPACE


	_fff_USE_NAMESPACE


#endif