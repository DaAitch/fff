//---------------------------------------------------------+
// fff/include/_fffLogStream.h
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
//!	\file		_fffLogStream.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Some nicer and independant output streamer.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ___logstream_h__included__
#define ___logstream_h__included__

#include "_ns.h"
#include "_types.h"

#include <iostream>

using namespace std;

namespace fff {
namespace _Intern {

#define fff_LOG_VAL(LOGSTREAM, VAL) ( (LOGSTREAM) >> #VAL << (VAL) )

class logstream
{
public:
	typedef logstream My;

public:
	logstream(std::ostream &baseStream)
		:
        m_baseStream(&baseStream),
		m_margin(0),
        m_layerOut(false),
        m_createdStream(NULL)
	{
	}

	logstream()
		:
		m_createdStream(new StringStream()),
        m_baseStream(NULL),
        m_margin(0),
        m_layerOut(false)
	{
        m_baseStream = m_createdStream;
	}

    ~logstream() throw()
    {
        if(m_createdStream)
        {
            delete m_createdStream;
            m_createdStream = NULL;
        }
    }
		
	My &indent(const char *title);
	My &indent(const String &title)
	{
		return indent(title.c_str());
	}

	template<class T>
	My &operator()(T title)
	{
		return indent(title);
	}

	My &outdent(const char *title=NULL);
	My &outdent(const String &title)
	{
		return outdent(title.c_str());
	}

		
	My &operator()()
	{
		return outdent();
	}

	My &put(Bool aligned=true);

	template<class T>
	My &operator<<(const T &value)
	{
		m_valueStream << value;
		return *this;
	}

	template<class T>
	My &operator>>(const T &value)
	{
		m_propStream << value;
		return *this;
	}

	My &operator!()
	{
		return put(true);
	}

	My &operator~()
	{
		return put(false);
	}

	std::ostream &getStream()
	{
		return *m_baseStream;
	}

	My &notice()
	{
		std::cin.get();
		return *this;
	}


    UInt getMargin() const
    {
        return m_margin;
    }

private:
	void pad()
	{
		for(UInt i = 0; i < m_margin; ++i)
			*m_baseStream << MARGIN;
	}

private:

	static const Char MARGIN = '\t';
	static const Char SPACE = ' ';
	static const Char NL = '\n';
	static const Char EQALS = '=';

private:
	std::ostream
		*m_baseStream,
        *m_createdStream;
        

	UInt m_margin;

	StringStream
		m_propStream,
		m_valueStream;

    bool
        m_layerOut;

};

}
}

#endif