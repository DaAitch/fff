
#ifndef __fffinvalidstateexception_h__included__
#	define __fffinvalidstateexception_h__included__

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

