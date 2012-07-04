//---------------------------------------------------------+
// fff/src/maintest.cpp
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
//!	\file		maintest.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Entry point in test mode to invoke tests.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#include "../include/fff.h"

using namespace fff;
using namespace std;

#ifdef _TEST
	void extRtCLCHandler(
		int err,
		const char *msg)
	{
		if(err)
		{
			StringStream strstr;
			strstr << "Error " << err << "[";
			strstr << rtclcDebugErrorByName(err);
			strstr << "]: " << msg;

			cerr << strstr.str() << endl << endl;

			throw exception(strstr.str().c_str());
		}
	}

	int main()
	{
		try
		{
			test_nextBinStep();
			test_bitReversal();
			test_fftButterfly();
			test_classV();
			test_classCv();
			test_calcOverlapSaveSampleCount();
			test_calcOverlapSaveSampleOffset();

			cout << "All tests passed!" << std::endl;
			cout << "Please press [ Enter ] to quit the test.   . . ." << std::endl;
			cin.get();
			return EXIT_SUCCESS;
		}
		catch(exception &e)
		{
			cout << e.what();

			cin.get();
			return
				EXIT_FAILURE;
		}
	}

#endif