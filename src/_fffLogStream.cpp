
#include "../include/_fffLogStream.h"



LogStream::My &LogStream::indent(
	const char *title=NULL)
{
	pad();
	if(title)
		(*m_baseStream) << title;
	(*m_baseStream) << std::endl;

	++m_margin;

	return *this;
}
LogStream::My &LogStream::outdent(
	const char *title)
{
	if(m_margin > 0)
		--m_margin;
	
	pad();
	if(title)
		(*m_baseStream) << title;

	(*m_baseStream) << std::endl;

	return *this;
}

LogStream::My &LogStream::put(Bool aligned)
{
	// property size plus " = " (=3 chars)
	String prop = m_propStream.str();
	String value = m_valueStream.str();

	m_propStream = StringStream();
	m_valueStream = StringStream();

	const ::size_t propSize = prop.size() + 3;
	
	pad();
	(*m_baseStream) << prop << SPACE << EQALS << SPACE;

	
	const char *val = value.c_str();

	while(char ch=*val++)
	{
		if(ch==NL)
		{
			(*m_baseStream) << std::endl;
			pad();

			if(aligned)
				for(::size_t i = 0; i < propSize; ++i)
					(*m_baseStream) << SPACE;
			else
				(*m_baseStream) << MARGIN;
		}

		(*m_baseStream) << ch;
	}

	(*m_baseStream) << std::endl;

	return *this;
	
}