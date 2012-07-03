
#ifndef __fffv_h__included__
#	define __fffv_h__included__

#	include "_fffBase.h"

_fff_BEGIN_NAMESPACE

	template<
		class SampleType
	>
	class V
	{
	public:
		typedef
			SampleType MySample;
		typedef
			V<MySample> My;

	public:
		V(
			UInt sampleCount=0)
			:
			m_samples(NULL),
			m_sampleCount(0)
		{
			assert(
				!isAllocated());

			alloc(
				sampleCount);
		}

		V(
			const My &rhs)
			:
			m_samples(NULL),
			m_sampleCount(0)
		{
			copy(
				rhs);
		}

		virtual ~V()
		{
			dealloc();
		}

		My &operator=(
			const My &rhs)
		{
			copy(
				rhs);
			return fff_ME;
		}

		void copy(
			const My &rhs)
		{
			alloc(
				rhs.getSampleCount());

			for(
				UInt i = 0;
				i < rhs.getSampleCount();
				++i)
			{
				m_samples[i] = rhs[i];
			}
		}

		const MySample &operator[](
			UInt sampleIndex) const
		{
			assert(
				sampleIndex < getSampleCount());

			return
				m_samples[sampleIndex];
		}
		MySample &operator[](
			UInt sampleIndex)
		{
			assert(
				sampleIndex < getSampleCount());

			return
				m_samples[sampleIndex];
		}

		UInt getSampleCount() const
		{
			return
				m_sampleCount;
		}

		void alloc(UInt sampleCount)
		{
			dealloc();

			if(sampleCount > 0)
				_alloc(sampleCount);

			init();
		}

		void dealloc()
		{
			if(isAllocated())
				_dealloc();

			assert(
				!isAllocated());
		}

		void init()
		{
			for(
				UInt i = 0;
				i < getSampleCount();
				++i)
			{
				m_samples[i] = (MySample)0;
			}
		}

		Bool isAllocated() const
		{
			return m_samples != NULL;
		}

		My &operator*=(
			MySample factor)
		{
			for(
				UInt i = 0;
				i < getSampleCount();
				++i)
			{
				m_samples[i] *= factor;
			}

			return
				fff_ME;
		}

		My &operator/=(
			MySample divider)
		{
			for(
				UInt i = 0;
				i < getSampleCount();
				++i)
			{
				m_samples[i] /= divider;
			}

			return
				fff_ME;
		}

		MySample calcContinuousRMS() const
		{
			MySample rms = 0.f;

			for(
				UInt i = 0;
				i < getSampleCount();
				++i)
			{
				rms += m_samples[i] * m_samples[i];
			}

			return sqrt(rms / (MySample)getSampleCount());
		}

		MySample *getSamples()
		{
			return
				m_samples;
		}

		const MySample *getSamples() const
		{
			return
				m_samples;
		}

	private:

		void _alloc(UInt sampleCount)
		{
			assert(
				!isAllocated());

			m_samples = new MySample[sampleCount];
			m_sampleCount = sampleCount;
		}

		void _dealloc()
		{
			assert(
				isAllocated());

			delete [] m_samples;
			m_samples = NULL;
			m_sampleCount = 0;
		}



	private:
		MySample
			*m_samples;
		UInt
			m_sampleCount;
	};

_fff_END_NAMESPACE

#endif