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

#ifndef __fffwave_h__included__
	#define __fffwave_h__included__

	#include <stdint.h>


	// riff prefix "RIFF"
	#define RIFF_PREFIX_LENGTH				4
	static const char *g_riffPrefix			= "RIFF";

	// riff file format "WAVE"
	#define RIFF_FILEFORMAT_LENGTH			4
	static const char *g_riffFileFormat		= "WAVE";

	// format header sign
	#define FORMAT_HEADERSIGN_LENGTH		4
	static const char *g_formatHeaderSign	= "fmt ";

	// data header sign
	#define DATA_HEADERSIGN_LENGTH			4
	static const char *g_dataHeaderSign		= "data";


	// dont use padding, since we want
	// to read structure out of the file
	#pragma pack(push)
	#pragma pack(1)

	// riff container header
	struct riff_header
	{
		char prefix[RIFF_PREFIX_LENGTH];
		uint32_t fileSize;
		char fileFormat[RIFF_FILEFORMAT_LENGTH];
	};

	// wave format header
	struct wave_format
	{
		uint16_t formatTag;
		uint16_t channels;
		uint32_t sampleRate;
		uint32_t bytesPerSec;
		uint16_t blockAlign;
		uint16_t bitsPerSample;
	};

	// one and only data header
	struct data_header
	{
		char headerSign[DATA_HEADERSIGN_LENGTH];
		uint32_t headerSize;
	};

	#pragma pack(pop)

	#define RIFF_OFFSET				0
	#define WAVE_OFFSET				12
	#define DATA_OFFSET				44

	// Offsets and sizes
	#define RIFF_FILESIZE_OFFSET	4
	#define DATA_SIZE_OFFSET		40

	#define RIFF_FILESIZE(FILESIZE) ((FILESIZE)-8)
	#define WAVE_HEADERSIZE			16
	#define DATA_SIZE(FILESIZE)		((FILESIZE)-DATA_OFFSET)

	// wikipedia.org adapted consts for the wave format
	#define PCM						0x0001
	#define MS_ADPCM				0x0002
	#define IEEE_FLOAT				0x0003
	#define IBM_CVSD				0x0005
	#define ALAW					0x0006
	#define MULAW					0x0007
	#define OKI_ADPCM				0x0010
	#define DVI_IMA_ADPCM			0x0011
	#define MEDIASPACE_ADPCM		0x0012
	#define SIERRA_ADPCM			0x0013
	#define G723_ADPCM				0x0014
	#define DIGISTD					0x0015
	#define DIGIFIX					0x0016
	#define DIALOGIC_OKI_ADPCM		0x0017
	#define YAMAHA_ADPCM			0x0020
	#define SONARC					0x0021
	#define DSPGROUP_TRUESPEECH		0x0022
	#define ECHOSC1					0x0023
	#define AUDIOFILE_AF36			0x0024
	#define APTX					0x0025
	#define AUDIOFILE_AF10			0x0026
	#define DOLBY_AC2				0x0030
	#define GSM610					0x0031
	#define ANTEX_ADPCME			0x0033
	#define CONTROL_RES_VQLPC_1		0x0034
	#define CONTROL_RES_VQLPC_2		0x0035
	#define DIGIADPCM				0x0036
	#define CONTROL_RES_CR10		0x0037
	#define NMS_VBXADPCM			0x0038
	#define CS_IMAADPCM				0x0039
	#define G721_ADPCM				0x0040
	#define MPEG_1_Layer_I_II		0x0050
	#define MPEG_1_Layer_III		0x0055
	#define XBOX_ADPCM				0x0069
	#define CREATIVE_ADPCM			0x0200
	#define CREATIVE_FASTSPEECH8	0x0202
	#define CREATIVE_FASTSPEECH10	0x0203
	#define FM_TOWNS_SND			0x0300
	#define OLIGSM					0x1000
	#define OLIADPCM				0x1001
	#define OLICELP					0x1002
	#define OLISBC					0x1003
	#define OLIOPR					0x1004



	#define MAX_AMPLITUDE			((2<<16)-1)


#endif