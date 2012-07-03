
#ifndef ___fffdevelhost_h__included__
#	define ___fffdevelhost_h__included__

#	define _fff_DEVELHOST_CRASH() \
		*((int *)NULL)=0

#	define _fff_DEVELHOST_IMPL_CRASH_COPYCTOR(CLASS)	\
		CLASS(const CLASS&) { \
			_fff_DEVELHOST_CRASH(); }
#	define _fff_DEVELHOST_IMPL_CRASH_ASSIGNOP(CLASS) \
		CLASS&operator=(const CLASS&) { \
			_fff_DEVELHOST_CRASH(); return *this; }

#	define _fff_DEVELHOST_IMPL_DEFAULT_COPYCTOR()
#	define _fff_DEVELHOST_IMPL_DEFAULT_ASSIGNOP()

#endif