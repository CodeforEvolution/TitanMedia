#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeMediaFileManager.h"
#include "EMMediaGlobals.h"
#include "EMBeMediaUtility.h"
#include "EMMediaFile.h"
#include "EMWaveFileReader.h"
#include "EMWaveFileWriter.h"
#include "EMMediaFileRepository.h"
#include "EMMediaFormat.h"
#include "EMAudioConverter.h"
#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"

#include "DirectoryIDs.h"
#include <StorageKit.h>
#include <MediaKit.h>



EMBeMediaFileManager::EMBeMediaFileManager()
	:	EMMediaFileManager(),
		m_opNewFile(NULL)
{
	EMBeMediaUtility::push(this, "EMBeMediaFileManager");
}

EMBeMediaFileManager::~EMBeMediaFileManager()
{
	EMBeMediaUtility::pop("EMBeMediaFileManager");
}

int32 EMBeMediaFileManager::GetFileTypeForE(const string* p_opFileName)
{
	entry_ref sRef;
	int32 vReturnType = EM_TYPE_UNKNOWN;
	get_ref_for_path(p_opFileName -> c_str(), &sRef);
	BMediaFile* opFile = new BMediaFile(&sRef, B_MEDIA_FILE_UNBUFFERED | B_MEDIA_FILE_NO_READ_AHEAD);
	status_t vError = opFile -> InitCheck();
	if(vError != B_OK)
	{	
		return EM_TYPE_UNKNOWN;
		delete opFile;
//		string oException("Unsupported file type! Cannot do anything with the file");
//		throw EMException(EM_DIALOG_NEVER, &oException);
	}
	media_file_format sFileFormat;
	vError = opFile -> GetFileFormatInfo(&sFileFormat);
	if(vError != B_OK)
	{
		delete opFile;
		return EM_TYPE_UNKNOWN;
//		string oException("Unsupported file type! Cannot do anything with the file");
//		throw EMException(EM_DIALOG_NEVER, &oException);
	}

	int32 vCount = opFile -> CountTracks();

	for(int32 vIndex = 0; vIndex < vCount; vIndex++)
	{
		media_format sFormat;
		BMediaTrack* opTrack = opFile -> TrackAt(vIndex);
		vError = opTrack -> InitCheck();
		if(vError != B_OK)
		{
			delete opFile;
			return EM_TYPE_UNKNOWN;
//			string oException("Unsupported file type! Cannot do anything with the file");
//			throw EMException(EM_DIALOG_NEVER, &oException);
		}
		if(opTrack -> EncodedFormat(&sFormat) != B_OK)
		{
			delete opFile;
			return EM_TYPE_UNKNOWN;
//			string oException("Unsupported file type! Cannot do anything with the file");
//			throw EMException(EM_DIALOG_NEVER, &oException);
		}
		if(sFormat.type == B_MEDIA_RAW_AUDIO)
			vReturnType = vReturnType | EM_TYPE_RAW_AUDIO;
		if(sFormat.type == B_MEDIA_RAW_VIDEO)
			vReturnType = vReturnType | EM_TYPE_RAW_VIDEO;
		if(sFormat.type == B_MEDIA_ENCODED_AUDIO)
			vReturnType = vReturnType | EM_TYPE_ENCODED_AUDIO;
		if(sFormat.type == B_MEDIA_ENCODED_VIDEO)
			vReturnType = vReturnType | EM_TYPE_ENCODED_VIDEO;
		opFile -> ReleaseTrack(opTrack);
	}

	delete opFile;
	return vReturnType;
}
/*
bool EMBeMediaFileManager::IsWave(const string* p_opFileName)
{
	entry_ref sRef;

	if((GetFileTypeFor(p_opFileName) & EM_TYPE_ANY_VIDEO) > 0)
		return false;
	
	BEntry oEntry(p_opFileName -> c_str(), false);
	oEntry.GetRef(&sRef);
	BMediaFile* opFile = new BMediaFile(&sRef, B_MEDIA_FILE_UNBUFFERED);
	
	status_t vError = opFile -> InitCheck();
	if(vError != B_OK)
		EMDebugger("2 ERROR! Could not initialize media file!");

	media_file_format sFileFormat;
	vError = opFile -> GetFileFormatInfo(&sFileFormat);
	if(vError != B_OK)
		EMDebugger("ERROR! Could not read file format from file!");

	bool vIsWave = false;
	if(strncmp(sFileFormat.short_name, "wave", 4) == 0)
		vIsWave = true;
			
	delete opFile;
	return vIsWave;
}

bool EMBeMediaFileManager::HasVideo(const string* p_opFileName)
{
	if((GetFileTypeFor(p_opFileName) & EM_TYPE_ANY_VIDEO) > 0)
		return true;
	return false;
}

bool EMBeMediaFileManager::HasAudio(const string* p_opFileName)
{
	if((GetFileTypeFor(p_opFileName) & EM_TYPE_ANY_AUDIO) > 0)
		return true;
	return false;
}*/

int64 EMBeMediaFileManager::CompareNumFramesInFile(const string* p_opFileName, int64 p_vNumFramesToCompareWith)
{
	EMWaveFileReader* opPeaksFile = new EMWaveFileReader(*p_opFileName);
	opPeaksFile -> InitCheckE();
	bool vResult = false;
	if(opPeaksFile -> NumberOfFramesInFile() == p_vNumFramesToCompareWith)
		vResult = true;
	else
		;//;//emout_commented_out_4_release << "ERROR! File exists, but has " << opPeaksFile -> NumberOfFramesInFile() << " frame(s) instead of " << p_vNumFramesToCompareWith << endl;
	delete opPeaksFile;
}

bool EMBeMediaFileManager::FileExists(const string* p_opFileName)
{
	BFile oFile(p_opFileName -> c_str(), B_READ_ONLY);
	if(oFile.InitCheck() != B_OK)
		return false;
	return true;
}

bool EMBeMediaFileManager::ConvertFileExistsFor(const string* p_opSourceFileName)
{
	string oConverttFileName = EMBeMediaUtility::MakeConvertFileName(*p_opSourceFileName);

	if(! FileExists(&oConverttFileName))
		return false;
		
	//Find out what the expected numframes is...
	EMWaveFileReader* opFile = new EMWaveFileReader(*p_opSourceFileName);
	opFile -> InitCheckE();
	int64 vOriginalNumFrames = opFile -> NumberOfFramesInFile();
	
	EMMediaFormat oSourceFormat = *(opFile -> GetFormat());
	EMMediaFormat oTargetFormat(EM_TYPE_RAW_AUDIO);
	delete opFile;

	int64 vExpectedNumPeaksFrames = static_cast<int64>(vOriginalNumFrames / (oSourceFormat.m_vFrameRate / oTargetFormat.m_vFrameRate));

	//Compare the expected number of frames now...
	return CompareNumFramesInFile(&oConverttFileName, vExpectedNumPeaksFrames);
}

bool EMBeMediaFileManager::PeaksFileExistsFor(const string* p_opFileName)
{
	string oPeaksFileName = EMBeMediaUtility::MakePeaksFileName(*p_opFileName); //ParseFullPath(oNoPathFileName, DIRECTORY_ID_PROJECT_DATA);

	if(! FileExists(&oPeaksFileName))
		return false;
		
	//Find out what the expected numframes is...
	EMWaveFileReader* opFile = new EMWaveFileReader(*p_opFileName);
	opFile -> InitCheckE();
	int64 vOriginalNumFrames = opFile -> NumberOfFramesInFile();
	delete opFile;
	int64 vExpectedNumPeaksFrames = static_cast<int64>(floor(vOriginalNumFrames / EM_LOW_RES_AUDIO_SKIP_RATE));

	//Compare the expected number of frames now...
	return CompareNumFramesInFile(&oPeaksFileName, vExpectedNumPeaksFrames);
}

bool EMBeMediaFileManager::ExtractFileExistsFor(const string* p_opFileName)
{
	string oExtractFileName = EMBeMediaUtility::MakeExtractFileName(*p_opFileName); //ParseFullPath(oNoPathFileName, DIRECTORY_ID_AUDIO_DATA_USED);

	if(! FileExists(&oExtractFileName))
		return false;

	//Find out what the expected numframes is...

	entry_ref sRef;

	if(p_opFileName == NULL)
		EMDebugger("ERROR! File name is NULL!");
	
	BEntry oEntry(p_opFileName -> c_str(), false);
	oEntry.GetRef(&sRef);
	BMediaFile* opFile = new BMediaFile(&sRef);
	
	status_t vError = opFile -> InitCheck();
	if(vError != B_OK)
		EMDebugger("ERROR! Could not initialize media file!");
	media_file_format sFileFormat;
	vError = opFile -> GetFileFormatInfo(&sFileFormat);
	if(vError != B_OK)
		EMDebugger("ERROR! Could not read file format from file!");
	int32 vCount = opFile -> CountTracks();
	int64 vOriginalNumFrames = 0;//opFile -> NumberOfFramesInFile();
	for(int32 vIndex = 0; vIndex < vCount; vIndex++)
	{
		media_format sFormat;
		BMediaTrack* opTrack = opFile -> TrackAt(vIndex);
		vError = opTrack -> InitCheck();
		if(vError != B_OK)
			EMDebugger("ERROR! Could not initialize BMediaTrack!");
		if(opTrack -> EncodedFormat(&sFormat) != B_OK)
			EMDebugger("ERROR! Could not get format of BMediaTrack!");
		
		if(sFormat.type == B_MEDIA_RAW_AUDIO || sFormat.type == B_MEDIA_ENCODED_AUDIO)
		{
			vOriginalNumFrames = opTrack -> CountFrames();
			opFile -> ReleaseTrack(opTrack);
			if(vIndex == vCount-1)
				break;	
		}
		opFile -> ReleaseTrack(opTrack);
	}	
	delete opFile;
	
	if(vOriginalNumFrames <= 0)
		EMDebugger("0 frames in media track!?");
	
	//Compare the expected number of frames now...
	return CompareNumFramesInFile(&oExtractFileName, vOriginalNumFrames);
}

string EMBeMediaFileManager::CreatePeaksFileE(const string* p_opFileName)
{
	string oPeaksFileName = EMBeMediaUtility::MakePeaksFileName(*p_opFileName);

	if(PeaksFileExistsFor(p_opFileName))
		return oPeaksFileName;

	register bool vShouldLeaveLoop = false; 
	EMWaveFileReader* opHiResFile = new EMWaveFileReader(*p_opFileName);
	EMWaveFileWriter* opLoResFile = new EMWaveFileWriter(oPeaksFileName);

	opHiResFile -> InitCheckE();
	opLoResFile -> InitCheckE();

	int64 vTotalLength = opHiResFile -> GetSize();
	int64 vFinishedLength = static_cast<int64>(floor(floor(vTotalLength / (EM_LOW_RES_AUDIO_SKIP_RATE * 4)) * 2));
	
	int vNumHiResSamplesToRead = 1000;
	typedef int8 LORES_SAMPLE_TYPE;
	int32 vHiResSampleSize = sizeof(signed short);
	int32 vLoResSampleSize = sizeof(LORES_SAMPLE_TYPE);

 	int vNumLoResSamplesToRead = vNumHiResSamplesToRead / EM_LOW_RES_AUDIO_SKIP_RATE;
	signed short* vpHiResArray = new signed short[vNumHiResSamplesToRead];
	LORES_SAMPLE_TYPE* vpLoResArray = new LORES_SAMPLE_TYPE[vNumLoResSamplesToRead];
	int32 vHiResBytes = vNumHiResSamplesToRead * vHiResSampleSize;
	int64 vHiResBytesActuallyRead = 0;
	int32 vNumChan = opHiResFile -> GetFormat() -> m_vNumChannels;
	int64 vWriteCount = 0;
	int64 vPosition = 0;
	int64 vCount = 0;
	while(! vShouldLeaveLoop)
	{
		vHiResBytesActuallyRead = vHiResBytes;
		opHiResFile -> ReadData(vpHiResArray, vHiResBytesActuallyRead, vPosition, 0, vHiResSampleSize);
		if(vHiResBytesActuallyRead != vHiResBytes)
			vShouldLeaveLoop = true;
		if(vHiResBytesActuallyRead == 0)
			break;

		vPosition += vHiResBytesActuallyRead;
		register int32 vLoResPos = 0;
		int64 a = (int64) floor(vHiResBytesActuallyRead / vHiResSampleSize); //num samples read
		for(vLoResPos = 0; vLoResPos < floor(a / EM_LOW_RES_AUDIO_SKIP_RATE); vLoResPos += vNumChan)
			for(register int32 vChan = 0; vChan < vNumChan; vChan++)
				vpLoResArray[vLoResPos + vChan] = static_cast<LORES_SAMPLE_TYPE>((vpHiResArray[(vLoResPos * EM_LOW_RES_AUDIO_SKIP_RATE) + vChan] / 32768.0) * 127.0);

		int64 vSize = vLoResPos;

		//Make sure we don't write too much!
		if(vWriteCount + vSize > vFinishedLength)
			vSize -= (vWriteCount + vSize) - vFinishedLength;

		opLoResFile -> WriteData(vpLoResArray, vSize);
		vWriteCount += vSize;
		
		if((vCount % 10) == 0)
		{
			char vpPercentage[10];
			sprintf(vpPercentage, "%d%%", (int) (100 * (vPosition / (float) vTotalLength)));
			string oString = string("Analyzing waveform: ") + vpPercentage;
			EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, const_cast<char*>(oString.c_str()));
		}
		vCount++;
	}

	EMMediaFormat oFormat(*opHiResFile -> GetFormat());
	oFormat.m_vBytesPerSample = vLoResSampleSize;
	opLoResFile -> WriteHeaderE(&oFormat);

	delete opHiResFile;
	delete opLoResFile;
	delete [] vpHiResArray;
	delete [] vpLoResArray;
	return string(oPeaksFileName);
}
bool EMBeMediaFileManager::CheckFileForVideoFormat(const string* p_opFileName)
{
	entry_ref m_sRef;
	BEntry oEntry(p_opFileName -> c_str(), false);
	oEntry.GetRef(&m_sRef);

	BMediaFile* opFile = new BMediaFile(&m_sRef, B_MEDIA_FILE_UNBUFFERED);
	//Point vidtrack to the Video-Track (temporary static at 0) (input file)
	//It is not always that the video data is on track 0?
	int32 tracks = opFile->CountTracks();
	media_format sFormat;
	BMediaTrack* opTrackOut = NULL;
	for (int32 i = 0; i < tracks; i++) 
	{
		opTrackOut = opFile -> TrackAt(i);
				
		//m_opTrackOut->EncodedFormat(&outfmt);
		//We wannt RAW Video.. nothing else can be handled :)
		//So we will have to negotiate for RAW_VIDEO through DecodeFormat(..)
		sFormat.type = B_MEDIA_RAW_VIDEO;
		sFormat.u.raw_video = media_raw_video_format::wildcard;
		status_t vErrorCode = opTrackOut -> DecodedFormat(&sFormat);
		if(vErrorCode != B_OK)
		{
			opFile -> ReleaseTrack(opTrackOut);
			opFile -> CloseFile();
			delete opFile;
			return false;
		}

		if(sFormat.IsVideo())
		{
			opFile -> ReleaseTrack(opTrackOut);
			opFile -> CloseFile();
			delete opFile;
			return true;
		}
		
	}
	if(opTrackOut != NULL)
		opFile -> ReleaseTrack(opTrackOut);
	opFile -> CloseFile();
	delete opFile;
	return false;
}

string EMBeMediaFileManager::ExtractAudioE(const string* p_opFileName)
{
	if(ExtractFileExistsFor(p_opFileName))
	{
//		string oNoPathFileName = EMBeMediaUtility::GetFileName(*p_opFileName);
//		oNoPathFileName += ".extract.wav";
		string oExtractFileName = EMBeMediaUtility::MakeExtractFileName(*p_opFileName); //ParseFullPath(oNoPathFileName, DIRECTORY_ID_AUDIO_DATA_USED);
		return oExtractFileName;
	}

	entry_ref sRef;
	string oNewFileName;

	if(p_opFileName == NULL)
		EMDebugger("ERROR! File name is NULL!");
	
	BEntry oEntry(p_opFileName -> c_str(), false);
	oEntry.GetRef(&sRef);

	BMediaFile* opFile = new BMediaFile(&sRef);

	status_t vError = opFile -> InitCheck();
	if(vError != B_OK)
		EMDebugger("ERROR! Could not initialize media file!");


	media_file_format sFileFormat;
	vError = opFile -> GetFileFormatInfo(&sFileFormat);
	if(vError != B_OK)
		EMDebugger("ERROR! Could not read file format from file!");

	int32 vCount = opFile -> CountTracks();
	
	for(int32 vIndex = 0; vIndex < vCount; vIndex++)
	{
		media_format sFormat;
		BMediaTrack* opTrack = opFile -> TrackAt(vIndex);
		vError = opTrack -> InitCheck();
		if(vError != B_OK)
			EMDebugger("ERROR! Could not initialize BMediaTrack!");
		
		if(opTrack -> EncodedFormat(&sFormat) != B_OK)
			EMDebugger("ERROR! Could not get format of BMediaTrack!");

		
		if((sFormat.type == B_MEDIA_RAW_AUDIO || sFormat.type == B_MEDIA_ENCODED_AUDIO) && vIndex == vCount-1)
		{
			//media_format sEncodedFormat;
			media_format sDecodedFormat;

			//Create the decode-stuff
			memcpy(&sDecodedFormat, &sFormat, sizeof(sDecodedFormat));
			status_t vErr = opTrack -> DecodedFormat(&sDecodedFormat);

			if(vErr != B_OK)
			{
				emerr << "ERROR! Format negotiation failed: " << strerror(vErr) << endl;
			} 

			EMMediaFormat oDecodedFormat(EM_TYPE_RAW_AUDIO);
			oDecodedFormat.CreateFormat(&sDecodedFormat);

			int64 vNumBytes = oDecodedFormat.m_vBufferSizeBytes;
			char* vpData = new char[vNumBytes];
			memset(vpData, 0, vNumBytes);

			int64 vFrames = vNumBytes / (oDecodedFormat.m_vBytesPerSample * oDecodedFormat.m_vNumChannels);
			vFrames=0;
			int64 vBytesPerFrame = oDecodedFormat.m_vNumChannels * oDecodedFormat.m_vBytesPerSample;
			media_header sHeader;
			
			int64 vNumProcessedFrames = 0;
			int64 vTotalTime = EMBeMediaUtility::FramesToTime(opTrack -> CountFrames(), &oDecodedFormat);

			int64 frame_count;
			frame_count = 0;
			int64 vCount = 0;
			while(true)
			{
				status_t vError = opTrack -> ReadFrames(vpData, &vFrames, &sHeader);
				if( vError !=B_OK)
				{
					if(frame_count==1)
						break;
					media_format rawAudioFormat;
					rawAudioFormat.type = B_MEDIA_RAW_AUDIO;
					rawAudioFormat.u.raw_audio = media_raw_audio_format::wildcard;
					status_t err = opTrack -> DecodedFormat(&rawAudioFormat);
					if( err !=B_OK)
					{
						break;
					}
					else
					oDecodedFormat.CreateFormat(&rawAudioFormat);
					frame_count = 1;
				}
				else
				{
					if(m_opNewFile == NULL)
					{
						oNewFileName = EMBeMediaUtility::MakeExtractFileName(*p_opFileName); //ParseFullPath(oNoPathFileName, DIRECTORY_ID_AUDIO_DATA_USED);			
						m_opNewFile = new EMWaveFileWriter(oNewFileName);
						m_opNewFile -> InitCheckE();
					}
				
					m_opNewFile -> WriteData(vpData, vFrames * vBytesPerFrame);
					vNumProcessedFrames += vFrames;
					
					if((vCount % 50) == 0)
					{
						char vpPercentage[10];
						sprintf(vpPercentage, "%d%%",  (int) (100 * (EMBeMediaUtility::FramesToTime(vNumProcessedFrames, &oDecodedFormat) / (float) vTotalTime)));
						string oString = string("Extracting audio data: ") + vpPercentage;
						EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, const_cast<char*>(oString.c_str()));
					}
					vCount++;
				}
			}
			if(vNumProcessedFrames <= 0)
			{
				return string("Noname");;
				EMDebugger("ERROR! Could not read any frames from the audio track, using BMediaTrack::ReadFrames(...). Very weird. Unsupported audio format perhaps?");
			}
			m_opNewFile -> WriteHeaderE(&oDecodedFormat);
			delete m_opNewFile;
			m_opNewFile = NULL;
			opFile -> ReleaseTrack(opTrack);
			opFile -> CloseFile();
			delete opFile;
			return oNewFileName;
		}
	}

	opFile -> CloseFile();
	delete opFile;
	return string("Noname");
}

bool EMBeMediaFileManager::FormatsMatch(const string* p_opQuestionedFileName, const EMMediaFormat* p_oWantedFormat)
{
	EMWaveFileReader* opHiResFile = new EMWaveFileReader(*p_opQuestionedFileName);
	opHiResFile -> InitCheckE();
	EMMediaFormat oQuestionedFormat = *opHiResFile -> GetFormat();
	delete opHiResFile;
	return (oQuestionedFormat == *p_oWantedFormat);
}

string EMBeMediaFileManager::CopyTo(const string* p_opSourceFile, const string* p_opTargetDirectory)
{
	string oFileName = EMBeMediaUtility::GetFileName(*p_opSourceFile);
	string oTargetFile = *p_opTargetDirectory + "/" + oFileName;

	EMMediaFile* opInFile = NULL;
	if(! EMMediaFileRepository::Instance() -> FindFileName(p_opSourceFile))
	{
		if(! EMMediaFileRepository::Instance() -> AddFile(p_opSourceFile))
			EMDebugger("ERROR! Could not add source file to file repository!");
	}

	opInFile = EMMediaFileRepository::Instance() -> FindFileName(p_opSourceFile);
	if(opInFile == NULL)
		EMDebugger("ERROR! In file still NULL!?");
	opInFile -> InitCheckE();
		

	EMMediaFile* opOutFile = NULL;
	if(! EMMediaFileRepository::Instance() -> FindFileName(&oTargetFile))
	{
		if(! EMMediaFileRepository::Instance() -> AddFile(&oTargetFile))
			EMDebugger("ERROR! Could not add target file to file repository!");
	}
	opOutFile = EMMediaFileRepository::Instance() -> FindFileName(&oTargetFile);
	if(opOutFile == NULL)
		EMDebugger("ERROR! Out file still NULL!?");
	opOutFile -> InitCheckE();
	
	opInFile -> Open(EM_READ);
	opOutFile -> Open(EM_WRITE);
	
	int64 vPosition = 0;
	char vpBuffer[1024];
	int64 vCount = 0;	
	int64 vLength = opInFile -> GetLength();
	while(vPosition < vLength)
	{
		int64 vNumRead = opInFile -> Read(vpBuffer, 1024, vPosition);
		opOutFile -> Write(vpBuffer, vNumRead);
		vPosition += vNumRead;

		if((vCount % 1024 * 25) == 0)
		{
			char vpPercentage[10];
			sprintf(vpPercentage, "%d%%",  (int) (100 * (vPosition) / (float) vLength));
			string oString = string("Importing audio to working directory: ") + vpPercentage;
			EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, const_cast<char*>(oString.c_str()));
		}
	}
	
	return oTargetFile;
}

string EMBeMediaFileManager::ConvertAudio(const string* p_oSourceFileName, const EMMediaFormat* p_opTargetFormat)
{

	if(ConvertFileExistsFor(p_oSourceFileName))
		return EMBeMediaUtility::MakeConvertFileName(*p_oSourceFileName);

	EMWaveFileReader* opSourceFile = new EMWaveFileReader(*p_oSourceFileName);
	opSourceFile -> InitCheckE();
	EMMediaFormat* opSourceFormat = opSourceFile -> GetFormat();

	string oConvertedFileName = "";

	if(opSourceFormat -> m_vBytesPerSample == 1)
		if(p_opTargetFormat -> m_vBytesPerSample == 1)
		{
			oConvertedFileName = EMAudioConverter<unsigned char, unsigned char>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else if(p_opTargetFormat -> m_vBytesPerSample == 2)
		{
			oConvertedFileName = EMAudioConverter<unsigned char, signed short>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else if(p_opTargetFormat -> m_vBytesPerSample == 4)
		{
			oConvertedFileName = EMAudioConverter<unsigned char, float>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else 
			EMDebugger("ERROR! Unknown target type!");
	else if(opSourceFormat -> m_vBytesPerSample == 2)
		if(p_opTargetFormat -> m_vBytesPerSample == 1)
		{
			oConvertedFileName = EMAudioConverter<signed short, unsigned char>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else if(p_opTargetFormat -> m_vBytesPerSample == 2)
		{
			oConvertedFileName = EMAudioConverter<signed short, signed short>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else if(p_opTargetFormat -> m_vBytesPerSample == 4)
		{
			oConvertedFileName = EMAudioConverter<signed short, float>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else
			EMDebugger("ERROR! Unknown target type!");
	else if(opSourceFormat -> m_vBytesPerSample == 4)
		if(p_opTargetFormat -> m_vBytesPerSample == 1)
		{
			oConvertedFileName = EMAudioConverter<float, unsigned char>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else if(p_opTargetFormat -> m_vBytesPerSample == 2)
		{
			oConvertedFileName = EMAudioConverter<float, signed short>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else if(p_opTargetFormat -> m_vBytesPerSample == 4)
		{
			oConvertedFileName = EMAudioConverter<float, float>::ConvertAudio(p_oSourceFileName, p_opTargetFormat);
		}
		else
			EMDebugger("ERROR! Unknown target type!");
	else
		EMDebugger("ERROR! Unknown source type!");

	delete opSourceFile;

	return oConvertedFileName;
}

/*int64 EMMediaFileManager::ConvertAudioChunk(void* p_upSourceChunk, 
											int64 p_vSourceSize, 
											const EMMediaFormat* p_opSourceFormat, 

											void* p_upTargetChunk, 
											int64 p_vTargetSize, 
											const EMMediaFormat* p_opTargetFormat)
{
	return 0; //The number of converted byte(s) (<= p_vTargetSize)
} */


#endif
