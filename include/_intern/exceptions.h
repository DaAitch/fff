//---------------------------------------------------------+
// fff/include/_intern/exceptions.h
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
//!	\file		exceptions.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Invalid state Exception definitions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ___exception_h__included__
#define ___exception_h__included__

#include "_base.h"
#include "_types.h"
#include "iprintable.h"
#include "logStream.h"
#include "path.h"
#include "exceptionsinternal.h"

#define __fff_EXCEPTION_MSG2(FILE, LINE, EXCEPTION, ...) \
    EXCEPTION##Exception( FILE, LINE, #EXCEPTION "Exception", EXCEPTION##ExceptionReport(__VA_ARGS__) )
#define __fff_EXCEPTION_MSG(EXCEPTION, ...) \
    __fff_EXCEPTION_MSG2(__FILE__, __LINE__, EXCEPTION, __VA_ARGS__)


#define _fff_EXCEPTION_THROW2(FILE, LINE, EXCEPTION, ...)	\
	throw __fff_EXCEPTION_MSG2(FILE, LINE, EXCEPTION, __VA_ARGS__)
#define _fff_EXCEPTION_THROW(EXCEPTION, ...)	\
	throw __fff_EXCEPTION_MSG(EXCEPTION, __VA_ARGS__)

#define _fff_EXCEPTION_HANDLE2(FILE, LINE, EXCEPTION, ...)	\
	::fffExceptionHandler(__fff_EXCEPTION_MSG2(FILE, LINE, EXCEPTION, __VA_ARGS__))
#define _fff_EXCEPTION_HANDLE(EXCEPTION, ...)	\
	::fffExceptionHandler(__fff_EXCEPTION_MSG(EXCEPTION, __VA_ARGS__))

    
#ifdef fff_USE_EXCEPTIONHANDLE
#define fff_ERR2(FILE, LINE, EXCEPTION, ...) \
    _fff_EXCEPTION_HANDLE2(FILE, LINE, EXCEPTION, __VA_ARGS__)
#define fff_ERR(EXCEPTION, ...) \
    _fff_EXCEPTION_HANDLE(EXCEPTION, __VA_ARGS__)
#else
#define fff_ERR2(FILE, LINE, EXCEPTION, ...) \
    _fff_EXCEPTION_THROW2(FILE, LINE, EXCEPTION, __VA_ARGS__)
#define fff_ERR(EXCEPTION, ...) \
    _fff_EXCEPTION_THROW(EXCEPTION, __VA_ARGS__)
#endif



#define fff_EXCEPTION_IMPL(EXCEPTION) \
    class EXCEPTION##Exception \
    : \
    public ::fff::_Intern::Exception \
    { \
    public: \
        template<class T> \
        EXCEPTION##Exception( \
            const char *file, \
            UInt line, \
            const char *parent, \
            const T &msg) \
            : \
            Exception( \
                file, \
                line, \
                parent, \
                msg) \
        { \
        } \
    };

#define fff_EXCEPTIONREPORT_BEGIN(EXCEPTION) \
    class EXCEPTION##ExceptionReport \
        : public ::fff::_Intern::Exceptions::IReport \
    { \
    public: \
        EXCEPTION##ExceptionReport
        
#define fff_EXCEPTIONREPORT_END() \
    };

#define fff_EXCEPTION_WITH_REPORT_BEGIN(EXCEPTION) \
    fff_EXCEPTION_IMPL(EXCEPTION) \
    fff_EXCEPTIONREPORT_BEGIN(EXCEPTION)

#define fff_EXCEPTION_WITH_REPORT_END() \
    };




namespace fff {
namespace _Intern {


class Exception :
    public iprintable
{
public:
    template<
        class T
    >
    Exception(
        const char *file,
        UInt line,
        const char *parentName,
        const T &msg)
        :
        m_file(fff_FILENAME(file)),
        m_line(line),
        m_parentName(parentName),
        m_message((String)msg)
	{
	}

	const String &getMessage() const
	{
		return
            m_message;
	}

    const String &getFile() const
    {
        return
            m_file;
    }

    UInt getLine() const
    {
        return
            m_line;
    }

    String getParent() const
    {
        return
            m_parentName;
    }

    String exception() const;
    String location() const;

    virtual void print(logstream &stream) const;

private:
	String
        m_message,
        m_file,
        m_parentName;
    UInt
        m_line;
};

namespace Exceptions {

class IReport
{
public:
    operator String() const
    {
        return reportContent();
    }

    String reportContent() const
    {
        return getReport().str();
    }

protected:
    const StringStream &getReport() const
    {
        if(!m_report)
            throw intern("invalid report stream");

        return
            m_report;
    }

    StringStream &getReport()
    {
        if(!m_report)
            throw intern("invalid report stream");

        return
            m_report;
    }

private:
    StringStream
        m_report;
};
}


}
}

extern void fffExceptionHandler(
        const fff::_Intern::Exception &e);

#endif

