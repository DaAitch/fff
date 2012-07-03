
#ifndef ___fffbase_h__included__
#	define ___fffbase_h__included__

#	ifndef _fff_IS_DEV

#		include <cassert>
#		include <cmath>
#		include <cstring>
#		include <iostream>
#		include <fstream>
#		include <sstream>
#		include <exception>

#	endif



#	ifndef _fff_IS_DEV
#		define _fff_NAMESPACE			fff
#		define _fff_NAMESPACE_PREFIX	_fff_NAMESPACE::
#		define _fff_USE_NAMESPACE		using namespace _fff_NAMESPACE;
#		define _fff_BEGIN_NAMESPACE		namespace _fff_NAMESPACE {
#		define _fff_END_NAMESPACE }
#	else
#		define _fff_NAMESPACE
#		define _fff_NAMESPACE_PREFIX
#		define _fff_USE_NAMESPACE
#		define _fff_BEGIN_NAMESPACE
#		define _fff_END_NAMESPACE
#	endif




#	include "__fffTypes.h"

#	ifndef _fff_IS_DEV
#		include "__fffDevelHost.h"
#		include "__fffTools.h"
#		include "__fffDebugging.h"
#	endif

#endif