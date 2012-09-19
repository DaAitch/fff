//---------------------------------------------------------+
// fff/src/Computing/Compiler.cpp
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
//!	\file		Compiler.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Let OpenCL compile source and manages
//!				defined kernels.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#include "../../include/_intern.h"
#include "../../include/Computing/Compiler.h"

#include <vector>

using namespace fff::_Intern::RtCLC;
using namespace std;

namespace fff {
namespace Computing {

const char *Compiler::m_cprog = "#include \"_.cl\"\n";
const char *Compiler::CL_LOCATION = "cl/";

Compiler::Compiler()
{
}

Compiler::Compiler(
	const OpenCLEnvironment &env,
	const String &includeDirectory,
	const Worker &worker,
    const Unroll &unroll)
	: 
	m_worker(
        worker),
    m_unroll(
        unroll),
	m_includeDirectory(
		includeDirectory),
	m_env(
		env)
{
    fff_RTCLC_ERR_INIT();
    fff_RTCLC_SEQ_CHECK(
	    m_program = ::cl::Program(
		    getEnv().getContext(),
		    m_cprog,
		    false,
            fff_RTCLC_ERR_PTR));
}

::cl::Kernel Compiler::kernel(
	String kernelName) const
{
    fff_RTCLC_ERR_INIT();

    cl_build_status status;

    fff_RTCLC_SEQ_CHECK(
        status = getProgram().getBuildInfo<CL_PROGRAM_BUILD_STATUS>(
            getEnv().getDevice(), fff_RTCLC_ERR_PTR));

    if(status != CL_BUILD_SUCCESS)
        fff_ERR(ProgramNotCompiled);

	::cl::Kernel kernel;
	fff_RTCLC_SEQ_CHECK(
		kernel = ::cl::Kernel(getProgram(), kernelName.c_str(), fff_RTCLC_ERR_PTR));

	return kernel;
}

void Compiler::build()
{
    StringStream ss;
    getWorker().generateCompileOptions(ss);
    getUnroll().generateCompileOptions(ss);
	_build(
		ss);
}

bool Compiler::isCompiled() const
{
    try
    {
        cl_int err=0;
        cl_build_status status;

        status = getProgram().getBuildInfo<CL_PROGRAM_BUILD_STATUS>(
            getEnv().getDevice(), &err);

        if(err)
            return false;
        else
            return status == CL_BUILD_SUCCESS;
    }
    catch(...)
    {
    }

    return false;
}

void Compiler::_build(StringStream &options)
{
	fff_RTCLC_ERR_INIT();

	//! \TODO windows paths with spaces crash
    //!  and no double quotes allowed!?
	options << " -I " << m_includeDirectory;
	options << " -D _fff_IS_DEV";

	if(useSample64() == True)
		options << " -D fff_USE_64";

	fff_EXPECT_VALID_OBJ(m_env);
    fff_EXPECT_VALID_CLOBJ(m_program);

	String buildOptions = options.str();
	Devices devices(1);
	devices[0] = m_env.getDevice();

	Int buildErr;

	if((buildErr = m_program.build(devices, buildOptions.c_str())) != CL_SUCCESS)
	{
		//std::cout << buildErr;
		
		switch(buildErr)
		{
		case CL_BUILD_PROGRAM_FAILURE:
			Int buildInfoErr;

			fff_RTCLC_SEQ_CHECK(
				buildInfoErr = m_program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(getEnv().getDevice(), fff_RTCLC_ERR_PTR));
			if(buildInfoErr != CL_SUCCESS)
			{
				String buildInfo;
				fff_RTCLC_SEQ_CHECK(
					buildInfo = m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(getEnv().getDevice(), fff_RTCLC_ERR_PTR));

				//if(buildInfo.size() > 500)
				//	buildInfo.resize(500);
				//std::cout << buildInfo;
				fff_ERR(
					ProgramBuild, buildInfo);
			}
			break;
		default:
			fff_RTCLC_SEQ_CHECK_RET(
                buildErr);
			break;
		}
	}
}

}
}