
#ifndef __fffcvhstschbuf_h__included__
#	define __fffcvhstschbuf_h__included__

#	include "_fffBase.h"
#	include "fffCv.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class CvHstSChBuf
			: public Cv<SampleType>
		{
		public:
			typedef
				SampleType MySample;
			typedef
				CvHstSChBuf<MySample> My;

			CvHstSChBuf(
				UInt sampleCount = 0)
				: Cv(sampleCount)
			{
			}
		};

	_fff_END_NAMESPACE

#endif