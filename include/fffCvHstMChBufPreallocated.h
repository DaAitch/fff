
#ifndef __ffcvhstmchbufpreallocated_h__included__
#	define __ffcvhstmchbufpreallocated_h__included__

#	include "_fffBase.h"
#	include "fffICvMChBuf.h"
#	include "fffV.h"

	template<
		class SampleType
	>
	class CvHstMChBufPreallocated :
		public ICvMChBuf<SampleType>
	{
	public:
		typedef
			SampleType MySample;
		typedef
			V<MySample> MySamples;
		typedef
			V<MySamples> MyMChSamples;
		typedef
			CvHstMChBufPreallocated<MySample> My;

	public:
		CvHstMChBufPreallocated()
		:
		m_channelCount(0),
		m_sampleCount(0),
		m_channelSamples(NULL)
		{
			assert(
				!isAllocated());
		}

		CvHstMChBufPreallocated(
			UInt channelCount,
			UInt sampleCount,
			MySample **channelSamples=NULL)
		:
		m_channels(0),
		m_length(0),
		m_channelSamples(0)
		{
			assert(
				!isAllocated());

			alloc(
				channelCount,
				sampleCount,
				channelSamples);

			assert(
				isAllocated());
		}

		
		void alloc(
			UInt channelCount,
			UInt sampleCount,
			MySample **channelSamples=NULL)
		{
			dealloc();

			if(channelCount > 0 && sampleCount > 0)
			{

				_alloc(
					channelCount,
					sampleCount,
					channelSamples);

				assert(
					isAllocated());

				initImag();

				assert(
					isAllocated());
			}
			else
				assert(
					!isAllocated());
		}

		void dealloc()
		{
			if(isAllocated())
				_dealloc();

			assert(
				!isAllocated());
		}

		void initImag()
		{
			if(!isAllocated())
				fff_THROW("not allocated");

			assert(
				isAllocated());

			_initImag();

			assert(
				isAllocated());
		}

		Bool isAllocated() const
		{
			if(_isAllocated())
			{

				assert(
					m_channelCount > 0 &&
					m_sampleCount > 0);

				assert(
					m_imagSamples.isAllocated());

				return True;
			}
			else
			{
				assert(
					m_channelCount == 0 &&
					m_sampleCount == 0);

				assert(
					!m_imagSamples.isAllocated());

				return False;
			}
		}
			

		UInt getChannelCount() const
		{
			return m_channelCount;
		}

		UInt getSampleCount() const
		{
			return m_sampleCount;
		}

		void setPointer(
			MySample **real,
			UInt sampleCount)
		{
			if(!real)
				fff_THROW("invalid pointer");

			if(sampleCount >= m_imagSamples.getSampleCount())
				fff_THROW("invalid size");

			m_channelSamples = real;
			m_sampleCount = sampleCount;
		}

		void clearPointer()
		{
			m_channelSamples = NULL;
			m_sampleCount = 0;
		}

		const MySample *getRawReal(
			UInt channel) const
		{
			return m_channelSamples[channel];
		}

		const SampleType *getRawImag(
			UInt channel) const
		{
			return m_imagSamples[channel].getSamples();
		}

		MySample *getRawReal(
			UInt channel)
		{
			return m_channelSamples[channel];
		}

		MySample *getRawImag(
			UInt channel)
		{
			return m_imagSamples[channel].getSamples();
		}

	private:

		void _alloc(
			UInt channelCount,
			UInt sampleCount,
			MySample **channelSamples)
		{
			assert(
				channelCount > 0 &&
				sampleCount > 0);

			m_channelSamples = channelSamples;
			m_channelCount = channelCount;
			m_sampleCount = sampleCount;

			m_imagSamples.alloc(
				m_channelCount);
		
			for(
				UInt channel = 0;
				channel < getChannelCount();
				++channel)
			{
				m_imagSamples[channel] = MySamples(sampleCount);
			}

			assert(
				isAllocated());
		}

		void _initImag()
		{
			assert(
				isAllocated());

			for(
				UInt channel = 0;
				channel < getChannelCount();
				++channel)
			{
				m_imagSamples[channel].init();
			}

			assert(
				isAllocated());
		}


		void _dealloc()
		{
			assert(
				isAllocated());

			m_imagSamples.dealloc();
	
			m_channelSamples = NULL;
	
			m_channelCount = 0;
			m_sampleCount = 0;

			assert(
				!isAllocated());
		}

		Bool _isAllocated() const
		{
			return
				m_imagSamples.isAllocated();
		}

	private:

		MySample	**m_channelSamples;
		MyMChSamples m_imagSamples;
		
		UInt
			m_channelCount,
			m_sampleCount;
	};


#endif