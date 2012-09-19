//---------------------------------------------------------+
// fff/include/Testing/testclasses.h
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
//!	\file		testclasses.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		fff testing
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+
#ifdef _fff_TEST

#ifndef __testclasses_h__included__
#define __testclasses_h__included__

//- PROJECT INCLUDES --------------------------------------+
#include "Base/_testingbase.h"
//---------------------------------------------------------+

//- EXTERNAL INCLUDES -------------------------------------+
#include <iomanip>
//---------------------------------------------------------+

//- NAMESPACES --------------------------------------------+
using namespace fff::Testing::Base;
//---------------------------------------------------------+

namespace fff {
namespace Testing {

fff_TEST_DECLARE(fff, classes)

namespace Classes {



// Computing


fff_TEST_DECLARE(classes, Unroll);
fff_TEST_DECLARE(classes, Worker);

fff_TEST_DECLARE(classes, ComputingData);
fff_TEST_DECLARE(classes, DeviceProperties);
fff_TEST_DECLARE(classes, Mapper);


fff_TEST_DECLARE(classes, OpenCLEnvironment);
fff_TEST_DECLARE(classes, Compiler);

// Buffer
fff_TEST_DECLARE(classes, Vector);
fff_TEST_DECLARE(classes, ComplexVector);

fff_TEST_DECLARE(classes, DevSingleChannel);
fff_TEST_DECLARE(classes, DevMultiChannel);
			
fff_TEST_DECLARE(classes, HostSingleChannel);
fff_TEST_DECLARE(classes, HostMultiChannel);
fff_TEST_DECLARE(classes, HostMultiChannelPreallocated);


fff_TEST_DECLARE(classes, UbiMultiChannel);

// Kernel
// void test_class_KernelBase();
fff_TEST_DECLARE(classes, Convolution);

fff_TEST_DECLARE(classes, TransferFunction);
fff_TEST_DECLARE(classes, Transformation);
fff_TEST_DECLARE(classes, FastConvolution);


// Filesystem
// void test_class_FileReader();
// void test_class_WaveReader();
// void test_class_WaveWriter();


}
}
}

#endif

#endif