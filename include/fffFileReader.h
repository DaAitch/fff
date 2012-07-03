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

#ifndef __ffffilereader_h__included__
#	define __ffffilereader_h__included__

#	include "_fffBase.h"
#	include <fstream>

	_fff_BEGIN_NAMESPACE

	class FileReader
	{
	public:
		FileReader(
			String file)
			:
			m_content(NULL)
		{
			std::ifstream f(file, std::ios::binary);

			if(f)
			{
				f.seekg(0, std::ios::end);
				Long size = f.tellg();
				f.seekg(0, std::ios::beg);

				m_content = new char[(UInt)size+1];
				f.read(m_content, size);
				m_content[size]=0;
				f.close();
			}
		}

		~FileReader()
		{
			if(m_content)
				delete [] m_content;
			m_content = NULL;
		}

		const char *getContent() const
		{
			return
				m_content;
		}

		bool operator!() const
		{
			return
				!m_content;
		}

	private:
		char *m_content;

	};

	_fff_END_NAMESPACE

#endif