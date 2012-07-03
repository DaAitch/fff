
#ifndef __fcocllogstream_h__included__
#	define __fcocllogstream_h__included__

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