//---------------------------------------------------------+
// fff/include/__fffDevelHost.h
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
//!	\file		__fffDevelHost.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		OOP implementation helper macros.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ____fffdevelhost_h__included__
#	define ____fffdevelhost_h__included__

#	define _fff_DEVELHOST_CRASH() \
		*((int *)NULL)=0

#	define _fff_DEVELHOST_IMPL_CRASH_COPYCTOR(CLASS)	\
		CLASS(const CLASS&) { \
			_fff_DEVELHOST_CRASH(); }
#	define _fff_DEVELHOST_IMPL_CRASH_ASSIGNOP(CLASS) \
		CLASS&operator=(const CLASS&) { \
			_fff_DEVELHOST_CRASH(); return *this; }

#	define _fff_DEVELHOST_IMPL_DEFAULT_COPYCTOR()
#	define _fff_DEVELHOST_IMPL_DEFAULT_ASSIGNOP()

#endif