//---------------------------------------------------------+
// fff/cl/_.h.cl
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
//!	\file		_.h.cl
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		OpenCL Implementation.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#include "fff.h.cl"

#include "consts.h.cl"
#include "math.h.cl"

#include "bits.h.cl"

#include "dft.h.cl"
#include "dconv.h.cl"

#include "fft.h.cl"
#include "fconv.h.cl"

#include "dsp.h.cl"
#include "overlapsave.h.cl"
#include "tf.h.cl"