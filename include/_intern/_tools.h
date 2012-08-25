//---------------------------------------------------------+
// fff/include/__ffTools.h
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
//!	\file		__fffTools.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Some smart helper function and macros.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ____tools_h__included__
#define ____tools_h__included__

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "_ns.h"
#include "_types.h"
//---------------------------------------------------------+

//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
#ifndef _fff_IS_DEV
#include <ostream>
using namespace std;
#endif

//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
_fff_USING_NAMESPACE
//---------------------------------------------------------+

_fff_BEGIN_NAMESPACE

#define fff_STRINGIFY(X)	_fff_STRINGIFY2(X)
#define _fff_STRINGIFY2(X)	#X

#define fff_ME				(*this)

#define fff_STRCONCAT(X)    (StringStream() + X).str()
#define fff_CSTRCONCAT(X)    fff_STRCONCAT(X).c_str()

#ifndef _fff_IS_DEV
template<
    class SampleType
>
void copySamples(
    SampleType *to,
    const SampleType *from,
    unsigned int n)
{
    while(n--)
        to[n] = from[n];
}

template<
    class SampleType
>
SampleType variance(
    const SampleType *x1,
    const SampleType *x2,
    unsigned int n)
{
    SampleType v(0);
    while(n--)
    {
        v +=
            (x1[n]-x2[n])*
            (x1[n]-x2[n]);
    }

    return
        v;
}

template<
    class T
>
void printarray(ostream &out, const T *arr, unsigned int n, const char *sep="\n")
{
    for(unsigned int i=0;i<n;++i)
        out << arr[i] << sep;
}

#endif

_fff_END_NAMESPACE

#endif /* ifndef ____fff_tools_h__included__ */