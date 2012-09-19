//---------------------------------------------------------+
// fff/src/_intern/path.cpp
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
//!	\file		path.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		paths
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#include "../../include/_intern.h"


namespace fff {
namespace _Intern {

String path::getLastToken() const
{
    size_t lastToken = getPath().find_last_of(fff_DS) + 1;
    return getPath().substr(lastToken);
}

}
}