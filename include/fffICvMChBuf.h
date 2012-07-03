
#ifndef __fcoclicomplexsamplesmultichannel_h__included__
#	define __fcoclicomplexsamplesmultichannel_h__included__

#	include "_fffBase.h"

	template<
		class SampleType
	>
	class ICvMChBuf
	{
	public:
		typedef
			SampleType MySample;

	public:

		virtual MySample *getRawReal(
			UInt channelIndex) = 0;
		virtual MySample *getRawImag(
			UInt channelIndex) = 0;

		virtual const MySample *getRawReal(
			UInt channelIndex) const = 0;
		virtual const MySample *getRawImag(
			UInt channelIndex) const = 0;

		virtual UInt getChannelCount() const = 0;
		virtual UInt getSampleCount() const = 0;

		virtual Bool isAllocated() const = 0;

	};
#endif