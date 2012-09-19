//---------------------------------------------------------+
// fff/include/_intern/exceptionsrtclc.h
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
//!	\file		exceptionsrtclc.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Runtime OpenCL Check functions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+
#ifndef __rtclc_h__included__
#define __rtclc_h__included__

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "exceptions.h"
#include "rtclc.h"
#include "typecast.h"
//---------------------------------------------------------+

//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
using namespace fff;
using namespace fff::_Intern::RtCLC;
//---------------------------------------------------------+

namespace fff {
namespace _Intern {
namespace Exceptions {
namespace RtCLC {


#define fff_EXPECT_VALID_CLOBJ(CLOBJ) \
    do{ if(!((CLOBJ)() != 0)) { \
        fff_ERR(InvalidCLObject, #CLOBJ);}}while(0)

#define fff_EXPECT_INVALID_CLOBJ(CLOBJ) \
    do{ if(!!((CLOBJ)() != 0)) { \
        fff_ERR(ValidCLObject, #CLOBJ);}}while(0)

#define fff_EXPECT_VALID_CLOBJ_RET(CLOBJ) \
    do{ if(!((CLOBJ)() != 0)) { \
        fff_ERR(InvalidCLObject, #CLOBJ);} \
        return (CLOBJ);}while(0)

fff_EXCEPTION_WITH_REPORT_BEGIN(RtCLC)
(
    Int err,
    const stringified &msg
)
{
    getReport() <<
        "RtCLC Error[" << err << ": " <<
        rtclcDebugErrorByName(err) << "]: " <<
        msg;
}
fff_EXCEPTION_WITH_REPORT_END()

fff_EXCEPTION_WITH_REPORT_BEGIN(RtCLCInternal)
(
    const stringified &msg=stringified()
)
{
    getReport() <<
        "Internal CL Error " << msg;
}
fff_EXCEPTION_WITH_REPORT_END()

fff_EXCEPTION_WITH_REPORT_BEGIN(InvalidCLObject)
(  
    const stringified &obj
)
{
    getReport() <<
        "Expect valid CL object \"" << obj <<
        "\", but it has a zero id";
}
fff_EXCEPTION_WITH_REPORT_END()

fff_EXCEPTION_WITH_REPORT_BEGIN(ProgramNotCompiled)
(  
)
{
    getReport() <<
        "Program not compiled";
}
fff_EXCEPTION_WITH_REPORT_END()

fff_EXCEPTION_WITH_REPORT_BEGIN(ValidCLObject)
(  
    const stringified &obj
)
{
    getReport() <<
        "Expect invalid CL object \"" << obj <<
        "\", but it has a non-zero id";
}
fff_EXCEPTION_WITH_REPORT_END()

fff_EXCEPTION_WITH_REPORT_BEGIN(NoCLDeviceFound)
(
    const stringified &msg
)
{
    getReport() <<
        "No OpenCL Device found: " << msg;
}
fff_EXCEPTION_WITH_REPORT_END()

}
}
}
}

#endif