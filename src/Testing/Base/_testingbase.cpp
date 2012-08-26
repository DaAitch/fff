//---------------------------------------------------------+
// fff/src/Testing/Base/_testingbase.cpp
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
//!	\file		_testingbase.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Base macros and functions for testing.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifdef _fff_TEST

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../../../include/_intern.h"
#include "../../../include/Testing/Base/_testingbase.h"
#include "../../../include/Testing/testintern.h"
#include "../../../include/Testing/testopenclc99.h"
#include "../../../include/Testing/testclasses.h"
//---------------------------------------------------------+

//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
#include <iostream>
#include <sstream>
//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
using namespace std;
using namespace fff::_Intern;
using namespace fff::Testing;
using namespace fff::Testing::Classes;
using namespace fff::Testing::OpenCLC99;
using namespace fff::Testing::Base;
//---------------------------------------------------------+

	int main()
	{
        try
        {
            fff_TEST_CALL(fff, intern);
		    fff_TEST_CALL(fff, openclc99);
		    fff_TEST_CALL(fff, classes);

            fff_TESTINGSTREAM("END OF TESTING");

		    ~(fff_TESTINGSTREAM >> "Assertions" <<
                    (testingAssertionFailures +
				    testingAssertionSucceedings));
            ~(fff_TESTINGSTREAM >> "Failures" <<
			        testingAssertionFailures);
            ~(fff_TESTINGSTREAM >> "Succeedings" <<
			    testingAssertionSucceedings);

            fff_TESTINGSTREAM();

            #ifdef _fff_INTERACTIVE_TESTING
            std::cin.get();
            #endif

		    return
			    testingAssertionFailures==0 ?
			    EXIT_SUCCESS :
			    EXIT_FAILURE;
        }
        catch(fff::_Intern::Exception &e)
        {
            testassert(
                false,
                e.getFile().c_str(),
                e.getLine(),
                e.exception().c_str());
        }
        catch(std::exception &e)
        {
            testassert(
                false,
                NULL,
                0,
                e.what());
        }
        catch(...)
        {
            testassert(
                false,
                NULL,
                0,
                "Unknown exception");
        }

        _CrtDumpMemoryLeaks();

        #ifdef _fff_INTERACTIVE_TESTING
            std::cin.get();
        #endif
        return EXIT_FAILURE;
	}


namespace fff {
namespace Testing {
namespace Base {

void testassert(
	bool assertion,
	const char *file,
	unsigned int line,
	const char *failmsg
	)
{
	if(!assertion)
	{
		++testingAssertionFailures;

        ~(fff_TESTINGSTREAM >> "Failure " >>
			file >> "[" >> line >> "]" <<
			failmsg);
	}
	else
		++testingAssertionSucceedings;
}


}
}
}

#endif