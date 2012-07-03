
#ifndef ___fffiprintable_h__included__
#	define ___fffiprintable_h__included__

#	include "_fffBase.h"
#	include "_fffLogStream.h"

	_fff_BEGIN_NAMESPACE

		class IPrintable
		{
			virtual void print(
				LogStream &stream) const = 0;
		};

	_fff_END_NAMESPACE

#endif