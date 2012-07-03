
#ifndef __fcoclcomplexhostmappedmultichanneldevicebuffer_h__included__
#	define __fcoclcomplexhostmappedmultichanneldevicebuffer_h__included__

/*#	include "fcoclConv.h"
#	include "fcoclDebugging.h"
#	include "fcoclComplexSamplesMultiChannel.h"
#	include "fcoclComplexDeviceBuffer.h"*/

#	include "_fffBase.h"
#	include "fffICvMChBuf.h"
#	include "fffClEnv.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class CvUbiMChBuf
			: public CvDevSChBuf<SampleType>
		{
		public:
			typedef
				SampleType MySample;
			typedef
				CvUbiMChBuf<MySample> My;
			typedef
				ICvMChBuf<MySample> MyCvMChBuf;

		public:
			CvUbiMChBuf(
				ClEnv &env,
				cl_mem_flags flags,
				MyCvMChBuf &hostMultiChannel,
				Bool blockingRWs = False)
			:
				m_env(
					env),
				CvDevSChBuf(
					env),
				m_multiChannel(
					hostMultiChannel),
				m_blocking(
					blockingRWs)
			{
				CvDevSChBuf::alloc(
					flags,
					hostMultiChannel.getSampleCount());

				assert(
					isAllocated());
			}

			virtual ~CvUbiMChBuf()
			{
				dealloc();
			}

			void enqueueHostUpdate(
				UInt channel)
			{
				fff_RTCLC_ERR_INIT();

				const cl::CommandQueue &queue = m_env.getQueue();
	
				fff_RTCLC_SEQ_CHECK_RET(
					queue.enqueueReadBuffer(
						getReal(),
						isBlocking(),
						0,
						getPerBufferSize(),
						m_multiChannel.getRawReal(channel)));

				fff_RTCLC_SEQ_CHECK_RET(
					queue.enqueueReadBuffer(
						getImag(),
						isBlocking(),
						0,
						getPerBufferSize(),
						m_multiChannel.getRawImag(channel)));
			}

			void enqueueDeviceUpdate(
				UInt channel) const
			{
				fff_RTCLC_ERR_INIT();

				const cl::CommandQueue &queue = m_env.getQueue();
	
				assert(
					queue());

				fff_RTCLC_SEQ_CHECK_RET(
					queue.enqueueWriteBuffer(
						getReal(),
						isBlocking(),
						0,
						getPerBufferSize(),
						m_multiChannel.getRawReal(channel)));

				fff_RTCLC_SEQ_CHECK_RET(
					queue.enqueueWriteBuffer(
						getImag(),
						isBlocking(),
						0,
						getPerBufferSize(),
						m_multiChannel.getRawImag(channel)));
			}

			const ICvMChBuf<MySample> &getMultiChannel() const
			{
				return m_multiChannel;
			}

			void setBlocking(
				Bool blockingRWs)
			{
				m_blocking = blockingRWs;
			}

			Bool isBlocking() const
			{
				return m_blocking;
			}

		private:

		private:
			ClEnv
				&m_env;
			MyCvMChBuf
				&m_multiChannel;
			Bool
				m_blocking;
		};

	_fff_END_NAMESPACE

#endif