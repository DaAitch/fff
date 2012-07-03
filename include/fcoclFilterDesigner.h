
#ifndef __fcoclfilter_h__included__
#	define __fcoclfilter_h__included__

#	include "fcoclComplexSamplesMultiChannel.h"

	class fcoclFilterDesigner
	{
	public:
		virtual fcocl_uint getChannelCount() const = 0;
		virtual fcocl_uint getSampleCount() const = 0;

		virtual void createFilter(
			fcoclComplexSamplesMultiChannel &b,
			fcoclComplexSamplesMultiChannel &a) = 0;
	};

#endif