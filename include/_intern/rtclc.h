//---------------------------------------------------------+
// fff/include/_intern/rtclc.h
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
//!	\file		rtclc.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		OpenCL Runtime Check
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ____rtclc_h__included__
#define ____rtclc_h__included__

#include "_base.h"
#include "_types.h"
#include "_tools.h"

namespace fff {
namespace _Intern {
namespace RtCLC {

#define fff_RTCLC_ERR		    _fff_err_num				/*!< Symbolic error number name.*/
#define fff_RTCLC_ERR_PTR	    &fff_RTCLC_ERR				/*!< Symbolic error number name.*/
#define fff_RTCLC_ERR_INIT()	Int fff_RTCLC_ERR = (Int)0	/*!< Symbolic error number name.*/

		//! \internal Calls external check function with given message.
#define _fff_RTCLC_ERRCHECK(ERRNO, FAILMSG)							\
	__rtclcCheckErr(__FILE__, __LINE__, ERRNO, (const char *)(FAILMSG) )

/*! Uses and checks return value of \em EXPR as error value
	\code{.cpp}
		fff_RTCLC_SEQ_CHECK_RET(
			devQueue.finish());
	\endcode
*/
#define fff_RTCLC_SEQ_CHECK_RET(EXPR)						\
	do {													\
    try {                                                   \
	fff_RTCLC_ERR = (EXPR);									\
    } catch(std::exception &e) {                            \
        fff_ERR(RtCLCInternal, e.what());\
    }                                                       \
	_fff_RTCLC_ERRCHECK(fff_RTCLC_ERR, fff_STRINGIFY(EXPR)); } while(0)

/*! Expects \em EXPR to autofill the error variable especially
	those CL functions, writing the error value back via the
	last \c{cl_int*} parameter.
*/
#define fff_RTCLC_SEQ_CHECK(EXPR)							\
	do {													\
    try {                                                   \
	EXPR;													\
    } catch(std::exception &e) {                            \
        fff_ERR(RtCLCInternal, e.what());\
    } \
	_fff_RTCLC_ERRCHECK(fff_RTCLC_ERR, fff_STRINGIFY(EXPR)); } while(0)

void __rtclcCheckErr(
    const char *file,
    UInt line,
	int err,
	const char *msg);
		


#define _fff_CASE_ERR(ERR)	\
	case (ERR): return String(#ERR);
	
String rtclcDebugErrorByName(
	Int errorNum);

}
}
}

#endif