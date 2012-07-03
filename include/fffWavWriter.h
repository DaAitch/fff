
#ifndef __fffwavwriter_h__included__
#	define __fffwavwriter_h__included__

#	include "_fffBase.h"

#	include <fstream>
#	include <climits>
#	include <cstdint>

	using namespace std;

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class WavWriter
		{
		public:
			typedef
				SampleType MySample;
		private:
			std::ofstream m_out;

			uint32_t m_sampleRate;
			uint32_t m_channels;

		public:
			WavWriter(const string &file, uint32_t sampleRate, uint16_t channels) throw(...)
				: m_out(file, ios::out | ios::binary)
			{
				if(!m_out)
					throw exception((string("Could not open output file: ") + file).c_str());

				m_sampleRate = sampleRate;
				m_channels = channels;

				m_out.seekp(DATA_OFFSET);
			}

			~WavWriter() { close(); }

			void write(Sample sample)
			{
				sample *= (Sample)MAXINT32;
				const int32_t file_sample = (int32_t)(sample);
				m_out.write((const char *)&file_sample, sizeof(file_sample));
			}

		private:
			void close()
			{
				const uint32_t fileSize = (uint32_t)m_out.tellp();

				riff_header riff;
				wave_format wave;
				data_header wave_data;
				data_header data;

				// riff container
				strncpy(riff.prefix, g_riffPrefix, RIFF_PREFIX_LENGTH);
				riff.fileSize = RIFF_FILESIZE(fileSize);
				strncpy(riff.fileFormat, g_riffFileFormat, RIFF_FILEFORMAT_LENGTH);

				// wave container
				strncpy(wave_data.headerSign, g_formatHeaderSign, FORMAT_HEADERSIGN_LENGTH);
				wave_data.headerSize = WAVE_HEADERSIZE;

				wave.formatTag = PCM;
				wave.channels = m_channels;
				wave.sampleRate = m_sampleRate;
				wave.bitsPerSample = sizeof(int32_t) * CHAR_BIT;
				wave.blockAlign = m_channels * sizeof(int32_t);
				wave.bytesPerSec = m_sampleRate * wave.blockAlign;

				// data container
				strncpy(data.headerSign, g_dataHeaderSign, DATA_HEADERSIGN_LENGTH);
				data.headerSize = DATA_SIZE(fileSize);


				// write structures
				m_out.seekp(RIFF_OFFSET);
				m_out.write((const char *)&riff, sizeof(riff));
				m_out.write((const char *)&wave_data, sizeof(wave_data));
				m_out.write((const char *)&wave, sizeof(wave));
				m_out.write((const char *)&data, sizeof(data));


				m_out.close();
			}
		};

	_fff_END_NAMESPACE

#endif