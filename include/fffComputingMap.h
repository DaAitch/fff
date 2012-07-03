
#ifndef __fcoclcomputingmap_h__included__
#	define __fcoclcomputingmap_h__included__

#	include "_fffBase.h"
#	include "fffComputingData.h"
#	include "fffDevProps.h"

	_fff_BEGIN_NAMESPACE


		/* mapping of the data according
			to the device properties */
		class ComputingMap
		{
		public:
			template<
				class SampleType
			>
			ComputingMap(
				ComputingData<SampleType> &computingData,
				DevProps &deviceProperties,
				UInt throughputFactor
				)
			{
				// computing strategy doesnt know anything about the input size

				// N = T*M
				// L = N   - 2*(M-1)
				// L = T*M - 2*(M-1)
				// L = T*M - 2M-2
				// L = M(T-2) - 2

				m_lb2FftElementCount = _fff_next_lb2_step(computingData.getKernelElementsCount()) + throughputFactor;
				m_localWorkerCount = _fff_NTH_BIT((ULong)getLb2FftElementCount()-1); // N/2 worker <-> each butterfly, one worker

				if(m_localWorkerCount > deviceProperties.getMaxWorkerGroupCount())
					m_localWorkerCount = deviceProperties.getMaxWorkerGroupCount();
			}

			UInt getLb2FftElementCount() const
			{
				return
					m_lb2FftElementCount;
			}

			ULong getLocalWorkerCount() const
			{
				return
					m_localWorkerCount;
			}

		private:
			UInt
				m_lb2FftElementCount;		// N
											/* Size of the fft convolution.
												Also defines size of input data
												to use (equals throughput of
												that iteration) and the size
												of preceding samples (eqals
												state of the fir) */
			ULong
				m_localWorkerCount;


		};


	_fff_END_NAMESPACE

#endif /* ifndef __fcoclcomputingmap_h__included__ */