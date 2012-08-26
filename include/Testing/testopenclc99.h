//---------------------------------------------------------+
// fff/include/Testing/OpenCLC99.h
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
//!	\file		fffTesting.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		fff testing functions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifdef _fff_TEST

#ifndef __testopenclc99_h__included__
#define __testopenclc99_h__included__

//- PROJECT INCLUDES --------------------------------------+
#include "../../cl/_.h.cl"
#include "Base/_testingbase.h"
//---------------------------------------------------------+

//- EXTERNAL INCLUDES -------------------------------------+
//---------------------------------------------------------+

//- NAMESPACES --------------------------------------------+
using namespace fff::Testing::Base;
//---------------------------------------------------------+

namespace fff {
namespace Testing {

fff_TEST_DECLARE(fff, openclc99)

namespace OpenCLC99 {

// bits
fff_TEST_DECLARE(openclc99, POW2)
fff_TEST_DECLARE(openclc99, BITMASK)
fff_TEST_DECLARE(openclc99, BITSWAPONLY)
fff_TEST_DECLARE(openclc99, bit_reversal)

fff_TEST_DECLARE(openclc99, factorial)

fff_TEST_DECLARE(openclc99, multiple2)
fff_TEST_DECLARE(openclc99, calcOverlapSaveSampleOffset)
fff_TEST_DECLARE(openclc99, calcOverlapSaveSampleCount)
fff_TEST_DECLARE(openclc99, fftButterfly)

fff_TEST_DECLARE(openclc99, dconv)


}
}
}

#endif

#endif