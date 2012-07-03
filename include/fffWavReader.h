
#ifndef __fcoclwavreader_h__included__
#	define __fcoclwavreader_h__included__

#	include <fstream>
#	include <climits>
#	include <cstdint>

#	include "_fffBase.h"

#	include "fffWave.h"
#	include "fffCvHstMChBuf.h"

	_fff_BEGIN_NAMESPACE

		template<
			class SampleType
		>
		class WavReader
		{
		public:
			typedef
				SampleType MySample;
		private:

			union wavsample
			{
				UInt bytes;
				Int value;
			};

			uint32_t	m_sampleCount;	// amount of samples of each channel
			uint16_t	m_sampleDepth;	// byte depth of one sample (only full 8 bits allowed)

			riff_header m_riff;
			wave_format m_format;
			data_header m_data;

			std::ifstream m_file;

		public:
			WavReader(const String &file) throw (...)
				: m_file(file, ios::in|ios::binary)
			{

				// initialize some members
				m_sampleCount = 0;
				m_sampleDepth = 0;
	
				printf("Start reading file: %s\n", file.c_str());

				// open file and check if it is open
				if(!m_file)
					throw exception((string("Could not open ") + file).c_str());

				m_file.seekg(0, ios::beg);

				// read riff header and check prefix
				m_file.read((char *)&m_riff, sizeof(m_riff));
				//print_riff(m_riff);

				if(strncmp(m_riff.prefix, g_riffPrefix, sizeof(m_riff.prefix)) != 0)
					throw exception((string("No Riff Container file: ") + file).c_str());

				// check file format
				if(strncmp(m_riff.fileFormat, g_riffFileFormat, sizeof(m_riff.fileFormat)) != 0)
					throw exception((string("No WAVE file format file: ") + file).c_str());

				m_file.seekg(sizeof(m_riff), ios::beg);

				// read format header and check if it has the PCM
				data_header formatHeader;
				m_file.read((char *)&formatHeader, sizeof(formatHeader));
				ios::pos_type pos = m_file.tellg();

				m_file.read((char *)&m_format, sizeof(m_format));
				//print_wave(m_format);

				if(m_format.formatTag != PCM )
					throw exception((string("No PCM Wave file: ") + file).c_str());

				// check format header sign
				if(strncmp(formatHeader.headerSign, g_formatHeaderSign, sizeof(formatHeader.headerSign)) != 0)
					throw exception((string("Unknown format block in file: ") + file).c_str());
	
				m_file.seekg((std::streamoff)pos + formatHeader.headerSize, ios::beg);

				// true if data block found
				bool dataBlock = false;

				// search data block
				while(!!m_file)
				{
					// read data header and check data header sign
					m_file.read((char *)&m_data, sizeof(m_data));
					//print_data(m_data);

					// if not is the data block, jump to next header
					if(strncmp(m_data.headerSign, g_dataHeaderSign, sizeof(m_data.headerSign)) != 0)
						m_file.seekg(m_data.headerSize, ios::cur);
					else
					{
						// data block found, so exit
						dataBlock = true;
						break;
					}
			
				}

				if(!m_file)
					throw exception((string("Read error in file: ") + file).c_str());

				// if no data block found, there are no samples
				if(!dataBlock)
					throw exception((string("Unknown data block in file: ") + file).c_str());

	
	
	

				// save some information
				m_sampleDepth = (m_format.bitsPerSample + 7) >> 3;			// or "/ 8", full 8 bits
				m_sampleCount = m_data.headerSize / m_sampleDepth / m_format.channels;

				if(m_format.channels != 2)
					throw exception(string("Only 2 channels allowed.").c_str());

	
			}

			void readFile(CvHstMChBuf<MySample> &multiChannel)
			{
				multiChannel.alloc(
					m_format.channels,
					m_sampleCount);

				wavsample file_sample;
	
				Sample maxAmpl = (Sample)MAXINT32;
				UInt shift = (sizeof(int32_t) - m_sampleDepth)<<3;

				// wave file consists of frames, each frame one sample per channel
				for(uint32_t s = 0; s < m_sampleCount; ++s)
				{
					for(uint16_t c = 0; c < m_format.channels; ++c)
					{
						file_sample.bytes = 0x00000000;

						m_file.read((char*)&file_sample.bytes, m_sampleDepth);
						if(!m_file)
							throw exception("Bad wave format in file");


						file_sample.bytes <<= shift;
			
						Sample smp = (Sample)file_sample.value / maxAmpl;

						multiChannel.getRawReal(c)[s] = 
							smp;

					}
				}

				m_file.close();
			}

			uint16_t getChannelCount() const { return m_format.channels; }
			uint32_t getSampleCount() const { return m_sampleCount; }
			uint32_t getSampleRate() const { return m_format.sampleRate; }

		};

	_fff_END_NAMESPACE

#endif