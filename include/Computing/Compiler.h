//---------------------------------------------------------+
// fff/include/fffCompiler.h
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
//!	\file		fffCompiler.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Let OpenCL compile source and manages
//!				defined kernels.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffcompiler_h__included__
#define __fffcompiler_h__included__

#include "../_intern.h"
#include "OpenCLEnvironment.h"
#include "Optimization/Worker.h"
#include "Optimization/Unroll.h"
#include "../../cl/_.h.cl"

using namespace fff::Computing::Optimization;
using namespace fff::_Intern::Exceptions::RtCLC;
using namespace fff::_Intern::Exceptions::Runtime;

namespace fff {
namespace Computing {

class Compiler
    :
    public iprintable
{
public:
    static const char *CL_LOCATION;

public:
    Compiler();

	Compiler(
		const OpenCLEnvironment &env,
		const String &includeDirectory=Compiler::CL_LOCATION,
        const Worker &worker=Worker(),
        const Unroll &unroll=Unroll());

	::cl::Kernel kernel(
		String kernelName) const;

	const ::cl::Program &getProgram() const
	{
        fff_EXPECT_VALID_CLOBJ_RET(m_program);
	}

	const OpenCLEnvironment &getEnv() const
	{
        fff_EXPECT_VALID_OBJ_RET(m_env);
	}

    OpenCLEnvironment &getEnv()
	{
        fff_EXPECT_VALID_OBJ_RET(m_env);
	}

    String getIncludeDirectory() const
    {
        return 
            m_includeDirectory;
    }

	Bool useSample64() const
	{
#ifdef fff_USE_64
		return True;
#else
		return False;
#endif
	}

	void build();

    const Worker &getWorker() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_worker);
    }

    const Unroll &getUnroll() const
    {
        fff_EXPECT_VALID_OBJ_RET(m_unroll);
    }

    virtual void print(logstream &out) const
    {
        out("Compiler");
        !fff_LOG_VAL(out, getEnv());
        !fff_LOG_VAL(out, getIncludeDirectory());
        !fff_LOG_VAL(out, isCompiled());
        !fff_LOG_VAL(out, getWorker());
        !fff_LOG_VAL(out, getUnroll());
        !fff_LOG_VAL(out, useSample64());
        out();
    }

    bool isCompiled() const throw();

    bool operator!() const throw()
    {
        return
            !isCompiled() ||
            !m_program() ||
            !m_env ||
            !m_worker ||
            !m_unroll;
    }

private:

	static const char *m_cprog;

	void _build(StringStream &options);

	::cl::Program
		m_program;
	OpenCLEnvironment
		m_env;
	String
		m_includeDirectory;

    
    Worker
        m_worker;
    Unroll
        m_unroll;
};

	
} }

#endif