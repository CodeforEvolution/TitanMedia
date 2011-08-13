#include "EMWinQuickTimeWriter.h"

#include "QTML.h"
#include "Movies.h"
#include "TextUtils.h"
#include "Quickdraw.h"
#include "QuickTimeComponents.h"
#include <FixMath.h>
#include "ImageCodec.h"
#include "EMWinMediaSingletonMuxer.h"

#include <windows.h>
#include "stdio.h"
#include "EMMediaFormat.h"
#include "EMGlobals.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaEngine.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"

//HDC m_hdcDest;
//HBITMAP m_dibHBitmap;
//void* DoCreateDIBSection(WORD	depth, LONG	width, LONG height);
//static void QTVideo_AddVideoSamplesToMedia (Media theMedia, const Rect *m_sTrackFrame, GWorldPtr theGWorld, OSType p_cType);


//#define		m_vVideoTimeScale 	600
#define		kPixelDepth 		24	/* use 8-bit depth */
#define		kNoOffset 			0
#define		kMgrChoose			0
#define		kSyncSample 		0
#define		kAddOneVideoSample	1
//#define		m_vSampleDuration 	25	/* frame duration = 1/24 sec */
#define		kTrackStart			0
#define		kMediaStart			0


EMWinQuickTimeWriter* EMWinQuickTimeWriter::m_opInstance = NULL;

EMWinQuickTimeWriter::EMWinQuickTimeWriter() :
	m_opMediaFormatForVideo(NULL),
	m_opMediaFormatForAudio(NULL),
	m_oGWorld(NULL),
	m_oExportMovie(NULL),
	m_vFileRefNum(0),
	m_oMovieExportTrack(NULL),
	m_oMovieExportMedia(NULL),
	m_oSoundExportTrack(NULL),
	m_oSoundExportMedia(NULL),
	m_mySampleDesc(NULL),
	m_pCompressedData(NULL)

{
	if( InitializeQTML(0) != noErr) //Initialize QTML
	{
	}

	if( EnterMovies() != noErr) //Initialize Quicktime
	{
	}
	m_VideoOptionSettingsHandle = NewHandle(0);
	m_VideoOptionSettingsRect.left =400;
	m_VideoOptionSettingsRect.top = 300;
	m_VideoOptionSettingsRect.right = 410;
	m_VideoOptionSettingsRect.bottom = 310;

}

EMWinQuickTimeWriter::~EMWinQuickTimeWriter()
{
	DisposeHandle((Handle)m_VideoOptionSettingsHandle);
	//Exit Close Movie and Terminate QTML
	ExitMovies();
	TerminateQTML();
}

EMWinQuickTimeWriter* EMWinQuickTimeWriter::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;

	if(m_opInstance == NULL)
		m_opInstance = EM_new EMWinQuickTimeWriter();

	return m_opInstance;
}

void EMWinQuickTimeWriter::SetRenderName(char* p_vpName)
{
	m_oOutputFileName = new char[255];
	int index = 0;

	while(p_vpName[index] != '\0')
	{
		m_oOutputFileName[index] = p_vpName[index];
		index++;
	}
	m_oOutputFileName[index] = 0;
}

void EMWinQuickTimeWriter::SetShuffleFormatForVideo(EMMediaFormat* p_opMediaFormat)
{
	m_opMediaFormatForVideo = p_opMediaFormat;

	EMMediaFormat oMediaFormatForVideoFlippedHeight = *m_opMediaFormatForVideo;
	
	oMediaFormatForVideoFlippedHeight.m_vHeight = - oMediaFormatForVideoFlippedHeight.m_vHeight;

	m_oScale.InitScaleFunctions(m_opMediaFormatForVideo, &oMediaFormatForVideoFlippedHeight);
	

	m_vSampleDuration = m_opMediaFormatForVideo -> m_vFrameRate;
	m_vVideoTimeScale = 600;
	int vConvertedVideoTimeScale = 60000;
	int vConvertedFrameRate = static_cast<int>(m_vSampleDuration*100.00);

	if(vConvertedVideoTimeScale % vConvertedFrameRate != 0)
	{
		m_vVideoTimeScale = vConvertedFrameRate;
		m_vSampleDuration = 100;
	} else
	{
		m_vSampleDuration = vConvertedVideoTimeScale / vConvertedFrameRate;
	}

}


bool EMWinQuickTimeWriter::GetEncoderPropertyPages(int32 p_vEncoderID, bool p_vShouldShow)
{
	OSErr errCode;
	bool hasOptionsDialog;
	Component c = 0;
	ComponentInstance	myExporter = NULL;
	ComponentDescription cd;
	cd.componentType = 0; //1768776559 (this number will look for all video encoders)
	cd.componentSubType = (OSType)p_vEncoderID;//currentCodec.cType;//0;
	cd.componentManufacturer = 0;
	cd.componentFlags = 0; //canMovieExportFiles;
	cd.componentFlagsMask = 0;//graphicsExporterIsBaseExporter;

	myExporter = OpenComponent(FindNextComponent(c, &cd));
	GetComponentInfo ((Component)myExporter, &cd, NULL, NULL, NULL );		

	if(cd.componentType == 1768776559)
	{
		if( errCode = ImageCodecGetSettings(myExporter, m_VideoOptionSettingsHandle) != 0)
			return false;

		if(!p_vShouldShow)
			return true;

		if( errCode = ImageCodecRequestSettings (myExporter, m_VideoOptionSettingsHandle, &m_VideoOptionSettingsRect, NULL) == 0)
		{
			errCode = ImageCodecSetSettings(myExporter, m_VideoOptionSettingsHandle);
		}
		return true;
	}

	SoundComponentData sndCompDataIn;
	sndCompDataIn.flags = 0;
	sndCompDataIn.format = kSoundNotCompressed;
	sndCompDataIn.numChannels = 2;
	sndCompDataIn.sampleSize = 16;
	sndCompDataIn.sampleRate = rate44khz;
	sndCompDataIn.sampleCount = 0;
	sndCompDataIn.buffer = 0;
	sndCompDataIn.reserved = 0;

	SoundComponentData sndCompDataOut;
	sndCompDataOut.flags = 0;
	sndCompDataOut.format = (OSType)p_vEncoderID;
	sndCompDataOut.sampleRate = rate44khz;
	sndCompDataOut.sampleCount = 0;
	sndCompDataOut.buffer = 0;
	sndCompDataOut.reserved = 0;
		
	c = FindNextComponent(c, &cd);

	sndCompDataOut.numChannels = 1;
	sndCompDataOut.sampleSize = 8;
	if(errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC) != 0)
	{
		sndCompDataOut.numChannels = 1;
		sndCompDataOut.sampleSize = 16;
		if(errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC) != 0)
		{
			sndCompDataOut.numChannels = 2;
			sndCompDataOut.sampleSize = 8;
			if(errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC) != 0)
			{
				sndCompDataOut.numChannels = 2;
				sndCompDataOut.sampleSize = 16;
				if(errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC) != 0)
				{
					return false;
				}
			}
		}
	}

	errCode = SoundConverterGetInfo(m_oSC, siOptionsDialog, &hasOptionsDialog);

	if ((errCode == noErr) && hasOptionsDialog)
	{
		if(p_vShouldShow)
			errCode = SoundConverterSetInfo(m_oSC, siOptionsDialog, NULL);
		return true;
	}

	return false;
}

void EMWinQuickTimeWriter::SetShuffleFormatForAudio(EMMediaFormat* p_opMediaFormat)
{
	m_opMediaFormatForAudio = p_opMediaFormat;
}

//void EMWinQuickTimeWriter::Set

bool EMWinQuickTimeWriter::InitCheckE(EMMediaType p_vMediaType)
{
	//Create a movie file with no tracks
	OSErr errCode;
	
	if(m_oExportMovie == NULL)
	{
		c2pstr (m_oOutputFileName);
		errCode = FSMakeFSSpec(0, 0, (unsigned char*)m_oOutputFileName, &m_fileExportSpec);

		CreateMovieFile(&m_fileExportSpec, FOUR_CHAR_CODE('TVOD'), smCurrentScript, createMovieFileDeleteCurFile |createMovieFileDontCreateResFile, &m_vFileRefNum, &m_oExportMovie);
		if(m_oExportMovie == NULL)
		{
			EMWinMediaSingletonMuxer::Instance() -> SetErrorHasOccured(true);
			MessageBox(NULL, "Can't render file. Maybe it is used by another application or writeprotected?", "Titan Message", NULL);
			return false;
		}
	}

	if((p_vMediaType & EM_TYPE_ANY_VIDEO) >0)
	{
		//Add a track to the movie file
		m_sTrackFrame.right = m_opMediaFormatForVideo -> m_vWidth;
		m_sTrackFrame.bottom = m_opMediaFormatForVideo -> m_vHeight;
		m_sTrackFrame.left = 0;
		m_sTrackFrame.top = 0;

		//Add a Video track to the Movie
		m_oMovieExportTrack = NewMovieTrack (m_oExportMovie, 
								FixRatio(m_sTrackFrame.right,1),
								FixRatio(m_sTrackFrame.bottom,1), 
								kNoVolume);

		//Add a mediafile to the track
		m_oMovieExportMedia = NewTrackMedia (m_oMovieExportTrack, VideoMediaType,
								m_vVideoTimeScale, /* Video Time Scale */
								NULL, 0);

		//Create a reference graphicWorld
		errCode = QTNewGWorld(&m_oGWorld, k24BGRPixelFormat, &m_sTrackFrame,NULL,NULL,NULL);	

		//Begin edditing video media
		errCode = BeginMediaEdits (m_oMovieExportMedia);
		
		//Get the current quality
		int vQualityInt = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_QUALITY)));
		float vQualityFloat = static_cast<float>(vQualityInt/100.0);

		m_vQuality = static_cast<unsigned long>(vQualityFloat * 1024.0);

		errCode = GetMaxCompressionSize(m_oGWorld -> portPixMap,
								&m_sTrackFrame, 
								kMgrChoose,
								m_vQuality, //codecNormalQuality, 
								m_vCurrentVideoCodecToUse,//kAnimationCodecType, 
								(CompressorComponent) anyCodec,
								&m_maxCompressedSize);

		m_pCompressedData = NewHandle(m_maxCompressedSize);

		MoveHHi( m_pCompressedData );
		HLock( m_pCompressedData );
		m_pCompressedDataPtr = StripAddress( *m_pCompressedData );

		m_pImageDesc = (ImageDescriptionHandle)NewHandle(4);

	}
	else if((p_vMediaType & EM_TYPE_ANY_AUDIO) >0)
	{
		//Add a Sound track to the Movie
		m_oSoundExportTrack = NewMovieTrack (m_oExportMovie, 0, 0, kFullVolume);

		//*******Create a SoundDescription*******

		//End creating SoundDescriptor

		//*****Trying another approach********
		SoundComponentData sndCompDataIn;
		sndCompDataIn.flags = 0;
		sndCompDataIn.format = kSoundNotCompressed;
		sndCompDataIn.numChannels = 2;
		sndCompDataIn.sampleSize = 16;
		sndCompDataIn.sampleRate = rate44khz;
		sndCompDataIn.sampleCount = 0;
		sndCompDataIn.buffer = 0;
		sndCompDataIn.reserved = 0;

		SoundComponentData sndCompDataOut;
		sndCompDataOut.flags = 0;
		sndCompDataOut.format = m_vCurrentAudioCodecToUse;
		sndCompDataOut.numChannels = m_vNumChannels;
		sndCompDataOut.sampleSize = m_vSampleSize;
		sndCompDataOut.sampleRate = m_vSampleRate;
		sndCompDataOut.sampleCount = 0;
		sndCompDataOut.buffer = 0;
		sndCompDataOut.reserved = 0;

		errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC);

		//Add a mediafile to the track
		TimeScale	myTimeScale;
		myTimeScale = (sndCompDataOut.sampleRate >> 16);
		m_oSoundExportMedia = NewTrackMedia (m_oSoundExportTrack, SoundMediaType, myTimeScale, NULL, 0);

		//Begin edditing sound media
		errCode = BeginMediaEdits (m_oSoundExportMedia);


		//Get information about the samples input size and outputsize
		m_vOutputSoundBytes = 0;

		// ignore if this gives an error, since some codecs don't use this selector (makes QDesign work)
		errCode = SoundConverterSetInfo(m_oSC, siCompressionChannels, &sndCompDataOut.numChannels);		

		errCode = SoundConverterGetBufferSizes (m_oSC, m_opMediaFormatForAudio -> m_vBufferSizeBytes, &m_vInputSoundFrames, &m_vInputSoundBytes, &m_vOutputSoundBytes );

		// create destination data buffer
		m_vpSoundDataDestPtr = NULL;
		m_vpSoundDataDestPtr = NewPtrClear(m_vOutputSoundBytes);

		errCode = SoundConverterGetInfo(m_oSC, siCompressionFactor, &m_vDestCompInfo);					

		m_vDestCompInfo.bytesPerFrame = m_vDestCompInfo.bytesPerPacket * m_vNumChannels;//theDestInfo.numChannels;


		//******Create sound descriptor******
		m_mySampleDesc = (SoundDescriptionV1Handle)NewHandleClear(sizeof(SoundDescriptionV1));	

		(*m_mySampleDesc) -> desc.descSize			= sizeof(SoundDescriptionV1);
		(*m_mySampleDesc) -> desc.dataFormat		= m_vCurrentAudioCodecToUse;//theDestInfo.format;						
		(*m_mySampleDesc) -> desc.resvd1			= 0;									
		(*m_mySampleDesc) -> desc.resvd2			= 0;									
		(*m_mySampleDesc) -> desc.dataRefIndex		= 1;									
		(*m_mySampleDesc) -> desc.version			= 1;									
		(*m_mySampleDesc) -> desc.revlevel			= 0;									
		(*m_mySampleDesc) -> desc.vendor			= 0;									
		(*m_mySampleDesc) -> desc.numChannels		= m_vNumChannels;//theDestInfo.numChannels;					
		(*m_mySampleDesc) -> desc.sampleSize		= m_vSampleSize; //theDestInfo.sampleSize;					 
		(*m_mySampleDesc) -> desc.compressionID		= 0;									
		(*m_mySampleDesc) -> desc.packetSize		= 0;									
		(*m_mySampleDesc) -> desc.sampleRate		= m_vSampleRate;//44100;//theDestInfo.sampleRate;
		(*m_mySampleDesc) -> samplesPerPacket 		= m_vDestCompInfo.samplesPerPacket;
		(*m_mySampleDesc) -> bytesPerPacket 		= m_vDestCompInfo.bytesPerPacket;
		(*m_mySampleDesc) -> bytesPerFrame 			= m_vDestCompInfo.bytesPerFrame;
		(*m_mySampleDesc) -> bytesPerSample 		= m_vDestCompInfo.bytesPerSample;

	}


	return true;
}

void EMWinQuickTimeWriter::GetVideoEncoderIDs(unsigned long* p_vpIDs)
{
	//Get number of encoders in the system
	CodecNameSpecListPtr	list;
	CodecNameSpec*	sCurrentCodecPtr;
	CodecNameSpec	sCurrentCodec;

	GetCodecNameList(&list, 1);

	sCurrentCodecPtr = list->list;

	for(int index = 0; index < list->count; index++)
	{
		sCurrentCodec = sCurrentCodecPtr[index];
		
		p_vpIDs[index] = sCurrentCodec.cType;
	}
}

void EMWinQuickTimeWriter::GetAudioEncoderIDs(unsigned long* p_vpIDs)
{
	//Find its component
	OSErr errCode;
	ComponentDescription cd;
	Component c = 0;
	cd.componentType = kSoundCompressor; //1768776559 (this number will look for all video encoders)
	cd.componentSubType = 0;//currentCodec.cType;//0;
	cd.componentManufacturer = 0;
	cd.componentFlags = 0; //canMovieExportFiles;
	cd.componentFlagsMask = 0;//graphicsExporterIsBaseExporter;
	int index = 0;
	while((c = FindNextComponent(c, &cd)) != 0) 
	{
		if(errCode = GetComponentInfo(c, &cd, NULL, NULL, NULL) == 0)
			p_vpIDs[index++] = cd.componentSubType;
		
		cd.componentType = kSoundCompressor; //1768776559 (this number will look for all video encoders)
		cd.componentSubType = 0;//currentCodec.cType;//0;
		cd.componentManufacturer = 0;
		cd.componentFlags = 0; //canMovieExportFiles;
		cd.componentFlagsMask = 0;//graphicsExporterIsBaseExporter;
	}

}


void EMWinQuickTimeWriter::SetVideoEncoderToUse(int32 p_vEncoderID)
{
	m_vCurrentVideoCodecToUse = (OSType)p_vEncoderID;
/*	m_vEncoderID = p_vEncoderID;
	//Check for codecs in the system
	CodecNameSpecListPtr	list;
	CodecNameSpec*	sCurrentCodecPtr;
	//CodecInfo info;

	GetCodecNameList(&list, 1); //short showAll );

	sCurrentCodecPtr = list->list;
	m_sCurrentCodec = sCurrentCodecPtr[m_vEncoderID];
	m_vCurrentVideoCodecToUse = m_sCurrentCodec.cType;
	*/

}

void EMWinQuickTimeWriter::SetAudioEncoderToUse(int32 p_vEncoderID)
{
	m_vCurrentAudioCodecToUse = (OSType)p_vEncoderID;

}


void EMWinQuickTimeWriter::SetCompressionCapabilityInfo(int32 p_vEncoderID, int32  p_vPropertyID)
{
	GetCompressionCapabilities(p_vEncoderID);
	int vPos;

	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find(" 8000") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(8000) * static_cast<unsigned long>(65536);
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("11025") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(11025) * static_cast<unsigned long>(65536);
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("11127") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(11127) * static_cast<unsigned long>(65536);
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("16000") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(16000) * static_cast<unsigned long>(65536);
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("22050") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(22050) * static_cast<unsigned long>(65536);
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("22255") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(22255) * static_cast<unsigned long>(65536);
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("32000") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(32000) * static_cast<unsigned long>(65536);
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("44100") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(44100) * static_cast<unsigned long>(65536);
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("48000") != std::string::npos )
		m_vSampleRate = static_cast<unsigned long>(48000) * static_cast<unsigned long>(65536);

	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("8 Bit") != std::string::npos )
		m_vSampleSize = 8;
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("16 Bit") != std::string::npos )
		m_vSampleSize = 16;

	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("Mono") != std::string::npos )
		m_vNumChannels = 1;
	else
	if( vPos = m_opAudioCompressionCapabilityArray[p_vPropertyID].find("Stereo") != std::string::npos )
		m_vNumChannels = 2;

}

std::string EMWinQuickTimeWriter::GetEncoderName(int32 p_vEncoderID)
{
	OSErr errCode;
	ComponentDescription cd;
	Component c = 0;
	ComponentInstance	myExporter = NULL;
	Handle componentName = NewHandle(0);
	std::string componentStringName;

	ComponentResult	myErr = badComponentType;
	cd.componentType = 0; //1768776559 (this number will look for all video encoders)
	cd.componentSubType = (OSType)p_vEncoderID;//currentCodec.cType;//0;
	cd.componentManufacturer = 0;
	cd.componentFlags = 0; //canMovieExportFiles;
	cd.componentFlagsMask = 0;//graphicsExporterIsBaseExporter;


	char* compressionName = new char[255];

//	errCode = FindCodec ((OSType)p_vEncoderID, anyCodec, &compressor, &decompressor);
		
	c = FindNextComponent(c, &cd);
	//	errCode = GetComponentInfo(c, &cd, NULL, NULL, NULL);
	//	if(cd.componentType == (OSType)p_vEncoderID)

//	myExporter = OpenComponent(compressor);

	GetComponentInfo (c, NULL, componentName, NULL, NULL );

//	CloseComponent(myExporter);


	BlockMoveData(*componentName, compressionName, GetHandleSize(componentName));

	p2cstr ((unsigned char*)compressionName);

	componentStringName = compressionName;
	return componentStringName;
}

char* EMWinQuickTimeWriter::GetCompressionCapabilities(int32 p_vEncoderID)
{
	OSErr errCode;
	ComponentDescription cd;
	Component c = 0;
	ComponentInstance	myExporter = NULL;
	int vIndex;

	ComponentResult	myErr = badComponentType;
	cd.componentType = kSoundCompressor; //1768776559 (this number will look for all video encoders)
	cd.componentSubType = (OSType) p_vEncoderID;//currentCodec.cType;//0;
	cd.componentManufacturer = 0;
	cd.componentFlags = 0; //canMovieExportFiles;
	cd.componentFlagsMask = 0;//graphicsExporterIsBaseExporter;

	//Step forward until wanted component
	c = FindNextComponent(c, &cd);
	GetComponentInfo(c, &cd, NULL, NULL, NULL);

	SoundComponentData sndCompDataIn;
	sndCompDataIn.flags = 0;
	sndCompDataIn.format = kSoundNotCompressed;
	sndCompDataIn.sampleCount = 0;
	sndCompDataIn.buffer = 0;
	sndCompDataIn.reserved = 0;
	sndCompDataIn.sampleSize = 16;
	sndCompDataIn.numChannels = 2;

	SoundComponentData sndCompDataOut;
	sndCompDataOut.flags = 0;
	sndCompDataOut.format = cd.componentSubType;
	sndCompDataOut.sampleCount = 0;
	sndCompDataOut.buffer = 0;
	sndCompDataOut.reserved = 0;


	unsigned long rateValue = static_cast<unsigned long>(44100) * static_cast<unsigned long>(65536);
	sndCompDataIn.sampleRate = rateValue;
	sndCompDataOut.sampleRate = rateValue;//rate44khz;

	bool manageMono8 = false;
	bool manageMono16 = false;
	bool manageStereo8 = false;
	bool manageStereo16 = false;
	int vNumberOfPropertyMultipliers = 0;

	sndCompDataOut.numChannels = 1;
	sndCompDataOut.sampleSize = 8;
	if(errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC) == 0)
	{
		manageMono8 = true;
		vNumberOfPropertyMultipliers++;
	}
	sndCompDataOut.numChannels = 1;
	sndCompDataOut.sampleSize = 16;
	if(errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC) == 0)
	{
		manageMono16 = true;
		vNumberOfPropertyMultipliers++;
	}
	sndCompDataOut.numChannels = 2;
	sndCompDataOut.sampleSize = 8;
	if(errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC) == 0)
	{
		manageStereo8 = true;
		vNumberOfPropertyMultipliers++;
	}
	sndCompDataOut.numChannels = 2;
	sndCompDataOut.sampleSize = 16;
	if(errCode = SoundConverterOpen (&sndCompDataIn, &sndCompDataOut, &m_oSC) == 0)
	{
		manageStereo16 = true;
		vNumberOfPropertyMultipliers++;
	}

	m_opAudioCompressionCapabilityArray = new std::string[9 * vNumberOfPropertyMultipliers+1];
	for(vIndex=0; vIndex<9 * vNumberOfPropertyMultipliers+1; vIndex++)
		m_opAudioCompressionCapabilityArray[vIndex] = "";

	vIndex=0;
	if(manageMono8)
	{
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string(" 8000 Hz, ") + std::string("8 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("11025 Hz, ") + std::string("8 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("11127 Hz, ") + std::string("8 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("16000 Hz, ") + std::string("8 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("22050 Hz, ") + std::string("8 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("22255 Hz, ") + std::string("8 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("32000 Hz, ") + std::string("8 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("44100 Hz, ") + std::string("8 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("48000 Hz, ") + std::string("8 Bit, Mono");
	}
	if(manageMono16)
	{
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string(" 8000 Hz, ") + std::string("16 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("11025 Hz, ") + std::string("16 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("11127 Hz, ") + std::string("16 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("16000 Hz, ") + std::string("16 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("22050 Hz, ") + std::string("16 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("22255 Hz, ") + std::string("16 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("32000 Hz, ") + std::string("16 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("44100 Hz, ") + std::string("16 Bit, Mono");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("48000 Hz, ") + std::string("16 Bit, Mono");
	}
	if(manageStereo8)
	{
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string(" 8000 Hz, ") + std::string("8 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("11025 Hz, ") + std::string("8 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("11127 Hz, ") + std::string("8 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("16000 Hz, ") + std::string("8 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("22050 Hz, ") + std::string("8 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("22255 Hz, ") + std::string("8 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("32000 Hz, ") + std::string("8 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("44100 Hz, ") + std::string("8 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("48000 Hz, ") + std::string("8 Bit, Stereo");
	}
	if(manageStereo16)
	{
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string(" 8000 Hz, ") + std::string("16 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("11025 Hz, ") + std::string("16 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("11127 Hz, ") + std::string("16 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("16000 Hz, ") + std::string("16 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("22050 Hz, ") + std::string("16 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("22255 Hz, ") + std::string("16 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("32000 Hz, ") + std::string("16 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("44100 Hz, ") + std::string("16 Bit, Stereo");
		m_opAudioCompressionCapabilityArray[vIndex++] += std::string("48000 Hz, ") + std::string("16 Bit, Stereo");
	}

	return (char*) m_opAudioCompressionCapabilityArray;
	
}


int EMWinQuickTimeWriter::GetNumberOfVideoEncoders()
{
	//Get number of encoders in the system
	CodecNameSpecListPtr	list;

	GetCodecNameList(&list, 1);

	return list -> count;
}

int EMWinQuickTimeWriter::GetNumberOfAudioEncoders()
{
	ComponentDescription cd;
	cd.componentType = kSoundCompressor; //1768776559 (this number will look for all video encoders)
	cd.componentSubType = 0;//currentCodec.cType;//0;
	cd.componentManufacturer = 0;
	cd.componentFlags = 0; //canMovieExportFiles;
	cd.componentFlagsMask = 0;//graphicsExporterIsBaseExporter;

	return CountComponents(&cd);
}


void EMWinQuickTimeWriter::DisConnectWithEncoders()
{

}

void EMWinQuickTimeWriter::StopWritingToDisk()
{
	OSErr errCode;

	//Saves all changes that have been made to the VideoMedia into the file and close the file
	if(m_oMovieExportMedia != NULL)
		errCode = EndMediaEdits (m_oMovieExportMedia);
	if(m_oSoundExportMedia != NULL)
		errCode = EndMediaEdits (m_oSoundExportMedia);

	if(m_oMovieExportTrack != NULL)
		errCode = InsertMediaIntoTrack (m_oMovieExportTrack, kTrackStart, kMediaStart, GetMediaDuration (m_oMovieExportMedia), fixed1);

	if(m_oSoundExportTrack != NULL)
		errCode = InsertMediaIntoTrack (m_oSoundExportTrack, kTrackStart, kMediaStart, GetMediaDuration (m_oSoundExportMedia), FixRatio(1, 1));

	short nResID = movieInDataForkResID;
	AddMovieResource(m_oExportMovie, m_vFileRefNum, &nResID, NULL);

	//Close the movie file
    if (m_vFileRefNum != 0)
        CloseMovieFile(m_vFileRefNum); 
    DisposeMovie(m_oExportMovie);
	m_oExportMovie = NULL;
	m_opMediaFormatForVideo = NULL;
	m_opMediaFormatForAudio = NULL;
	m_vFileRefNum = 0;

	m_oMovieExportTrack = NULL;
	m_oMovieExportMedia = NULL;
	m_oSoundExportTrack = NULL;
	m_oSoundExportMedia = NULL;

}

void EMWinQuickTimeWriter::ProcessVideoBuffer(EMMediaDataBuffer* p_opDataBuffer)
{
	if(m_oExportMovie != NULL)
	{

		m_oScale.Scale(p_opDataBuffer -> Data(), p_opDataBuffer -> BackBuffer());

		//***********HERE CHANGES TO THE MEDIA FILE SHOULD BE MADE (ADDING FRAMES FOR EXAMPLE)************
		AddVideoSamplesToMedia(m_oMovieExportMedia, m_oGWorld, p_opDataBuffer -> BackBuffer()/*p_opDataBuffer->Data()*/, m_vCurrentVideoCodecToUse);
	}
}

void EMWinQuickTimeWriter::ProcessAudioBuffer(EMMediaDataBuffer* p_opDataBuffer)
{
	OSErr errCode;
	
	if(m_oExportMovie != NULL)
	{
		unsigned long vOutputSoundFrames = 0;

		errCode = SoundConverterConvertBuffer (m_oSC, p_opDataBuffer -> Data(), m_vInputSoundFrames, m_vpSoundDataDestPtr, &vOutputSoundFrames, &m_vOutputSoundBytes );

		Handle vDestHandle;
		vDestHandle = NewHandleClear(0);

		errCode = PtrAndHand(m_vpSoundDataDestPtr, vDestHandle, m_vOutputSoundBytes);


		//End making SoundDescriptionV1Handle

		unsigned long myLengthInSamples = vOutputSoundFrames * m_vDestCompInfo.samplesPerPacket;
		errCode = AddMediaSample(m_oSoundExportMedia,
								vDestHandle,
								0,
								vOutputSoundFrames * m_vDestCompInfo.bytesPerFrame,
								1, 
						 		(SampleDescriptionHandle)m_mySampleDesc,
						 		myLengthInSamples,
						 		0,
						 		NULL);
		if(errCode!=0)
		{
			int breakHere=0;
		}
	}
}


void EMWinQuickTimeWriter::AddVideoSamplesToMedia (Media theMedia, GWorldPtr p_opGWorld, void* p_vBitmapDataPtr, OSType p_cType)
{
	OSErr err = noErr;

	(**p_opGWorld -> portPixMap).baseAddr = (char*)p_vBitmapDataPtr;

	LockPixels (p_opGWorld -> portPixMap);
	
	EraseRect (&m_sTrackFrame);

	err = CompressImage (p_opGWorld -> portPixMap, 
							&m_sTrackFrame, 
							m_vQuality, //codecNormalQuality,
							p_cType,
							m_pImageDesc, 
							m_pCompressedDataPtr );

	err = AddMediaSample(theMedia, 
							m_pCompressedData,
							kNoOffset,
							(**m_pImageDesc).dataSize, 
							m_vSampleDuration,
							(SampleDescriptionHandle)m_pImageDesc, 
							kAddOneVideoSample,	
							kSyncSample,
							NULL);

/*	if (imageDesc)
	{
		DisposeHandle ((Handle)imageDesc);
	}
	if (compressedData)
	{
		DisposeHandle (compressedData);
	}
*/
}

bool EMWinQuickTimeWriter::ExtractAudioFromQuickTimeFile(const char* p_opSource, const char* p_opDestination)
{
	//Attributes
	FSSpec fileSpec;	// Descriptive information on file to open
	OSErr errCode;		// Result code
	short theFile;		// Reference number of movie file
	Movie theMovie = NULL;		// Movie identifier
	Track track = NULL;	//Track identifier

	//Find the media in the quicktime source file
	c2pstr ((char*)p_opSource);
	errCode = FSMakeFSSpec
			(0,					// Volume reference number
			0,					// ID of parent directory
			(unsigned char*)p_opSource,			// File name
			&fileSpec);			// Returns a specification record
	p2cstr ((unsigned char*)p_opSource);

	if ( errCode != noErr )										// Was there an error?
    {
		return false;
	}

	errCode = OpenMovieFile (&fileSpec, &theFile, fsRdPerm);	// Open the movie file
	if ( errCode != noErr )										// Was there an error?
    {
		return false;
	}


	errCode = NewMovieFromFile (&theMovie, theFile, nil, nil, newMovieActive, nil); //Get Movie from file
	
	CloseMovieFile (theFile);                                  // Close the file

	if ( errCode != noErr )															// Was there an error?
    {
		return false;
	}

	//Find the sound track
	for (int lIndex = 1; lIndex <= GetMovieTrackCount(theMovie); lIndex++)
	{
		OSType      dwType;
		track = GetMovieIndTrack(theMovie, lIndex);

		GetMediaHandlerDescription(GetTrackMedia(track), &dwType, nil, nil);

		if (dwType == SoundMediaType)
		{
			break;
		}
	}

	short	nFileRefNum = 0;
	Movie exportMovie = NULL;		// Movie identifier
	FSSpec fileExportSpec;
	c2pstr ((char*)p_opDestination);

	//Create the wave outputfile
	errCode = FSMakeFSSpec(0, 0, (unsigned char*)p_opDestination, &fileExportSpec);
	p2cstr ((unsigned char*)p_opDestination);
    errCode = CreateMovieFile(&fileExportSpec, FOUR_CHAR_CODE('TVOD'), smCurrentScript, createMovieFileDeleteCurFile |createMovieFileDontCreateResFile, &nFileRefNum, &exportMovie);
	if ( errCode != noErr )	// Was there an error?
    {
		return false;
	}

	//***************CONVERT MOVIE SOUND TO WAVE*********************
	SoundDescriptionHandle desc;
	ComponentInstance ci;
	ci = OpenDefaultComponent(MovieExportType, kQTFileTypeWave);




	desc = (SoundDescriptionHandle )NewHandleClear(sizeof(SoundDescription));
	(**desc).descSize = sizeof(SoundDescription);
	(**desc).sampleSize = 16;
	(**desc).numChannels = 2;
	(**desc).sampleRate = 44100L << 16;
	(**desc).dataFormat = k16BitLittleEndianFormat;


	//Specify the export component in which format you want the audio.
	errCode = MovieExportSetSampleDescription(ci, (SampleDescriptionHandle)desc, SoundMediaType);



	//Perform the export operation.
	errCode = ConvertMovieToFile(theMovie, track, &fileExportSpec, kQTFileTypeWave, FOUR_CHAR_CODE('TVOD'), smSystemScript, nil, 0, ci);


	CloseMovieFile (nFileRefNum);
	//After you have finished, dispose everything that you have created.
	CloseComponent(ci);
	DisposeHandle((Handle)desc);
	if(errCode == -2126)
	{
		MessageBox(NULL, "This media file is copy protected and for this reason the sound can not be extracted.\nThe video will still be imported, but without the audio.", "Media file import", MB_OK | MB_ICONINFORMATION);
		DeleteFile(p_opDestination);
		return false;
	}
	else if(errCode != 0)
	{
		MessageBox(NULL, string(string("The audio in \"") + string(p_opSource) + string("\" could not be extracted.\nYou may be missing a codec, or the file may be copy protected.\nThe video will still be imported, but without the audio.")).c_str(), "Media file import", MB_OK | MB_ICONINFORMATION);
		DeleteFile(p_opDestination);
		return false;
	}
	//***************END CONVERT MOVIE SOUND*********************

	return true;
}

