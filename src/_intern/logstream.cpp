//---------------------------------------------------------+
// fff/src/_intern/logstream.cpp
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
//!	\file		logstream.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Some nicer and independant output streamer.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#include "../../include/_intern.h"

namespace fff {
namespace _Intern {

logstream::My &logstream::indent(
	const char *title=NULL)
{
	pad();
	if(title)
		*m_baseStream << title;
	*m_baseStream << std::endl;

	++m_margin;

    m_layerOut = false;

	return *this;
}
logstream::My &logstream::outdent(
	const char *title)
{
	if(m_margin > 0)
		--m_margin;
	
	if(title)
    {
        pad();
		*m_baseStream << title;
        *m_baseStream << std::endl;
    }
    
    if(title || m_layerOut)
	    *m_baseStream << std::endl;

    m_layerOut = true;

	return *this;
}

logstream::My &logstream::put(Bool aligned)
{
	// property size plus " = " (=3 chars)
	String prop = m_propStream.str();
	String value = m_valueStream.str();

	m_propStream = StringStream();
	m_valueStream = StringStream();

	pad();
    if(value.size() > 0)
	    *m_baseStream << prop << SPACE << EQALS << SPACE;
    else
    {
        *m_baseStream << prop << std::endl;
        return *this;
    }

	const ::size_t propSize = prop.size() + 3;
	const char *val = value.c_str();

	while(char ch=*val++)
	{
		if(ch==NL)
		{
			*m_baseStream << std::endl;
			pad();

			if(aligned)
				for(::size_t i = 0; i < propSize; ++i)
					*m_baseStream << SPACE;
			else
				*m_baseStream << MARGIN;
		}

		*m_baseStream << ch;
	}

	*m_baseStream << std::endl;

	return *this;
	
}

}
}