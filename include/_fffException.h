//---------------------------------------------------------+
// fff/include/_fffException.h
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
//!	\file		_fffException.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Invalid state Exception definition.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef ___fffexception_h__included__
#	define ___fffexception_h__included__

#	include "_fffBase.h"

_fff_BEGIN_NAMESPACE

#	define fff_THROW(MSG)	\
		throw InvalidStateException( (MSG), (__LINE__), _fff_NAMESPACE_PREFIX String(__FILE__))

	class InvalidStateException : public std::exception
	{
	public:
		InvalidStateException(
			const String &message,
			UInt line,
			const String &file
			)
		{
			StringStream ss;
			ss << "INVALIDSTATE[";
			ss << file << "(" << line << "): ";
			ss << message;
			m_message = ss.str();
		}

		virtual const char *what() const
		{
			return m_message.c_str();
		}

	private:
		String m_message;
	};

_fff_END_NAMESPACE

#endif

