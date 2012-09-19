//---------------------------------------------------------+
// fff/src/_intern/exceptions.cpp
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
//!	\file		exceptions.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		fff testing functions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#include "../../include/_intern.h"

namespace fff {
namespace _Intern {


String Exception::exception() const
{
    return
        getParent() + " " +
        location() + ": " +
        getMessage();
}

String Exception::location() const
{
    StringStream ss;
    ss << m_file << "[" << m_line << "]";
    return ss.str();
}



void Exception::print(logstream &stream) const
{
    stream(getParent());
    ~(stream >> "File" << getFile());
    ~(stream >> "Line" << getLine());
    ~(stream >> "Message" << getMessage());
    stream();
}

}
}