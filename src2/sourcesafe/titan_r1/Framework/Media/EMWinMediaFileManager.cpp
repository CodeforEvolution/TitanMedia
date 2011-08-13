#include "EMGlobals.h"
#include "EMMediaEngine.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMediaFileManager.h"
#include "EMMediaGlobals.h"
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h"
#include "EMMediaFile.h"
#include "EMWaveFileReader.h"
#include "EMWaveFileWriter.h" 
#include "EMMediaFileRepository.h"
#include "EMMediaFormat.h"  
#include "EMAudioConverter.h"
#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h" 
#include "EMWinMediaSystemInspector.h"
#include "EMMediaEngineUIDs.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"


#include "DirectoryIDs.h"  
#include <math.h>
#include <streams.h>
#include <Mtype.h>  

#include <Vfw.h>

#include "EMAudioExtractor.h"

EMWinMediaFileManager::EMWinMediaFileManager()
	:	EMMediaFileManager(), 
		m_opNewFile(NULL),
		m_oQuickTimeVideo(NULL)
{
	//m_oQuickTimeVideo = new EMWinQuicktimeVideoSuper("TOM");
	AVIFileInit();

} 

EMWinMediaFileManager::~EMWinMediaFileManager()   
{
	AVIFileExit();

	if(m_oQuickTimeVideo != NULL)
		delete m_oQuickTimeVideo;
}

uint32 GetNumInputs(IBaseFilter* p_opFilter)
{
	int32 vRefCount = 0;

	uint32 vPinCount = 0;
	IEnumPins* opPinEnumerator = NULL;
	HRESULT vResult = p_opFilter -> EnumPins(&opPinEnumerator);
	if(SUCCEEDED(vResult))
	{
		IPin* opPin = NULL;
		while(opPinEnumerator -> Next(1, &opPin, NULL) == S_OK)
		{
			PIN_INFO sPinInfo;
			vResult = opPin -> QueryPinInfo(&sPinInfo);
			if(SUCCEEDED(vResult))
			{
				if(sPinInfo.dir == PINDIR_INPUT)
					vPinCount++;
			}
			if(sPinInfo.pFilter != NULL)
				vRefCount = sPinInfo.pFilter -> Release();
			vRefCount = opPin ->Release();
		}
		vRefCount = opPinEnumerator -> Release();
	}
	return vPinCount;
}

uint32 GetNumOutputs(IBaseFilter* p_opFilter)
{
	int32 vRefCount = 0;

	uint32 vPinCount = 0;
	IEnumPins* opPinEnumerator = NULL;
	HRESULT vResult = p_opFilter -> EnumPins(&opPinEnumerator);
	if(SUCCEEDED(vResult))
	{
		IPin* opPin = NULL;
		while(opPinEnumerator -> Next(1, &opPin, NULL) == S_OK)
		{
			PIN_INFO sPinInfo;
			vResult = opPin -> QueryPinInfo(&sPinInfo);
			if(SUCCEEDED(vResult))
			{
				if(sPinInfo.dir == PINDIR_OUTPUT)
					vPinCount++;
			}
			if(sPinInfo.pFilter != NULL)
				vRefCount = sPinInfo.pFilter -> Release();
			vRefCount = opPin -> Release();
		}
		vRefCount = opPinEnumerator -> Release();
	}
	return vPinCount;
}

IPin* GetFirstInput(IBaseFilter* p_opFilter)
{
	IPin* opFirstInput = NULL;
	uint32 vPinCount = 0;
	IEnumPins* opPinEnumerator = NULL;
	HRESULT vResult = p_opFilter -> EnumPins(&opPinEnumerator);
	if(SUCCEEDED(vResult))
	{
		IPin* opPin = NULL;
		while(opPinEnumerator -> Next(1, &opPin, NULL) == S_OK && opFirstInput == NULL)
		{
			PIN_INFO sPinInfo;
			vResult = opPin -> QueryPinInfo(&sPinInfo);
			if(SUCCEEDED(vResult))
			{
				if(sPinInfo.dir == PINDIR_INPUT)
					opFirstInput = opPin;
			}
			if(sPinInfo.pFilter != NULL)
				sPinInfo.pFilter -> Release();
			if(opFirstInput == NULL)
				opPin -> Release();
		}
		opPinEnumerator -> Release();
	}
	return opFirstInput;
}

IPin* GetFirstOutput(IBaseFilter* p_opFilter)
{
	IPin* opFirstOutput = NULL;
	uint32 vPinCount = 0;
	IEnumPins* opPinEnumerator = NULL;
	HRESULT vResult = p_opFilter -> EnumPins(&opPinEnumerator);
	if(SUCCEEDED(vResult))
	{
		IPin* opPin = NULL;
		while(opPinEnumerator -> Next(1, &opPin, NULL) == S_OK && opFirstOutput == NULL)
		{
			PIN_INFO sPinInfo;
			vResult = opPin -> QueryPinInfo(&sPinInfo);
			if(SUCCEEDED(vResult))
			{
				if(sPinInfo.dir == PINDIR_OUTPUT)
					opFirstOutput = opPin;
			}
			if(sPinInfo.pFilter != NULL)
				sPinInfo.pFilter -> Release();
			if(opFirstOutput == NULL)
				opPin -> Release();
		}
		opPinEnumerator -> Release();
	}
	return opFirstOutput;
}

bool MediaTypesMatch(IPin* p_opAConnectedPin, EMMediaType p_eType)
{
	bool vResult = false;
	AM_MEDIA_TYPE sType;
//	WAVEFORMATEX* opWave;

	if(SUCCEEDED(p_opAConnectedPin -> ConnectionMediaType(&sType)))
	{
		if(sType.majortype == MEDIATYPE_Audio && (p_eType & EM_TYPE_ANY_AUDIO) > 0)
		{			
//			opWave = reinterpret_cast<WAVEFORMATEX*>(sType.pbFormat);
//			vResult = opWave -> nChannels == 2;			//Make sure it is not mono!!! -- Hack by Kaneda
			vResult = true;
		}
		else if(sType.majortype == MEDIATYPE_Video && (p_eType & EM_TYPE_ANY_VIDEO) > 0)
			vResult = true;
		FreeMediaType(sType);
	}
	return vResult;
}

int32 EMWinMediaFileManager::DiscoverFileType(IGraphBuilder* p_opGraph)
{
	ULONG vFetched = 0;
	IBaseFilter* opRenderer = NULL;
	IEnumFilters* opEnumFilters = NULL;
	IPin* opInPin = NULL;	// renderer input
//	PIN_INFO sPinInfo;
	PIN_DIRECTION sPinDir;
//	int vNumOutputPins = 0;
	IEnumPins* opEnumPins = NULL;
	int32 vFormatType = EM_TYPE_UNKNOWN;

	p_opGraph -> EnumFilters(&opEnumFilters);
	opEnumFilters -> Reset();

	//Loop through all the filters in the graph
	while(opEnumFilters -> Next(1, &opRenderer, &vFetched) == S_OK)
	{
		//;	// get next filter
		opRenderer -> EnumPins(&opEnumPins);
		opEnumPins -> Reset();
//		vNumOutputPins = 0;

		//Loop through all the pins in one filter
		IPin* opAnOutputPin = NULL;
		while(opEnumPins -> Next(1, &opInPin, &vFetched) == S_OK)
		{
			opInPin -> QueryDirection(&sPinDir); //         QueryPinInfo(&sPinInfo);
			//sPinInfo.pFilter -> Release();
			if(sPinDir == PINDIR_OUTPUT)	//sPinInfo.dir == PINDIR_OUTPUT)
			{
				//Is this a video or an audio renderer
				AM_MEDIA_TYPE sAMMediaType;
				HRESULT hr = opInPin -> ConnectionMediaType(&sAMMediaType);

				if(sAMMediaType.majortype == MEDIATYPE_Video)
					vFormatType = vFormatType | EM_TYPE_UNKNOWN_VIDEO; //Don't know if it's encoded or not yet
				else if(sAMMediaType.majortype == MEDIATYPE_Audio)
					vFormatType = vFormatType | EM_TYPE_UNKNOWN_AUDIO; //Don't know if it's encoded or not yet
				
				if(sAMMediaType.formattype == FORMAT_WaveFormatEx)
				{
					if((vFormatType & EM_TYPE_ANY_VIDEO) > 0)
						vFormatType = vFormatType | EM_TYPE_ENCODED_AUDIO;
					else
						vFormatType = vFormatType | EM_TYPE_RAW_AUDIO;
				}
				else if(sAMMediaType.formattype == FORMAT_MPEGVideo )
					vFormatType = vFormatType | EM_TYPE_ENCODED_VIDEO;
				else if(sAMMediaType.formattype == FORMAT_MPEG2Video )
					vFormatType = vFormatType | EM_TYPE_ENCODED_VIDEO;
				else if(sAMMediaType.formattype == FORMAT_VideoInfo)
					vFormatType = vFormatType | EM_TYPE_ENCODED_VIDEO;
				else if(sAMMediaType.formattype == FORMAT_VideoInfo2 )
					vFormatType = vFormatType | EM_TYPE_ENCODED_VIDEO;

				FreeMediaType(sAMMediaType);
			}

			opInPin -> Release();
			opInPin = NULL;
		}

		opEnumPins -> Release();
		opEnumPins = NULL;

		opRenderer -> Release();
		opRenderer = NULL;
	}

	opEnumFilters -> Release();	
	opEnumFilters = NULL;

	//If it's video as well as audio...

	//Fast hack to test Quicktime and AVI

	//vFormatType = vFormatType | EM_TYPE_ENCODED_VIDEO;

	if((vFormatType & EM_TYPE_ANY_AUDIO) == EM_TYPE_UNKNOWN_AUDIO)
		vFormatType = vFormatType | EM_TYPE_ENCODED_AUDIO;

	return vFormatType;
}

int32 EMWinMediaFileManager::GetFileTypeForE(const string* p_opFileName)
{
	IFilterGraph2* g = NULL;
	IGraphBuilder* opGraph = NULL;
	int32 vFormatType = EM_TYPE_UNKNOWN;

	if(p_opFileName == NULL)
		EMDebugger("ERROR! Filename null!");

	string oFileName(*p_opFileName);

	char* opExt = strrchr(oFileName.c_str(), '.');

	if(_stricmp(opExt, ".bmp") == 0)
	{
		vFormatType = EM_TYPE_BMP;
	}
	else if(_stricmp(opExt, ".gif") == 0)
	{
		vFormatType = EM_TYPE_GIF;
	}
	else if(_stricmp(opExt, ".jpg") == 0)
	{
		vFormatType = EM_TYPE_JPG;
	}
	else if(_stricmp(opExt, ".tga") == 0)
	{
		vFormatType = EM_TYPE_TGA;
	}
	else if(_stricmp(opExt, ".avi") == 0)
	{
		PAVIFILE sPavi;
		PAVISTREAM sPstm;
		AVISTREAMINFOW sPsi;
		long vStreamNumber = 0;
		if (AVIFileOpen(&sPavi, oFileName.c_str(), OF_READ | OF_SHARE_DENY_NONE,NULL)==0)
		{			
			while(AVIFileGetStream(sPavi, &sPstm, 0, vStreamNumber) == 0)
			{
				sPstm -> Info(&sPsi, sizeof(AVISTREAMINFOW));
				if(sPsi.fccType == streamtypeAUDIO)
				{
					vFormatType = vFormatType | EM_TYPE_ENCODED_AUDIO;
				}
				if(sPsi.fccType == streamtypeVIDEO)
				{
					vFormatType = vFormatType | EM_TYPE_ENCODED_VIDEO;
				}
				vStreamNumber++;
			}
			AVIFileRelease(sPavi);
			return vFormatType;

		}
		
	}
	else if(_stricmp(opExt, ".mov") == 0)
	{
		string oQuickTimeFileName(*p_opFileName);
		if(m_oQuickTimeVideo->CheckIfQuickTimeMovie(&oQuickTimeFileName, 0))
		{
			vFormatType = vFormatType | EM_TYPE_ENCODED_VIDEO;
		}
		if(m_oQuickTimeVideo->CheckIfQuickTimeMovie(&oQuickTimeFileName, 1))
		{
			vFormatType = vFormatType | EM_TYPE_ENCODED_AUDIO;
		}
		return vFormatType;
	}
	else if(_stricmp(opExt, ".mpg") == 0)
	{
		vFormatType = 0; //vFormatType | EM_TYPE_ENCODED_VIDEO;
		//vFormatType = vFormatType | EM_TYPE_ENCODED_AUDIO;
		MessageBox(NULL, "Sorry, as for now there is no MPEG Video support!", "", MB_OK);
		return vFormatType;
	}
	else
	{
		HRESULT vResult = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**) &opGraph);

		string oFileName(*p_opFileName);
		LPWSTR vpName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> StringToUnicode(oFileName.c_str());

		vResult = opGraph -> RenderFile(vpName, NULL);

		if(FAILED(vResult))
		{
//			eo << "ERROR! Failed to render graph for file: " << oFileName.c_str() << ef;
//			eo << "       The error code from DirectX is " << (int) vResult << ef;
		}
		else
		{
			vFormatType = DiscoverFileType(opGraph);
		}


		IEnumFilters* opFilterEnumerator = NULL;
		opGraph ->EnumFilters(&opFilterEnumerator);

		IBaseFilter* opCurrentFilter = NULL;
		while(opFilterEnumerator ->Next(1, &opCurrentFilter, NULL) == S_OK)
		{
			if(FAILED(opGraph -> RemoveFilter(opCurrentFilter)))
				MessageBox(NULL, "ERROR! Failed to remove filter from graph!", "", MB_OK);
			opCurrentFilter -> Release();
		} 
		opFilterEnumerator ->Release();
		opFilterEnumerator = NULL;

		opGraph -> Release();
		opGraph = NULL;
	}

	//Megahack för att ta bort video från asf......
	const char* vHack = oFileName.c_str();

	int n = strlen(vHack);

	vHack += n - 4;

	if(_stricmp(vHack,".asf") == 0)
	{
		vFormatType = vFormatType ^ (vFormatType & EM_TYPE_ANY_VIDEO);
	}

	return vFormatType; 
}

bool EMWinMediaFileManager::FindOriginalFormat(string p_oFileName, EMMediaFormat* p_opFormat)
{
//	_ASSERT(p_opFormat != NULL);

//	IFilterGraph2* opIFilterGraph = NULL;
	IGraphBuilder* opIGraphBuilder = NULL;

	//CHANGED FROM CLSCTX_INPROC_SERVER TO CLSCTX_INPROC
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**) &opIGraphBuilder);
	LPWSTR vpFileNameUnicode = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> StringToUnicode(p_oFileName.c_str());
    HRESULT vResult = opIGraphBuilder -> RenderFile(vpFileNameUnicode, NULL);

	delete [] vpFileNameUnicode;

//	EMMediaFormat oFmt(EM_TYPE_UNKNOWN);

	if(SUCCEEDED(vResult))
	{
		IEnumFilters* opFilterEnumerator = NULL;

		vResult = opIGraphBuilder -> EnumFilters(&opFilterEnumerator);
		if(SUCCEEDED(vResult))
		{
			IBaseFilter* opFilter = NULL;
			while(opFilterEnumerator -> Next(1, &opFilter, NULL) == S_OK)
			{
				if(GetNumInputs(opFilter) == 0 && GetNumOutputs(opFilter) > 0)
				{
					IEnumPins* opPinEnumerator = NULL;
					vResult = opFilter ->EnumPins(&opPinEnumerator);
					if(SUCCEEDED(vResult))
					{
						IPin* opPin = NULL;
						while(opPinEnumerator->Next(1, &opPin, NULL) == S_OK)
						{
							AM_MEDIA_TYPE uMediaType;
							vResult = opPin -> ConnectionMediaType(&uMediaType);
							if(SUCCEEDED(vResult))
							{
								if(uMediaType.majortype == MEDIATYPE_Audio)
								{
									MessageBox(NULL, "Pin is audio type!", "", MB_OK);
								}
								else if(uMediaType.majortype == MEDIATYPE_Video)
								{
									MessageBox(NULL, "Pin is video type!", "", MB_OK);
								}
								else
									MessageBox(NULL, "Pin is other type!", "", MB_OK);

								FreeMediaType(uMediaType);
							}
							opPin -> Release();
						}
						opPinEnumerator -> Release();
					}
				}
				opFilter -> Release();
			}
			opFilterEnumerator -> Release();
		}
	}
	return true;
}

bool EMWinMediaFileManager::CompareNumFramesInFile(const string* p_opFileName, int64 p_vNumFramesToCompareWith)
{
	EMWaveFileReader* opPeaksFile = EM_new EMWaveFileReader(*p_opFileName);
	opPeaksFile -> InitCheckE();
	bool vResult = false;
	if(opPeaksFile -> NumberOfFramesInFile() == p_vNumFramesToCompareWith)
		vResult = true;
//	else
//		eo << "ERROR! File exists, but has " << (uint32) opPeaksFile -> NumberOfFramesInFile() << " frame(s) instead of " << (uint32) p_vNumFramesToCompareWith << ef;
	delete opPeaksFile;
	return vResult;
}

bool EMWinMediaFileManager::FileExists(const string* p_opFileName)
{
	FILE* sFile = fopen(p_opFileName -> c_str(), "r");
	if(sFile == NULL)
		return false;
	fclose(sFile);
	return true;
}

bool EMWinMediaFileManager::ConvertFileExistsFor(const string* p_opSourceFileName)
{
	string oConverttFileName(EMMediaUtility::Instance() -> MakeConvertFileName((*p_opSourceFileName).c_str()));

	if(! FileExists(&oConverttFileName))
		return false;
		
	//Find out what the expected numframes is...
	EMWaveFileReader* opFile = EM_new EMWaveFileReader(*p_opSourceFileName);
	opFile -> InitCheckE();
	int64 vOriginalNumFrames = opFile -> NumberOfFramesInFile();
	
	EMMediaFormat oSourceFormat = *(opFile -> GetFormat());
	EMMediaFormat oTargetFormat(EM_TYPE_RAW_AUDIO);
	delete opFile;

	int64 vExpectedNumPeaksFrames = static_cast<int64>(vOriginalNumFrames / (oSourceFormat.m_vFrameRate / oTargetFormat.m_vFrameRate));

	//Compare the expected number of frames now...
	return CompareNumFramesInFile(&oConverttFileName, vExpectedNumPeaksFrames);
}

bool EMWinMediaFileManager::PeaksFileExistsFor(const string* p_opFileName)
{
	string oPeaksFileName(EMMediaUtility::Instance() -> MakePeaksFileName((*p_opFileName).c_str()));

	if(! FileExists(&oPeaksFileName))
		return false;
		
	//Find out what the expected numframes is...
	EMWaveFileReader* opFile = EM_new EMWaveFileReader(*p_opFileName);
	opFile -> InitCheckE();
	int64 vOriginalNumFrames = opFile -> NumberOfFramesInFile();
	delete opFile;
	int64 vExpectedNumPeaksFrames = static_cast<int64>(floor(vOriginalNumFrames / EM_LOW_RES_AUDIO_SKIP_RATE));

	//Compare the expected number of frames now...
	return CompareNumFramesInFile(&oPeaksFileName, vExpectedNumPeaksFrames);
}

bool EMWinMediaFileManager::ExtractFileExistsFor(const string* p_opFileName)
{
	string oExtractFileName(EMMediaUtility::Instance() -> MakeExtractFileName((*p_opFileName).c_str()));

	if(! FileExists(&oExtractFileName))
		return false;

	//Find out what the expected numframes is...

/*	entry_ref sRef;

	if(p_opFileName == NULL)
		EMDebugger("ERROR! File name is NULL!");
	
	BEntry oEntry(p_opFileName -> c_str(), false);
	oEntry.GetRef(&sRef);
	BMediaFile* opFile = EM_new BMediaFile(&sRef);
	
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
	*/
	//Compare the expected number of frames now...
	return true; //CompareNumFramesInFile(&oExtractFileName, vOriginalNumFrames);
}

string EMWinMediaFileManager::CreatePeaksFileE(const string* p_opFileName)
{
//	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_AUDIO_EXTRACTION_CONVERSION_DIALOG, "Analyzing audio...", const_cast<char*>(string(string("") + string(p_opFileName -> c_str()) + string("...")).c_str()), NULL);
	string oPeaksFileName(EMMediaUtility::Instance() -> MakePeaksFileName((*p_opFileName).c_str()));
	if(PeaksFileExistsFor(p_opFileName))
	{
		uint16 vVal = 1000;
//		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_SET_VALUE_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG, &vVal, NULL, NULL);
		::Sleep(50);
//		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_CLOSE_AUDIO_EXTRACTION_CONVERSION_DIALOG);
		return oPeaksFileName;
	}

	register bool vShouldLeaveLoop = false; 
	EMWaveFileReader* opHiResFile = EM_new EMWaveFileReader(*p_opFileName);
	EMWaveFileWriter* opLoResFile = EM_new EMWaveFileWriter(oPeaksFileName);

	opHiResFile -> InitCheckE();
	opLoResFile -> InitCheckE();

	int64 vTotalLength = opHiResFile -> GetSize();
	int64 vFinishedLength = static_cast<int64>(floor(floor(vTotalLength / (EM_LOW_RES_AUDIO_SKIP_RATE * 4)) * 2));
	
	int vNumHiResSamplesToRead = 1000;
	typedef unsigned char LORES_SAMPLE_TYPE;
	int32 vHiResSampleSize = sizeof(signed short);
	int32 vLoResSampleSize = sizeof(LORES_SAMPLE_TYPE);

 	int vNumLoResSamplesToRead = vNumHiResSamplesToRead / EM_LOW_RES_AUDIO_SKIP_RATE;
	signed short* vpHiResArray = EM_new signed short[vNumHiResSamplesToRead];
	LORES_SAMPLE_TYPE* vpLoResArray = EM_new LORES_SAMPLE_TYPE[vNumLoResSamplesToRead];
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
				//Make sure we add the zero-level value to the sample value, since we're converting from SIGNED (shorts) to UNSIGNED (chars).
				vpLoResArray[vLoResPos + vChan] = static_cast<LORES_SAMPLE_TYPE>(((vpHiResArray[(vLoResPos * EM_LOW_RES_AUDIO_SKIP_RATE) + vChan] + 1) / 32768.0) * 256.0) - 128; 

		int64 vSize = vLoResPos;

		//Make sure we don't write too much!
		if(vWriteCount + vSize > vFinishedLength)
			vSize -= (vWriteCount + vSize) - vFinishedLength;

		opLoResFile -> WriteData(vpLoResArray, vSize);
		vWriteCount += vSize;
		
		if((vCount % 50) == 0 && vTotalLength > 0)
		{
			float vFloatVal = (static_cast<float>(vPosition) / static_cast<float>(vTotalLength)) * 1000.0;
			uint16 vVal = static_cast<uint16>(vFloatVal);
//			EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_SET_VALUE_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG, &vVal, NULL, NULL);
		
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

//	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_CLOSE_AUDIO_EXTRACTION_CONVERSION_DIALOG);

	return string(oPeaksFileName);
}

bool EMWinMediaFileManager::CheckFileForVideoFormat(const string* p_opFileName)
{
/*	entry_ref m_sRef;
	BEntry oEntry(p_opFileName -> c_str(), false);
	oEntry.GetRef(&m_sRef);

	BMediaFile* opFile = EM_new BMediaFile(&m_sRef, B_MEDIA_FILE_UNBUFFERED);
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
	delete opFile; */
	return true;
}

IPin* EMWinMediaFileManager::GetPin(IBaseFilter* p_opFilterToInvestigate, PIN_DIRECTION p_sPinDirectionWanted)
{
    IEnumPins* opEnumerator = NULL;
    IPin* opPin = NULL;
	bool vFound = false;
    
	if(p_opFilterToInvestigate == NULL)
		EMDebugger("ERROR! Can't investigate NULL filter for pins, in EMWinMediaFileManager::GetPin");

	try
	{
		p_opFilterToInvestigate -> EnumPins(&opEnumerator);
		while(opEnumerator -> Next(1, &opPin, 0) == S_OK)
		{
			PIN_DIRECTION sPinDirectionThis;
			opPin -> QueryDirection(&sPinDirectionThis);
			if(sPinDirectionThis == p_sPinDirectionWanted)
			{
				vFound = true;
				break;
			}
			opPin -> Release();
		}
		opEnumerator -> Release();
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception while looking for pin!");
	}

	//The following code is equal to if(vFound) return opPin; else return NULL;
    return (vFound ? opPin : NULL);  
}

IPin* EMWinMediaFileManager::FindAndDiscardRenderer(EMMediaType p_eType)
{
	int32 vRefCount = 0;

	IEnumFilters* opFilterEnumerator = NULL;
	IBaseFilter* opCurrentFilter = NULL;

	IBaseFilter* opRendererFilter = NULL;
	IPin* opRendererFilterInput = NULL;

	HRESULT vResult = S_OK;

	if(FAILED(m_opGraph -> EnumFilters(&opFilterEnumerator)))
		EMDebugger("ERROR! Couldn't get filter enumerator from the graph!");

	opFilterEnumerator -> Reset();
	while(opFilterEnumerator -> Next(1, &opCurrentFilter, NULL) == S_OK && opRendererFilter == NULL && opRendererFilterInput == NULL)
	{
		uint32 vNumberOfInputPins = GetNumInputs(opCurrentFilter);
		uint32 vNumberOfOutputPins = GetNumOutputs(opCurrentFilter);

		//The definition of a renderer...
		if(vNumberOfInputPins >= 1 && vNumberOfOutputPins <= 0)
		{
			IPin* opInputPin = GetFirstInput(opCurrentFilter);
//			_ASSERT(opInputPin != NULL);

			if(MediaTypesMatch(opInputPin, p_eType))
			{
				opRendererFilterInput = opInputPin;
				opRendererFilter = opCurrentFilter;
			}

			if(opRendererFilterInput == NULL)
				vRefCount = opInputPin -> Release();
		}
		if(opRendererFilter == NULL) 
			vRefCount = opCurrentFilter -> Release();
	}
	
	IPin* opPrevFilterOutputPin = NULL;
	vRefCount = opFilterEnumerator -> Release();

	if(opRendererFilterInput != NULL && opRendererFilter != NULL)
	{
		opRendererFilterInput -> ConnectedTo(&opPrevFilterOutputPin);
		opRendererFilterInput -> Disconnect();
		opPrevFilterOutputPin -> Disconnect();
		m_opGraph -> RemoveFilter(opRendererFilter);
		vRefCount = opRendererFilterInput -> Release();
		vRefCount = opRendererFilter -> Release();
	}

	return opPrevFilterOutputPin;
}

string EMWinMediaFileManager::ExtractAudioE(const string* p_opFileName)
{
	EMWaveFileReader* opReader = EM_new EMWaveFileReader(*p_opFileName);
	if(opReader -> InitCheckE())
	{
		EMMediaFormat* opFormat = opReader -> GetFormat();

		bool vCheck = opFormat -> m_vFrameRate == 44100;
		vCheck &= (opFormat -> m_vNumChannels == 2);
		vCheck &= (opFormat -> m_vBytesPerSample == 2);


		if(vCheck)
		{

			int vProjectID = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_PROJECT_ID)));
			int64 vDummy = DIRECTORY_ID_AUDIO_DATA_USED;
			string oPath = *static_cast<string*>(EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDummy));

			string oOldPath = EMMediaUtility::Instance() -> GetStringBeforeLast(p_opFileName -> c_str(), '\\');

			//Now, we need to check if the file should be copied to the audio directory, or if that has been done already, through
			//a convert/extract process or such...
			if(stricmp(oOldPath.c_str(), oPath.c_str()) != 0)
			{
				//Copy?? Needs progress bar etc... no time...

//				string oPath(EMMediaUtility::Instance() -> GetPathName(oCheckFile.c_str()));
//				EMMediaFileManager::Instance() -> CopyTo(p_opFileName, &oPath);

//				return oPath + EMMediaUtility::Instance() -> GetStringAfterLast(p_opFileName -> c_str(),  '\\');;
			}
			else
			{
				delete opReader;
				return *p_opFileName;
			}
		}
	}

	delete opReader;


	if(ExtractFileExistsFor(p_opFileName))
	{
		string oExtractFileName(EMMediaUtility::Instance() -> MakeExtractFileName((*p_opFileName).c_str()));
		return oExtractFileName;
	}

	string oExtractedAudioFileName = EMMediaUtility::Instance() -> MakeExtractFileName((*p_opFileName).c_str());

	if(EMAudioExtractor::Instance() -> ExtractAudio(p_opFileName -> c_str(), oExtractedAudioFileName.c_str()) == false)
	{
		oExtractedAudioFileName = "Noname";
	}

	return oExtractedAudioFileName;
}

bool EMWinMediaFileManager::FormatsMatch(const string* p_opQuestionedFileName, const EMMediaFormat* p_oWantedFormat)
{
	EMWaveFileReader* opHiResFile = EM_new EMWaveFileReader(*p_opQuestionedFileName);
	opHiResFile -> InitCheckE();

	EMMediaFormat oQuestionedFormat = *opHiResFile -> GetFormat();
	oQuestionedFormat.Display();

	delete opHiResFile;
	return (oQuestionedFormat == *p_oWantedFormat);
}

string EMWinMediaFileManager::CopyTo(const string* p_opSourceFile, const string* p_opTargetDirectory)
{
	string oFileName(EMMediaUtility::Instance() -> GetFileName((*p_opSourceFile).c_str()));
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
	unsigned char vpBuffer[4096];
	int64 vCount = 0;	
	int64 vLength = opInFile -> GetLength();
	while(vPosition < vLength)
	{
		int64 vNumRead = opInFile -> Read(vpBuffer, 4096, vPosition);
		opOutFile -> Write(vpBuffer, vNumRead);
		vPosition += vNumRead;

/*		if((vCount % (4096 * 25)) == 0)
		{
			char vpPercentage[10];
			sprintf(vpPercentage, "%d%%",  (int) (100 * (vPosition) / (float) vLength));
			string oString = string("Importing audio to working directory: ") + vpPercentage;
			eo << oString.c_str() << ef;
//			EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, const_cast<char*>(oString.c_str()));
		} */
	}
	
	return oTargetFile;
}

string EMWinMediaFileManager::ConvertAudio(const string* p_oSourceFileName, const EMMediaFormat* p_opTargetFormat)
{  
	if(ConvertFileExistsFor(p_oSourceFileName))
		return string(EMMediaUtility::Instance() -> MakeConvertFileName((*p_oSourceFileName).c_str()));

	EMWaveFileReader* opSourceFile = EM_new EMWaveFileReader(*p_oSourceFileName);
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

#endif
