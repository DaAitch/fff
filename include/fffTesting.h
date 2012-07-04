//---------------------------------------------------------+
// fff/include/fffTesting.h
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

#ifndef __ffftesting_h__included__
#	define __ffftesting_h__included__


#	include "fff.h"
#	include "../cl/fffConvolution.h.cl"

	_fff_BEGIN_NAMESPACE

		void test_bitReversal();
		void test_nextBinStep();
		void test_calcOverlapSaveSampleOffset();
		void test_calcOverlapSaveSampleCount();
		void test_fftButterfly();
		void test_classV();
		void test_classCv();

	_fff_END_NAMESPACE

#endif /* ifndef __fcocltesting_h__included__ */