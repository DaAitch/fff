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

#ifndef ___fffbase_h__included__
#	define ___fffbase_h__included__

#	ifndef _fff_IS_DEV

#		include <cassert>
#		include <cmath>
#		include <cstring>
#		include <iostream>
#		include <fstream>
#		include <sstream>
#		include <exception>

#	endif



#	ifndef _fff_IS_DEV
#		define _fff_NAMESPACE			fff
#		define _fff_NAMESPACE_PREFIX	_fff_NAMESPACE::
#		define _fff_USE_NAMESPACE		using namespace _fff_NAMESPACE;
#		define _fff_BEGIN_NAMESPACE		namespace _fff_NAMESPACE {
#		define _fff_END_NAMESPACE }
#	else
#		define _fff_NAMESPACE
#		define _fff_NAMESPACE_PREFIX
#		define _fff_USE_NAMESPACE
#		define _fff_BEGIN_NAMESPACE
#		define _fff_END_NAMESPACE
#	endif




#	include "__fffTypes.h"

#	ifndef _fff_IS_DEV
#		include "__fffDevelHost.h"
#		include "__fffTools.h"
#		include "__fffDebugging.h"
#	endif

#endif