

#ifndef ___fffdebugging_h__included__
#	define ___fffdebugging_h__included__

#	include <ctime>

#	include "_fffBase.h"
#	include "_fffException.h"
#	include "_fffLogStream.h"

// rtclc = run time cl check

	_fff_BEGIN_NAMESPACE

#		define fff_RTCLC_ERR		_fff_err_num
#		define fff_RTCLC_ERR_PTR	&fff_RTCLC_ERR
#		define fff_RTCLC_ERR_INIT()	Int fff_RTCLC_ERR = (Int)0

#		define _fff_RTCLC_ERRCHECK(FAILMSG)							\
			__rtclcCheckErr(fff_RTCLC_ERR, (const Char *)(FAILMSG))

#		define fff_RTCLC_SEQ_CHECK_RET(EXPR)						\
			do {													\
			fff_RTCLC_ERR = (EXPR);									\
			_fff_RTCLC_ERRCHECK(fff_STRINGIFY(EXPR)); } while(0)

#		define fff_RTCLC_SEQ_CHECK(EXPR)							\
			do {													\
			EXPR;													\
			_fff_RTCLC_ERRCHECK(fff_STRINGIFY(EXPR)); } while(0)


		void __rtclcCheckErr(
			Int err,
			const Char *subject);
		


#		define _fff_CASE_ERR(ERR)	\
			case (ERR): return String(#ERR);
	
		String rtclcDebugErrorByName(
			Int errorNum);

	_fff_END_NAMESPACE

#endif