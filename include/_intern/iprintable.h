//---------------------------------------------------------+
// fff/include/_intern/iprintable.h
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
//!	\file		iprintable.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Interface to let objects be printable.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ___iprintable_h__included__
#define ___iprintable_h__included__

#include "_base.h"
#include "logstream.h"
#include <ostream>

using namespace std;
using namespace fff::_Intern;

namespace fff {
namespace _Intern {

class iprintable
{
public:
	virtual void print(logstream &out) const
    {
        // nothing is default...
    }
};

}
}

ostream &operator<<(ostream &stream, const iprintable &printable);

/*
logstream &operator<<(logstream &lhs, const iprintable &printable);
*/
#endif