
#ifndef ____fff_tools_h__included__
#	define ____fff_tools_h__included__

#	include "_fffBase.h"

#	define fff_STRINGIFY(X)	    _fff_STRINGIFY2(X)
#	define _fff_STRINGIFY2(X)	#X

#	define fff_ME					(*this)

#	define fff_ARRAYITEMS(ARR)	(sizeof(ARR)/sizeof((ARR)[0]))







#endif /* ifndef ____fff_tools_h__included__ */