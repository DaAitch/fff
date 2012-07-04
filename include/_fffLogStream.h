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

#ifndef ___ffflogstream_h__included__
#	define ___ffflogstream_h__included__

#	include "_fffBase.h"

	_fff_BEGIN_NAMESPACE

		class LogStream
		{
		public:
			typedef LogStream My;

		public:
			LogStream(std::ostream &baseStream)
				: m_baseStream(&baseStream),
				m_margin(0)
			{
			}

			LogStream()
				: m_baseStream(NULL),
				m_margin(0)
			{
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
			My &operator<<(T value)
			{
				m_valueStream << value;
				return *this;
			}

			template<class T>
			My &operator>>(T value)
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
				return put(False);
			}

			void setStream(
				std::ostream &baseStream)
			{
				m_baseStream = &baseStream;
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

			void die()
			{
				(*m_baseStream).flush();
				exit(EXIT_FAILURE);
			}



		private:
			void pad()
			{
				for(UInt i = 0; i < m_margin; ++i)
					(*m_baseStream) << MARGIN;
			}

		private:

			static const Char MARGIN = '\t';
			static const Char SPACE = ' ';
			static const Char NL = '\n';
			static const Char EQALS = '=';

		private:
			std::ostream
				*m_baseStream;
			UInt m_margin;

			StringStream
				m_propStream,
				m_valueStream;

		};

	_fff_END_NAMESPACE

#endif