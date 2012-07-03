
#ifndef ___ffftypes_h__included__
#	define ___ffftypes_h__included__

//#	define fff_USE_64

#	include "_fffBase.h"

#	ifndef _fff_IS_DEV
#		include <CL/cl.hpp>

#	endif

	_fff_BEGIN_NAMESPACE

#		ifndef _fff_IS_DEV
			typedef ::cl_bool	Bool;
			typedef ::cl_char	Char;
			typedef ::cl_uchar	UChar;
			typedef ::cl_short	Short;
			typedef ::cl_ushort	UShort;
			typedef ::cl_int	Int;
			typedef ::cl_uint	UInt;
			typedef ::cl_long	Long;
			typedef ::cl_ulong	ULong;
			typedef ::cl_float	Float;
			typedef ::cl_double	Double;
#		else
			typedef unsigned int	Bool;
			typedef char			Char;
			typedef unsigned char	UChar;
			typedef short			Short;
			typedef unsigned short	UShort;
			typedef int				Int;
			typedef unsigned int	UInt;
			typedef long			Long;
			typedef unsigned long	ULong;
			typedef float			Float;
			typedef double			Double;
#		endif


#		ifndef _fff_IS_DEV
			typedef ::std::string String;
			typedef ::std::stringstream StringStream;

			typedef ::std::vector<cl::Device> Devices;
			typedef ::std::vector<cl::Platform> Platforms;

#		endif


		typedef Float	Sample32;
		typedef Double	Sample64;

#		ifdef fff_USE_64
			typedef Sample64	Sample;
#		else
			typedef Sample32	Sample;
#		endif


#	ifndef _fff_IS_DEV
		static const Bool False = CL_FALSE;
		static const Bool True = CL_TRUE;
#	else
#		define False 0
#		define True 1
#	endif




	_fff_END_NAMESPACE


	_fff_USE_NAMESPACE


#endif