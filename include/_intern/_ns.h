
#ifndef ____ns_h__included__
#define ____ns_h__included__

#ifndef _fff_IS_DEV

#define _fff_USE_NAMESPACE

#define _fff_NAMESPACE			fff
#define _fff_NAMESPACE_PREFIX	_fff_NAMESPACE::
#define _fff_USING_NAMESPACE	using namespace _fff_NAMESPACE;

#define _fff_BEGIN_NAMESPACE	namespace _fff_NAMESPACE {
#define _fff_BEGIN_NAMESPACE_CL namespace cl {

#define _fff_END_NAMESPACE		}

#else

#define _fff_NAMESPACE
#define _fff_NAMESPACE_PREFIX
#define _fff_USING_NAMESPACE

#define _fff_BEGIN_NAMESPACE
#define _fff_BEGIN_NAMESPACE_CL

#define _fff_END_NAMESPACE

#endif

#endif