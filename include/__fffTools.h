/*----------------------------------------------------------
 *    The Fast Filtering Framework implements an LTI filter
 *    with Khronos Group's OpenCL.
 *    Copyright (C) 2012  Philipp Renoth
 *----------------------------------------------------------
 *    This program is free software: you can redistribute
 *    it and/or modify it under the terms of the
 *    GNU General Public License as published by the
 *    Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the
 *    GNU General Public License along with this program.
 *    If not, see <http://www.gnu.org/licenses/>.
 *--------------------------------------------------------*/

#ifndef ____fff_tools_h__included__
#	define ____fff_tools_h__included__

#	include "_fffBase.h"

#	define fff_STRINGIFY(X)	    _fff_STRINGIFY2(X)
#	define _fff_STRINGIFY2(X)	#X

#	define fff_ME					(*this)

#	define fff_ARRAYITEMS(ARR)	(sizeof(ARR)/sizeof((ARR)[0]))







#endif /* ifndef ____fff_tools_h__included__ */