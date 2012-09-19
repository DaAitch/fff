//---------------------------------------------------------+
// fff/src/_intern/iprintable.cpp
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
//!	\file		iprintable.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#include "../../include/_intern.h"

using namespace fff::_Intern;
using namespace std;

ostream &operator<<(ostream &stream, const iprintable &printable)
{
    logstream ls(stream);
    printable.print(ls);
    return stream;
}

/*
logstream &operator<<(logstream &lhs, const iprintable &printable)
{
    printable.print(lhs);
    return
        lhs;
}
*/