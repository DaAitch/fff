/*----------------------------------------------------------
 *    The Fast Filtering Framework implements an LTI filter
 *    with Khronos Group's OpenCL.
 *    Copyright (C) 2012  Philipp Renoth
 *----------------------------------------------------------
 *    This program is free software: you can redistribute
 *    it and/or modify it under the terms of the
 *    GNU General Public License as published by the
 *    Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the
 *    GNU General Public License along with this program.
 *    If not, see <http://www.gnu.org/licenses/>.
 *--------------------------------------------------------*/

#ifndef ___fffcvhstmchbuf_h__included__
#	define ___fffcvhstmchbuf_h__included__

#	include "_fffBase.h"
#	include "fffCvHstSChBuf.h"
#	include "fffICvMChBuf.h"
#	include "fffCv.h"


	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class CvHstMChBuf
			: public ICvMChBuf<SampleType>
		{
		public:
			typedef
				SampleType	MySample;
			typedef
				CvHstMChBuf<MySample> My;
			typedef
				CvHstSChBuf<MySample> MySamples;
			typedef
				std::vector<MySamples> MyChannels;
		

		public:
			CvHstMChBuf()
				:
				m_sampleCount(0),
				m_channelSamples(0)
			{
				assert(
					!isAllocated());
			}

			CvHstMChBuf(
				UInt channelCount,
				UInt sampleCount)
				: m_sampleCount(0),
				m_channelSamples(0)
			{
				assert(
					!isAllocated());

				alloc(
					channelCount,
					sampleCount);
			}

			CvHstMChBuf(
				const My &rhs)
			{
				copy(
					rhs);
			}

			virtual ~CvHstMChBuf()
			{
				dealloc();
			}

			My &operator=(
				const My &rhs)
			{
				copy(
					rhs);

				return fcocl_ME;
			}

			void copy(
				const My &rhs)
			{
				if(this != &rhs)
				{
					dealloc();

					if(rhs.isAllocated())
					{
						alloc(
							rhs.getChannelCount(),
							rhs.getSampleCount());

						for(
							UInt channel = 0;
							channel < getChannelCount();
							++channel)
						{
							set(
								channel,
								rhs[channel]);
						}

						assert(
							isAllocated());
					}
					else
						assert(
							!isAllocated());
				}
			}

		
			void alloc(
				UInt channelCount,
				UInt sampleCount)
			{
				dealloc();

				assert(
					!isAllocated());

				if(channelCount > 0 && sampleCount > 0)
				{
					_alloc(
						channelCount,
						sampleCount);

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


			void init()
			{
				if(!isAllocated())
					fcocl_THROW("not allocated");

				assert(
					isAllocated());

				_init();

				assert(
					isAllocated());
			}

			Bool isAllocated() const
			{
				if(_isAllocated())
				{
					assert(
						m_sampleCount > 0);

					for(
						UInt channel = 0;
						channel < getChannelCount();
						++channel)
					{
						assert(
							m_channelSamples[channel].isAllocated());
					}

					return True;
				}
				else
				{
					assert(
						m_channelSamples.size() == 0);
					assert(
						m_sampleCount == 0);

					return False;
				}
			}

			const MySamples &operator[](
				UInt channelIndex) const
			{
				return
					m_channelSamples[channelIndex];
			}

			MySamples &operator[](
				UInt channelIndex)
			{
				return
					m_channelSamples[channelIndex];
			}
			

			UInt getChannelCount() const
			{
				return
					m_channelSamples.size();
			}

			UInt getSampleCount() const
			{
				return
					m_sampleCount;
			}

			MySample *getRawReal(
				UInt channelIndex)
			{
				return
					fff_ME[channelIndex].getReal().getSamples();
			}

			MySample *getRawImag(
				UInt channelIndex)
			{
				return
					fff_ME[channelIndex].getImag().getSamples();
			}

			const MySample *getRawReal(
				UInt channelIndex) const
			{
				return
					fff_ME[channelIndex].getReal().getSamples();
			}

			const MySample *getRawImag(
				UInt channelIndex) const
			{
				return
					fff_ME[channelIndex].getImag().getSamples();
			}

			My &operator*=(
				MySample factor)
			{
				for(
					UInt channel = 0;
					channel < getChannelCount();
					++channel)
				{
					fff_ME[channel] *= factor;
				}

				return fff_ME;
			}

			My &operator/=(
				MySample factor)
			{
				for(
					UInt channel = 0;
					channel < getChannelCount();
					++channel)
				{
					fff_ME[channel] /= factor;
				}

				return fff_ME;
			}

			void normalize()
			{
				if(!isAllocated())
					fff_THROW("not allocated");

				MySample maxGain = 0.f;
				
				for(
					UInt channel = 0;
					channel < getChannelCount();
					++channel)
				{
					maxGain = max(
						maxGain, fff_ME[channel].calcMaxGain());
				}

				fff_ME /= maxGain;
			}



		private:

		

			void _alloc(
				UInt channelCount,
				UInt sampleCount)
			{
				assert(
					!isAllocated());

				assert(
					channelCount > 0 &&
					sampleCount > 0);

				m_sampleCount = sampleCount;
				m_channelSamples.resize(channelCount);

				for(
					UInt channel = 0;
					channel < getChannelCount();
					++channel)
				{
					m_channelSamples[channel] = 
						CvHstSChBuf<MySample>(sampleCount);
				}

				assert(
					isAllocated());
			}

			void _init()
			{
				assert(
					isAllocated());

				for(
					fcocl_uint channel = 0;
					channel < getChannelCount();
					++channel)
				{
					m_channelSamples[channel].init();
				}

				assert(
					isAllocated());
			}

			void _dealloc()
			{
				assert(
					isAllocated());

				for(
					UInt channel = 0;
					channel < getChannelCount();
					++channel)
				{
					m_channelSamples[channel].dealloc();
				}

				m_channelSamples.clear();
				m_sampleCount = 0;

				assert(
					!isAllocated());
			}

			Bool _isAllocated() const
			{
				return
					m_channelSamples.size() != 0;
			}

		private:

			MyChannels
				m_channelSamples;
			UInt
				m_sampleCount;
		};

	_fff_END_NAMESPACE

#endif