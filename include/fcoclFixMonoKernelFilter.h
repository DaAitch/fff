
#ifndef __fcoclfixmonokernelfilter_h__included__
#	define __fcoclfixmonokernelfilter_h__included__

#	include "fcoclFilterDesigner.h"

	class fcoclFixMonoKernelFilter : public fcoclFilterDesigner
	{
	public:
		fcoclFixMonoKernelFilter(
			const fcocl_sample *const b,
			fcocl_uint bn,
			const fcocl_sample *const a,
			fcocl_uint an,
			fcocl_uint channels) :
			m_n(max(bn, an)),
			m_channels(channels)
		{
			

			m_bs.alloc(
				m_n);
			m_as.alloc(
				m_n);

			for(
				fcocl_uint i = 0;
				i < bn;
				++i)
				m_bs.getReal()[i] = b[i];

			for(
				fcocl_uint i = 0;
				i < an; 
				++i)
				m_as.getReal()[i] = a[i];
		}

		fcocl_uint getChannelCount() const
		{
			return m_channels;
		}

		fcocl_uint getSampleCount() const
		{
			return m_n;
		}

		void createFilter(
			fcoclComplexSamplesMultiChannel &b,
			fcoclComplexSamplesMultiChannel &a)
		{
			b.alloc(
				m_channels,
				m_n);
			a.alloc(
				m_channels,
				m_n);

			for(
				fcocl_uint channel = 0;
				channel < m_channels;
				++channel)
			{
				b[channel] = m_bs;
				a[channel] = m_as;
			}
		}

	private:

		fcoclComplexSamples
			m_as,
			m_bs;
		fcocl_uint
			m_n,
			m_channels;
	};

#endif