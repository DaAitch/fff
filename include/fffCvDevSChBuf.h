
#ifndef __fcoclcomplexdevicebuffer_h__included__
#	define __fcoclcomplexdevicebuffer_h__included__

#	include "_fffBase.h"
#	include "fffClEnv.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class CvDevSChBuf
		{
		public:
			typedef SampleType MySample;
			typedef CvDevSChBuf<MySample> My;

		public:

			CvDevSChBuf(
				ClEnv &env,
				cl_mem_flags flags,
				UInt sampleCount)
				:
				m_env(
					env),
				m_sampleCount(
					0)
			{
				assert(
					!isAllocated());

				alloc(
					flags,
					sampleCount);
			}

			CvDevSChBuf(
				ClEnv &env)
				:
				m_env(
					env),
				m_sampleCount(
					0)
			{
				assert(
					!isAllocated());
			}

			virtual ~CvDevSChBuf()
			{
				dealloc();
			}

			void alloc(
				cl_mem_flags flags,
				UInt sampleCount)
			{
				dealloc();

				if(sampleCount > 0)
				{
					_alloc(
						sampleCount,
						flags);

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

			Bool isAllocated() const
			{
				if(_isAllocated())
				{
					assert(
						m_re() &&
						m_im());

					assert(
						m_sampleCount > 0);

					return True;
				}
				else
				{
					assert(
						!m_re() &&
						!m_im());

					assert(
						m_sampleCount == 0);

					return False;
				}
			}


			cl::Buffer &getReal()
			{
				return m_re;
			}

			const cl::Buffer &getReal() const
			{
				return m_re;
			}

			cl::Buffer &getImag()
			{
				return m_im;
			}

			const cl::Buffer &getImag() const
			{
				return m_im;
			}

			UInt getSampleCount() const
			{
				return m_sampleCount;
			}

			UInt getPerBufferSize() const
			{
				return getSampleCount() * sizeof(MySample);
			}

		

			void enqueueCopy(
				My &dstBuffer,
				UInt offsetSrc,
				UInt offsetDst,
				UInt sampleCount) const
			{
				fff_RTCLC_ERR_INIT();

				if(offsetDst + sampleCount > dstBuffer.getSampleCount())
					fff_THROW("invalid destination offset or sampleCount");
				if(offsetSrc + sampleCount > getSampleCount())
					fff_THROW("invalid source offset or sampleCount");

				fff_RTCLC_SEQ_CHECK_RET(
					m_env.getQueue().enqueueCopyBuffer(
						getReal(),
						dstBuffer.getReal(),
						offsetSrc,
						offsetDst,
						sampleCount * sizeof(MySample)));

				fff_RTCLC_SEQ_CHECK_RET(
					m_env.getQueue().enqueueCopyBuffer(
						getImag(),
						dstBuffer.getImag(),
						offsetSrc,
						offsetDst,
						sampleCount * sizeof(MySample)));
			}

			void enqueueCopy(
				My &dstBuffer) const
			{
				if(getLength() != dstBuffer.getSampleCount())
					fff_THROW("different length");

				enqueueCopy(
					dstBuffer,
					0,
					0,
					getSampleCount());
			}

			void enqueueCopy(
				My &dstBuffer,
				UInt offsetDst,
				UInt sampleCount) const
			{
				if(offsetDst >= dstBuffer.getSampleCount())
					fff_THROW("invalid destination offset");

				if(sampleCount > dstBuffer.getSampleCount() - offsetDst)
					fff_THROW("destination length too short");

				if(getSampleCount() < sampleCount)
					fff_THROW("source length too short");

				enqueueCopy(
					dstBuffer,
					0,
					offsetDst,
					sampleCount);
			}

			ClEnv &getEnv()
			{
				return m_clEnv;
			}

		private:

			void _alloc(
				UInt sampleCount,
				cl_mem_flags flags)
			{
				fff_RTCLC_ERR_INIT();

				assert(
					!isAllocated());
				assert(
					sampleCount > 0);
	

				m_sampleCount = sampleCount;
	
				fff_RTCLC_SEQ_CHECK(
					m_re = cl::Buffer(
						m_env.getContext(),
						flags,
						getPerBufferSize(),
						NULL,
						fff_RTCLC_ERR_PTR));

				fff_RTCLC_SEQ_CHECK(
					m_im = cl::Buffer(
						m_env.getContext(),
						flags,
						getPerBufferSize(),
						NULL,
						fff_RTCLC_ERR_PTR));

				assert(
					isAllocated());
			}

			void _dealloc()
			{
				assert(
					isAllocated());

				m_sampleCount = 0;

				m_re = cl::Buffer();
				m_im = cl::Buffer();

				assert(
					!isAllocated());
			}

			Bool _isAllocated() const
			{
				return
					m_re() &&
					m_im();
			}

		private:
			ClEnv
				m_env;
			cl::Buffer
				m_re,
				m_im;
			UInt
				m_sampleCount;



		};

	_fff_END_NAMESPACE

#endif