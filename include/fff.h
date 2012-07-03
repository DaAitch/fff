/*----------------------------------------------------------
 *    The Fast Filtering Framework implements an LTI filter
 *    with Khronos Group's OpenCL.
 *    Copyright (C) 2012  Philipp Renoth
 *----------------------------------------------------------
 *    This program is free software: you can redistribute
 *    it and/or modify it under the terms of the
 *    GNU General Public License as published by the
 *    Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the
 *    GNU General Public License along with this program.
 *    If not, see <http://www.gnu.org/licenses/>.
 *--------------------------------------------------------*/

#ifndef __fff_h__included__
#	define __fff_h__included__

#	include "_fffBase.h"




	_fff_BEGIN_NAMESPACE
		extern const char *gLicenseInformation;
	_fff_END_NAMESPACE





#	include "_fffLogStream.h"

#	include "fffCompiler.h"
#	include "fffFileReader.h"

#	include "fffCvDevSChBuf.h"
#	include "fffCvHstMChBuf.h"
#	include "fffCvHstMChBufPreallocated.h"
#	include "fffCvUbiMChBuf.h"
#	include "fffICvMChBuf.h"

#	include "fffClEnv.h"

#	include "fffCv.h"
#	include "fffV.h"

#	include "fffDevProps.h"

#	include "fffWavReader.h"

#	include "fffTesting.h"
#	include "fffComputingData.h"
#	include "fffComputingMap.h"
#	include "fffTFuncKernel.h"
#	include "fffConvKernel.h"
#	include "fffWavWriter.h"


		

#endif /* ifndef __fff_h__included__ */