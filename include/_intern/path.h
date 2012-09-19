//---------------------------------------------------------+
// fff/include/_intern/path.h
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
//!	\file		path.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Environmental paths
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ___path_h__included__
#define ___path_h__included__

#include "_base.h"
#include "exceptionsinternal.h"
#include "_types.h"

#ifdef _fff_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

using namespace fff::_Intern::Exceptions;

namespace fff {
namespace _Intern {

#define fff_FILENAME(PATH) \
    (fff::_Intern::path( (PATH) ).getLastToken())

#define fff_FILE() \
    fff_FILENAME( __FILE__ )

#define fff_CSTRFILE() \
    fff_FILENAME( __FILE__ ).c_str()

#define fff_EXPANDPATH(PATH) \
    ((const char *)fff::_Intern::path( (PATH) ))

class path
{
public:
    path(const char *path)
    {
        #ifdef _fff_WIN
            DWORD size = ExpandEnvironmentStringsA(path, NULL, 0);
            char *p = new char[size+1];
            ExpandEnvironmentStringsA(path, p, size+1);
        #else
            // someone'd like to implement this?
            #error Use of this class might went wrong!
        #endif

        m_path = String(p);

        #ifdef _fff_WIN
            delete [] p;
        #endif
    }

    ~path() throw()
    {
    }

    String getLastToken() const;

    operator const char *() const
    {
        return
            getPath().c_str();
    }

    const String &getPath() const
    {
        return
            m_path;
    }

    bool operator!() const throw()
    {
        return
            false;
    }

private:
    String
        m_path;
};

}
}


#endif