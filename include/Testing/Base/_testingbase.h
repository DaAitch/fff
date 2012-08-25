//---------------------------------------------------------+
// fff/include/Testing/Base/_testingbase.h
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
//!	\file		_testingbase.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Base macros and functions for testing.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+
#ifdef _TEST

#ifndef ___testingbase_h__included__
#define ___testingbase_h__included__

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../../_intern.h"
#include "../../_intern/logstream.h"
//---------------------------------------------------------+

//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
#include <sstream>
#include <iostream>
//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
using namespace std;
using namespace fff::_Intern;
//---------------------------------------------------------+

#ifdef _TEST

int main();

namespace fff {
namespace Testing {
namespace Base {

static logstream testingStream(std::cout);
static unsigned int testingAssertionFailures = 0;
static unsigned int testingAssertionSucceedings = 0;

//
void testassert(
	bool assertion,				//!< [in]
	const char *file,			//!< [in]
	unsigned int line,			//!< [in]
	const char *failmsg=NULL	//!< [in]
);


#define _fff_TEST_MASSERT(EXPR, MSG) \
	::fff::Testing::Base::testassert( \
		(EXPR), __FILE__, __LINE__, (MSG) )

#define _fff_TEST_ASSERT(EXPR) \
	_fff_TEST_MASSERT(EXPR, #EXPR)


#define fff_TEST_MOK(RHS, MSG) \
	fff_TEST_MTRUE((RHS), (MSG))
#define fff_TEST_MTRUE(RHS, MSG) \
    _fff_TEST_MASSERT(!!(RHS), (MSG))
#define fff_TEST_MFALSE(RHS, MSG) \
    fff_TEST_MTRUE(!(RHS), (MSG))
#define fff_TEST_MNOK(RHS, MSG) \
	fff_TEST_MOK(!(RHS), (MSG))
#define fff_TEST_MNULL(RHS, MSG) \
    fff_TEST_MEQ( (RHS), 0, (MSG))
#define fff_TEST_MNNULL(RHS, MSG) \
    fff_TEST_MNEQ( (RHS), 0, (MSG))
#define fff_TEST_MEQ(LHS, RHS, MSG) \
	_fff_TEST_MASSERT((LHS)==(RHS), (MSG))
#define fff_TEST_MNEQ(LHS, RHS, MSG) \
	_fff_TEST_MASSERT((LHS)!=(RHS), (MSG))
#define fff_TEST_MLT(LHS, RHS, MSG) \
	_fff_TEST_MASSERT((LHS)< (RHS), (MSG))
#define fff_TEST_MLTE(LHS, RHS, MSG) \
	_fff_TEST_MASSERT((LHS)<=(RHS), (MSG))
#define fff_TEST_MGT(LHS, RHS, MSG) \
	_fff_TEST_MASSERT((LHS)> (RHS), (MSG))
#define fff_TEST_MGTE(LHS, RHS, MSG) \
	_fff_TEST_MASSERT((LHS)>=(RHS), (MSG))

#define fff_TEST_OK(RHS) \
	fff_TEST_TRUE((RHS))
#define fff_TEST_TRUE(RHS) \
    _fff_TEST_ASSERT(!!(RHS))
#define fff_TEST_FALSE(RHS) \
    fff_TEST_TRUE(!(RHS))
#define fff_TEST_NOK(RHS) \
	fff_TEST_OK(!(RHS))
#define fff_TEST_NULL(RHS) \
    fff_TEST_EQ( (RHS), 0)
#define fff_TEST_NNULL(RHS) \
    fff_TEST_NEQ( (RHS), 0)
#define fff_TEST_EQ(LHS, RHS) \
	_fff_TEST_ASSERT((LHS)==(RHS))
#define fff_TEST_NEQ(LHS, RHS) \
	_fff_TEST_ASSERT((LHS)!=(RHS))
#define fff_TEST_LT(LHS, RHS) \
	_fff_TEST_ASSERT((LHS)< (RHS))
#define fff_TEST_LTE(LHS, RHS) \
	_fff_TEST_ASSERT((LHS)<=(RHS))
#define fff_TEST_GT(LHS, RHS) \
	_fff_TEST_ASSERT((LHS)> (RHS))
#define fff_TEST_GTE(LHS, RHS) \
	_fff_TEST_ASSERT((LHS)>=(RHS))

#define fff_TESTINGSTREAM \
    fff::Testing::Base::testingStream

#define _fff_TEST_NAME(TYPE, NAME) \
    test_##TYPE##_##NAME

#define fff_TEST_CALL(TYPE, NAME) \
    _fff_TEST_NAME(TYPE, NAME)()

#define fff_TEST_DECLARE(TYPE, NAME) \
    void _fff_TEST_NAME(TYPE, NAME)();

#define fff_TEST_BEGIN_IMPL(TYPE, NAME) \
    void _fff_TEST_NAME(TYPE, NAME)() { \
        fff_TESTINGSTREAM("TEST " #TYPE "|" #NAME); \
	
#define fff_TEST_END_IMPL() \
	fff_TESTINGSTREAM(); }

#define fff_TEST_BEGIN_SECTION(STRSECTION) \
    { fff_TESTINGSTREAM(STRSECTION);

#define fff_TEST_END_SECTION() \
    fff_TESTINGSTREAM(); }


#define fff_TEST_TRY_NOTHROW(DO) \
    try \
    { \
            do{ DO ;}while(0); \
    } \
    catch(Exception &e) \
    { \
        testassert( \
            false, \
            e.getFile().c_str(), \
            e.getLine(), \
            fff_CSTRCONCAT( \
                "Expected no exception to be thrown in " #DO \
                ", but one is with message" + \
                e.exception())); \
    } \
    catch(std::exception &e) \
    { \
        testassert( \
            false, \
            fff_CSTRFILE(), \
            __LINE__, \
            fff_CSTRCONCAT( \
                "Expected no exception to be thrown in " #DO \
                ", but a std::exception is, with message: " + \
                e.what() \
            ) \
        ); \
    } \
    catch(...) \
    { \
        testassert( \
            false, \
            fff_CSTRFILE(), \
            __LINE__, \
            "Expected no exception to be thrown in " #DO \
            ", but an unknown is"); \
    }

#define fff_TEST_TRY_CATCH(DO, EXCEPTION) \
    try \
    { \
        do{ DO ;}while(0); \
        testassert( \
            false, \
            fff_CSTRFILE(), \
            __LINE__, \
            "Expected " #EXCEPTION " to be thrown in " #DO \
            ", but none is"); \
    } \
    catch(EXCEPTION &) \
    { \
    } \
    catch(Exception &e) \
    { \
        testassert( \
            false, \
            e.getFile().c_str(), \
            e.getLine(), \
            fff_CSTRCONCAT( \
                "Expected " #EXCEPTION " to be thrown in " #DO \
                ", but another is with message: " + e.exception() \
            ) \
        ); \
    } \
    catch(std::exception &e) \
    { \
        testassert( \
            false, \
            fff_CSTRFILE(), \
            __LINE__, \
            fff_CSTRCONCAT( \
                "Expected " #EXCEPTION " to be thrown in " #DO \
                ", but another is with message: " + e.what() \
            ) \
        ); \
    } \
    catch(...) \
    { \
        testassert( \
            false, \
            fff_CSTRFILE(), \
            __LINE__, \
            "Expected " #EXCEPTION " to be thrown in " #DO \
            ", but another is"); \
    }

}
}
}

#endif


#endif

#endif