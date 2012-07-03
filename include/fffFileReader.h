
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