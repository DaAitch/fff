//---------------------------------------------------------+
// fff/include/fffCvUbiMChBuf.h
//---------------------------------------------------------+
//  License:
//    
//    The Fast Filtering Framework implements an LTI filter
//    with Khronos Group's OpenCL.
//    Copyright (C) 2012  Philipp Renoth <fff@aitch.de>
//
//    This program is free software: you can redistribute
//    it and/or modify it under the terms of the
//    GNU General Public License as published by the
//    Free Software Foundation, either version 3 of the
//    License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will
//    be useful, but WITHOUT ANY WARRANTY; without even the
//    implied warranty of MERCHANTABILITY or
//    FITNESS FOR A PARTICULAR PURPOSE.
//    See the GNU General Public License for more details.
//
//    You should have received a copy of the
//    GNU General Public License along with this program.
//    If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------+
//!
//!	\file		fffCvUbiMChBuf.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Multi channel ubiquitous complex vector
//!				buffer, means multi channel host buffer,
//!				single channel device buffer mapping.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __fffcvubimchbuf_h_included__
#	define __fffcvubimchbuf_h_included__

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