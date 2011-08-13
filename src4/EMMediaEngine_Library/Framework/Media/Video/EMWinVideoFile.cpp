

#include "EMGlobals.h"
#include "EMMediaEngine.h"
#ifdef PLATFORM_WINDOWS //PLATFORM_BEOS
//#include "EMReadFrameuids.h"

#include "EMWinVideoFile.h"
#include "EMVideoFile.h"

#include "EMWinMediaUtility.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaEngine.h"
#include "EMMediaFormat.h"
#include "EMMediaIDManager.h"
#include "EMMediaTimer.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
//#include "EMReadFrame.h"

#include "EMVideoPreview.h"
#include "EMBitmap.h"
#include "EMWinBitmap.h"


EMWinVideoFile::EMWinVideoFile(string p_oFileName)
	:	EMVideoFile(p_oFileName),
		m_opBufferContent(NULL),
		m_opBufferSecondary(NULL),
		m_opBufferSwapping(NULL),
		m_vDcols(0),
		m_opDestinationBitMap(NULL),
		m_vDrows(0),
		//m_opFile(NULL),
		m_oFileName(p_oFileName),
		m_opFormat(NULL),
		m_vIsInitialized(false),
		m_vOnlyOnce(true),
		m_opR(NULL),
		m_vSeeked(false),
		m_opSourceBitMap(NULL),
		m_vTimeContinuousZoomScaleHeight(0),
		m_vTimeContinuousZoomScaleWidth(0),
		m_vTotalNumberOfFrames(0),
		m_opMediaDataBuffer(NULL),
		m_opSeekSemaphore(CreateSemaphore(NULL, 1, 1, NULL)),
		m_vNeedToSyncWithTime(FALSE),
		m_vNeedToSyncWithTimeDone(FALSE),
		m_opTempBuffer(NULL),
		m_vBelongedToUniqueID(-1),
		m_pfrm(NULL),
		m_oQuickTimeVideo(NULL),
		m_opSource(EM_new EMMediaFormat(EM_TYPE_ANY_VIDEO)),
		m_opDestination(EM_new EMMediaFormat(EM_TYPE_ANY_VIDEO))
		//m_opTrackOut(NULL)

{
	m_opFormat = new EMMediaFormat(EM_TYPE_RAW_VIDEO);
	m_vTimeZoomScale = 0;//*(static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	m_vWorkFPS = 0;//*(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	m_vTimeZoomScaleWidth = 0;// static_cast<int64>((1000000.0 / m_vWorkFPS) / m_vTimeZoomScale);
	//EMMediaTimer::Instance() -> AddListener(this);
	m_opVideoPreview = EMMediaEngine::Instance() -> GetVideoPreviewWindow();
	m_vMagicSpeedValue = 1.0;
}

EMWinVideoFile::~EMWinVideoFile()
{	
	//EMMediaTimer::Instance() -> RemoveListener(this);

	AVIFileExit();

	if(m_oQuickTimeVideo != NULL)
		delete m_oQuickTimeVideo;

	if(m_opFormat != NULL)
	{
		delete m_opFormat;
		m_opFormat = NULL;
	}

	delete m_opSource;
	delete m_opDestination;

	if(m_opBufferContent != NULL)
	{
		delete m_opBufferContent;
		m_opBufferContent = NULL;
	}

	if(m_opMediaDataBuffer != NULL)
	{
		delete m_opMediaDataBuffer;
		m_opMediaDataBuffer = NULL;
	}

	if(m_opSourceBitMap != NULL)
	{
		DeleteObject(m_opSourceBitMap);
		m_opSourceBitMap = NULL;
	}
	if(m_opDestinationBitMap != NULL)
	{
		DeleteObject(m_opDestinationBitMap);
		m_opDestinationBitMap = NULL;
	}

	CloseHandle(m_opSeekSemaphore);		//Kaneda - Buggfix
	m_opSeekSemaphore = 0;

//	HRESULT err = m_opIReadFrame->StopGraph(); 

/*	m_opSeek -> Release();
	m_opSeek = NULL;

	m_opIReadFrame -> Release();
	m_opIReadFrame = NULL;
*/
/*	unsigned long vJunk;

	IEnumFilters *opIEnum  = NULL;
	IEnumPins	*opIEnumP    = NULL;
	IBaseFilter *opIFilter = NULL;
	IPin		*opIPin    = NULL;
	IPin		*opIPin2    = NULL;
	HRESULT hr;

	if (m_opGraph != NULL)
	{
		hr = m_opGraph -> EnumFilters(&opIEnum);
		hr = opIEnum -> Reset();

		while (opIEnum -> Next(1, &opIFilter, &vJunk) == S_OK)
		{
			hr = opIFilter -> EnumPins(&opIEnumP);
			hr = opIEnumP -> Reset();
	
			while (opIEnumP -> Next(1, &opIPin, &vJunk) == S_OK)
			{
				if (opIPin -> ConnectedTo(&opIPin2) == S_OK)
				{
					hr = m_opGraph -> Disconnect(opIPin);
					hr = m_opGraph -> Disconnect(opIPin2);

					vJunk = opIPin2 -> Release();
					opIPin2 = NULL;
				}
				vJunk = opIPin -> Release();
				opIPin = NULL;
			}
			vJunk = opIEnumP -> Release();
			opIEnumP = NULL;

			hr = m_opGraph -> RemoveFilter(opIFilter);
			vJunk= opIFilter -> Release();
			opIFilter = NULL;

			opIEnum -> Reset();
		}

		vJunk= opIEnum -> Release();
		opIEnum = NULL;

		vJunk = m_opGraph -> Release();
		m_opGraph = NULL;
	}

	FreeMediaType(m_opType);
	*/
}

float* EMWinVideoFile::GetAspectRatio()
{
	
	return &vAspectRatio;
}

int64 EMWinVideoFile::GetBitmapAtTimePosition(int64* p_vpToTime, void* p_opBitmap)
{

//	m_opIReadFrame -> GrabFrame(p_opBitmap, m_vStart_Time);//, m_oBitmapInfoTrash);
//	return 0;
//	LockVideo();
	HRESULT err = S_FALSE;//B_ERROR;
	int64 vTempStartTime;
	int64 vToTime = *p_vpToTime;
	if(m_vIsInitialized)
	{
		//err = m_opTrackOut -> SeekToTime(p_vpToTime, B_MEDIA_SEEK_CLOSEST_BACKWARD);
//		eo << "ENTERING SEEKtoTIME" << ef;

//	if(m_opBufferSecondary == NULL)
//	{
		m_opIReadFrame ->SeekToTime(*p_vpToTime);
//	}
//		eo << "LEAVING SEEKtoTIME" << ef;

/*		if(err != S_OK)
		{
			emerr << "ERROR: " << strerror(err) << endl;
			EMDebugger("Couldn't Seek!?");
		}
*/
	}
	else
		EMDebugger("DIE");
	if(m_opBufferSecondary != NULL)
	{
//		eo << "ENTERING GrabFrame1" << ef;
		m_opIReadFrame -> GrabFrame(m_opBufferSecondary, m_vStart_Time);
//		eo << "LEAVING GrabFrame1" << ef;
	}
	else
	{
		//err = m_opTrackOut -> ReadFrames(p_opBitmap, &framecount, &m_opMediaHeader);
				//Get the next frame
		MessageBox(NULL, "No Secondary Buffer","EMWinVideoFile",MB_OK);
//		m_opIReadFrame -> GrabFrame(p_opBitmap, m_vStart_Time);//, m_oBitmapInfoTrash);
	}
	vTempStartTime = m_vStart_Time;//m_opMediaHeader.start_time;


/*	if(err != B_OK)
	{
		m_opIReadFrame -> SeekToTime(*p_vpToTime);

		m_opIReadFrame -> GrabFrame(p_opBitmap, m_vStart_Time);//, m_oBitmapInfoTrash);
		
	}
*/
	if(vToTime == 0 && m_vStart_Time != 0)
	{
//		MessageBox(NULL, "WRONG WRONG WRONG","WRONG WRONG WRONG",MB_OK);
	}
	if(m_vStart_Time == vToTime && m_opBufferSecondary != NULL)
	{
		memcpy(p_opBitmap, m_opBufferSecondary -> Data(), m_opBufferSecondary->m_vSizeAvailable);//GetSize());
		//err = m_opTrackOut -> ReadFrames(m_opBufferSecondary -> Data(), &framecount, &m_opMediaHeader);
//		eo << "ENTERING GrabFrame2" << ef;
		m_opIReadFrame -> GrabFrame(m_opBufferSecondary , m_vStart_Time);
//		eo << "LEAVING GrabFrame2" << ef;
		*p_vpToTime = m_vStart_Time;//m_opMediaHeader.start_time;
		//UnlockVideo();
		eo << "LEAVING EMWinVideoFile::GetBitmapAtTimePosition" << ef;
		return vTempStartTime;
	}
	
//	char* vpNumber = new char[10];
//	char* vpNumber2 = new char[10];
	while(m_vStart_Time < vToTime)
	{
		vTempStartTime = m_vStart_Time;

		if(m_opBufferSecondary != NULL)
		{
			memcpy(p_opBitmap, m_opBufferSecondary -> Data(), m_opBufferSecondary->m_vSizeAvailable);// GetSize());
//			eo << "ENTERING GrabFrame3" << ef;
			m_opIReadFrame -> GrabFrame(m_opBufferSecondary, m_vStart_Time);
//			eo << "LEAVING GrabFrame3" << ef;
		}
		else
		{
		MessageBox(NULL, "No Secondary Buffer - 2","EMWinVideoFile",MB_OK);
//			memcpy(m_opBufferContent, p_opBitmap, GetSize());
//			m_opIReadFrame -> GrabFrame(p_opBitmap, m_vStart_Time,m_opBufferSecondary->m_oFormat.m_vWidth, m_opBufferSecondary->m_oFormat.m_vHeight, m_opBufferSecondary->m_oFormat.m_vBytesPerRow);//, m_oBitmapInfoTrash);
		}
//		eo << "Hopfully done with EMWinVideoFile::GetBitmapAtTimePosition" << ef;
		
//		_itoa(m_vStart_Time, vpNumber, 10);
//		_itoa(vToTime, vpNumber2, 10);
//		eo << vpNumber << " < " << vpNumber2 << ef;
		/*if(err != B_OK)
		{
			emerr << "ERROR:" << strerror(err) <<endl;
			EMDebugger("ERROR! Went out of bounds in EMWinVideoFile::SeekToTime()");
		}
		*/
	}
//	delete vpNumber;
	if(m_vStart_Time > vToTime)
	{
		*p_vpToTime = m_vStart_Time;//m_opMediaHeader.start_time;
		//UnlockVideo();
		eo << "LEAVING EMWinVideoFile::GetBitmapAtTimePosition" << ef;
		return vTempStartTime;
	}
	
	int64 vRealStartTime = m_vStart_Time;//m_opMediaHeader.start_time;
	//err = m_opTrackOut -> ReadFrames(m_opBufferContent, &framecount, &m_opMediaHeader);

//	MessageBox(NULL, "No Secondary Buffer - 3","EMWinVideoFile",MB_OK);	
//	m_opIReadFrame -> GrabFrame(m_opBufferContent, m_vStart_Time,m_opBufferSecondary->m_oFormat.m_vWidth, m_opBufferSecondary->m_oFormat.m_vHeight, m_opBufferSecondary->m_oFormat.m_vBytesPerRow);//, m_oBitmapInfoTrash);
	*p_vpToTime = m_vStart_Time;	
//	UnlockVideo();

//	eo << "LEAVING EMWinVideoFile::GetBitmapAtTimePosition" << ef;
	return vRealStartTime;
}

void EMWinVideoFile::SetMagicSpeedValue(float p_vMagicSpeedValue)
{
	m_vMagicSpeedValue = p_vMagicSpeedValue;
}

float EMWinVideoFile::GetMagicSpeedValue()
{
	return m_vMagicSpeedValue;
}


int64 EMWinVideoFile::GetDuration()
{
	//Counting microseconds is to be MUCH more accurate than below! Change when time is available
	//This should be correct now :)
	if(m_vIsInitialized)
	{
		float vDur = m_vTotalDuration;
		float vFPS = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

		float tmp = static_cast<float>(vFPS * vDur / 1000000.0f) * m_vMagicSpeedValue;
		tmp /= vFPS;

		return static_cast<int64>(tmp * 1000000.0f);

	}

	return 0;
}

EMMediaFormat* EMWinVideoFile::GetFormat()
{
		return m_opFormat;
}

/*int32 EMWinVideoFile::GetID() const
{
	return m_vID;
}
*/
int64 EMWinVideoFile::GetSize() const
{
	if(m_vIsInitialized)
	{
		return m_opVideoHeader->bmiHeader.biSizeImage;//biHeight*m_pVideoHeader->bmiHeader.biWidth*(m_pVideoHeader -> bmiHeader.biBitCount/8);
	}

	return 0;
}

bool EMWinVideoFile::InitCheckE()
{

	if(m_vIsInitialized)
		return true;

	//Is it AVI??
	AVIFileInit();

	if (AVIFileOpen(&m_pavi,m_oFileName.c_str(),OF_READ | OF_SHARE_DENY_NONE,NULL)!=0)
	{ //NO it is not AVi, is it QUICKTIME?
		m_oQuickTimeVideo = new EMWinQuicktimeVideoSuper(m_oFileName.c_str());
		m_oQuickTimeVideo -> InitCheckE();
		m_vTotalNumberOfFrames = 100;
		m_vTotalDuration = m_oQuickTimeVideo -> GetDuration();
		vAspectRatio = (static_cast<float>(m_oQuickTimeVideo -> GetWidth())/static_cast<float>(m_oQuickTimeVideo -> GetHeight()));

		m_opPreviewWindowSize = 0;
	
		m_Position = 0; //Start from the beginning
		//HBITMAP opDestinationBitMap = m_oQuickTimeVideo->GetDIBAtTime(55, 42, 10000000);

		char vDimensionWidth[12];
		char vDimensionHeight[12];
		itoa(m_oQuickTimeVideo -> GetWidth(), vDimensionWidth, 10);
		itoa(m_oQuickTimeVideo -> GetHeight(), vDimensionHeight, 10);
		m_oDimensionString = "";
		m_oDimensionString  += string(vDimensionWidth) + string(" x ") + string(vDimensionHeight);

		//m_DwRate = 
		m_vIsInitialized = true;
		return true;
	} else
	{
		//Yes it was AVI
		int vStreamNumber = 0;
		bool vIsVideo = false;
		while(AVIFileGetStream(m_pavi, &m_pstm, 0, vStreamNumber) == 0)
		{
			m_pstm -> Info(&m_psi, sizeof(AVISTREAMINFOW));
			if(m_psi.fccType == streamtypeVIDEO)
			{
				vIsVideo = true;
				break;
			}
			vStreamNumber++;
		}

		//This was no AVI file
		if(!vIsVideo)
			return false;

		//Get Total number of frames in video file
		m_vTotalNumberOfFrames = m_psi.dwLength;

		m_vDwRate = static_cast<double>(m_psi.dwRate / m_psi.dwScale);
		/*if(m_psi.dwRate > 99)
		{
			m_vDwRate = static_cast<float>(m_psi.dwRate / 100.0);
		}*/
		//Get Total duration of the video file in nanoSeconds
		m_vTotalDuration = static_cast<int64>(m_vTotalNumberOfFrames / m_vDwRate * 1000000.0); //Have to make it microseconds (Because Rille says so, and because it should comply to BeOS)

		vAspectRatio = (static_cast<float>(m_psi.rcFrame.right)/static_cast<float>(m_psi.rcFrame.bottom));
		
		char vDimensionWidth[12];
		char vDimensionHeight[12];
		itoa(m_psi.rcFrame.right, vDimensionWidth, 10);
		itoa(m_psi.rcFrame.bottom, vDimensionHeight, 10);
		m_oDimensionString = "";
		m_oDimensionString  += string(vDimensionWidth) + string(" x ") + string(vDimensionHeight);

		m_opSource -> m_vWidth = m_psi.rcFrame.right;
		m_opSource -> m_vHeight = m_psi.rcFrame.bottom;
		m_opSource -> m_vDepth = m_psi.dwSampleSize;
		m_opSource -> m_vBytesPerRow = m_opSource -> m_vDepth * m_opSource -> m_vWidth;
		m_opSource -> m_vBufferSizeBytes = m_psi.dwSuggestedBufferSize;

		m_opPreviewWindowSize = 0;
	
		m_Position = 0; //Start from the beginning

		m_vIsInitialized = true;
		return true;
	}

}

const char* EMWinVideoFile::GetDimensionString()
{
	return m_oDimensionString.c_str();
}

const char* EMWinVideoFile::GetFrameRateString()
{
	sprintf((char*)m_oFrameRateString.c_str(), "%.1f", m_vDwRate);

	return m_oFrameRateString.c_str();
	
}

void EMWinVideoFile::SetIAMConnected(bool p_vIAMConnected)
{
	m_vIAMConnected = p_vIAMConnected;
}

bool EMWinVideoFile::IAMConnected()
{
	return m_vIAMConnected;
}

/*bool EMWinVideoFile::LockVideo()
{
//	status_t vAcquireResult = acquire_sem(m_vVideoImageProcessingSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		return false;
	return true;
}
*/
/*void EMWinVideoFile::UnlockVideo()
{
//	release_sem(m_vVideoImageProcessingSemaphore);
}
*/
/*bool EMWinVideoFile::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_TIMER_SEEKED:
		//	m_vSeeked=true;
			break;
		case EM_SETTING_UPDATED:
		{
			eo << "EMWinVideoFile begin handle EM_SETTING_UPDATED" << ef;
			m_vTimeZoomScale = *(static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
			m_vWorkFPS = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
			m_vTimeZoomScaleWidth =  static_cast<int64>((1000000.0 / m_vWorkFPS) / m_vTimeZoomScale);
			eo << "EMWinVideoFile end handle EM_SETTING_UPDATED" << ef;
			break;
		}
		case EM_MESSAGE_TIMER_STOPPED:
			//m_vNeedToSyncWithTime = TRUE;
			break;
		case EM_MESSAGE_TIMER_STARTED:
			m_vNeedToSyncWithTime = TRUE;
			break;
		default:
			break;
	};
	
	return true;
}*/

int64 EMWinVideoFile::NumberOfFramesInFile()
{
	if(m_vIsInitialized)
	{
		return m_vTotalNumberOfFrames;
	} else
	{
		MessageBox(NULL, "Video-Clip was not initialized(proceeding anyways)", "EMWinVideoFile::NumberOfFramesInFile()", MB_OK);
		if(InitCheckE())
		{
			return m_vTotalNumberOfFrames;
		} else
		{
			EMDebugger("Couldn't Initialize EMWinVideoFile::NumberOfFramesInFile()"); 
		}
	}

	return 0;
}

int64* EMWinVideoFile::NumberOfFramesInWFPS(float p_vFramerateCurrentFPS)
{

	if(m_vIsInitialized)
	{
/*
		m_vMultiplier = (10000000 / m_opVideoHeader->AvgTimePerFrame) / p_vFramerateCurrentFPS;
		m_vNumberOfFramesInWFPS = static_cast<int64>(m_vTotalNumberOfFrames/m_vMultiplier)+1;*/

		float vDur = (GetDuration() / 1000000.0f) * m_vMagicSpeedValue;
		m_vNumberOfFramesInWFPS = static_cast<int64>(p_vFramerateCurrentFPS * vDur);

		return &m_vNumberOfFramesInWFPS;
	}
	else
		return 0;

}

void EMWinVideoFile::ReConnectGraph()
{
	HRESULT hr7 = m_opGraph -> ConnectDirect(m_pSourceOutPin, m_pSplitterInPin, NULL);
	PIN_DIRECTION pPinDirection = PINDIR_OUTPUT;
	m_pSplitterOutPin = GetPin(myIBaseFilter, pPinDirection);
	HRESULT hr8 = m_opGraph -> ConnectDirect(m_pSplitterOutPin, m_pDecoderInPin, NULL);
	HRESULT hr = m_opMediaControl->Run();
	SetIAMConnected(true);
}

void EMWinVideoFile::DisconnectGraph()
{
	
	HRESULT err = m_opIReadFrame->StopGraph(); 
	HRESULT hr3 = m_opGraph -> Disconnect(m_pDecoderInPin);
	HRESULT hr4 = m_opGraph -> Disconnect(m_pSplitterOutPin);
	m_pSplitterOutPin -> Release();
	HRESULT hr5 = m_opGraph -> Disconnect(m_pSplitterInPin);
	HRESULT hr6 = m_opGraph -> Disconnect(m_pSourceOutPin);
	SetIAMConnected(false);
}

void EMWinVideoFile::CloseAVIStream()
{
	if(m_pfrm != NULL)
	{
		try
		{
			AVIStreamGetFrameClose(m_pfrm);
		}
		catch(...)
		{
			eo << "There was an error trying to close media stream, probable cause (a bug in the codec)" << ef;
			eo << "Continuing anyways." << ef;
		}
		m_pfrm = NULL;
	}
}

HBITMAP EMWinVideoFile::ReadAndScaleData(int64 p_vTime, int16 p_vHeight)
{

	if(m_oQuickTimeVideo != NULL)
	{
		//It is a Quicktime file
		int64 seekTime = static_cast<int64>((float)p_vTime / m_vMagicSpeedValue); //Has to be done for quicktime seeks in milli-seconds

		long vSrows, vScols;
		vSrows = m_oQuickTimeVideo -> GetHeight();
		vScols = m_oQuickTimeVideo -> GetWidth();
		m_vDrows = p_vHeight;
		float yFactor=((float)m_vDrows)/(float)vSrows;		
		m_vDcols = (long)(ceil((float)vScols*(float)yFactor));
		int64 vImageSize = m_vDrows * m_vDcols * 3;

		m_opDestinationBitMap = m_oQuickTimeVideo->GetDIBAtTime(m_vDcols, p_vHeight, seekTime);
		
		//Temporarily write the image to preview window
/*		EMBitmap* m_opNewEMBeBitmapContainer;
		m_opNewEMBeBitmapContainer = EM_new EMWinBitmap(m_opDestinationBitMap);
		m_opVideoPreview -> Lock();
		m_opVideoPreview -> DrawBitmap(m_opNewEMBeBitmapContainer);//opEMBeBitmapContainer);
		m_opVideoPreview -> Unlock();
*/
		return m_opDestinationBitMap;

	}
	
	//it is AVI
	if(m_pfrm == NULL)
	{
		if ((m_pfrm=AVIStreamGetFrameOpen(m_pstm,NULL))==NULL)
			return 0;
	}

	void* pBitmapData = NULL;
	LPSTR pointerToDIBBits;
	LPSTR lpbi;
	int32 theFrame = static_cast<int32>((float)p_vTime / m_vMagicSpeedValue / (1000000.0 / m_vDwRate));

	if ((lpbi = (LPSTR)AVIStreamGetFrame(m_pfrm, theFrame))==NULL)
		return 0;
	m_pbmih = (LPBITMAPINFOHEADER)lpbi;
	pointerToDIBBits = lpbi + sizeof(BITMAPINFO)-1;// + ::PaletteSize(lpbi);

	if(p_vHeight != m_vTimeContinuousZoomScaleHeight)
	{
		long vSrows, vScols;
		vSrows = m_pbmih -> biHeight;
		vScols = m_pbmih -> biWidth;
		m_vDrows = p_vHeight;

		float yFactor=((float)m_vDrows)/(float)vSrows;
		
		m_vDcols = (long)(ceil((float)vScols*(float)yFactor));
			
	} 



	BITMAPINFO oBitmapInfoDestination;
	oBitmapInfoDestination.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	oBitmapInfoDestination.bmiHeader.biHeight = m_vDrows;
	oBitmapInfoDestination.bmiHeader.biWidth = m_vDcols;
	oBitmapInfoDestination.bmiHeader.biBitCount = 24; //m_pbmih -> biBitCount;
	oBitmapInfoDestination.bmiHeader.biSizeImage = m_vDrows * m_vDcols * 3; //(m_pbmih -> biBitCount / 8);
	oBitmapInfoDestination.bmiHeader.biCompression = 0;
	oBitmapInfoDestination.bmiHeader.biPlanes = 1;

	m_opDestinationBitMap = CreateDIBSection(NULL, &oBitmapInfoDestination, DIB_RGB_COLORS, &pBitmapData, NULL, 0);

	DIBSECTION oDibInfo;
	GetObject(m_opDestinationBitMap, sizeof(DIBSECTION), &oDibInfo);

	m_opSource -> m_vWidth = m_pbmih -> biWidth;
	m_opSource -> m_vHeight = m_pbmih -> biHeight;
	m_opSource -> m_vDepth = m_pbmih -> biBitCount / 8;
	m_opSource -> m_vBytesPerRow = m_pbmih -> biSizeImage / abs(m_pbmih -> biHeight);
	m_opSource -> m_vBufferSizeBytes = m_pbmih -> biSizeImage;

	m_opDestination -> m_vWidth = oDibInfo.dsBmih.biWidth; //m_vDcols;
	m_opDestination -> m_vHeight = oDibInfo.dsBmih.biHeight; //m_vDrows;
	m_opDestination -> m_vDepth = oDibInfo.dsBmih.biBitCount / 8; // 3
	m_opDestination -> m_vBytesPerRow = oDibInfo.dsBmih.biSizeImage / oDibInfo.dsBmih.biHeight;
	m_opDestination -> m_vBufferSizeBytes = oDibInfo.dsBmih.biSizeImage;

	m_oScale.InitScaleFunctions(m_opSource, m_opDestination);
//	if(m_vScale)
//	m_oScale.InitScaleFunctions(m_pbmih -> biWidth, m_pbmih -> biHeight, m_vDcols, m_vDrows, m_pbmih -> biBitCount/8, m_pbmih->biSizeImage/abs(m_pbmih->biHeight), oDibInfo.dsBm.bmWidthBytes);// m_opMediaDataBuffer -> m_oFormat.m_vBytesPerRow);

	m_oScale.Scale(pointerToDIBBits, pBitmapData);

	m_vTimeZoomScaleHeight = p_vHeight;
	m_vTimeContinuousZoomScaleWidth = m_vTimeZoomScaleWidth;
	m_vTimeContinuousZoomScaleHeight = p_vHeight;
	
	//Temporarily write the image to preview window
/*	EMBitmap* m_opNewEMBeBitmapContainer;
	m_opNewEMBeBitmapContainer = EM_new EMWinBitmap(m_opDestinationBitMap);
	m_opVideoPreview -> Lock();
	m_opVideoPreview -> DrawBitmap(m_opNewEMBeBitmapContainer);//opEMBeBitmapContainer);
	m_opVideoPreview -> Unlock();
*/
//	AVIStreamGetFrameClose(m_pfrm);
//	m_pfrm = NULL;
	return m_opDestinationBitMap;
}

int64 EMWinVideoFile::ReadData(EMMediaDataBuffer** p_opBufferPrimary, EMMediaDataBuffer** p_opBufferSecondary, int64 p_vMediaTimePosition, int64 p_vBufferOffset, bool p_vSeeking)
{
	if(m_oQuickTimeVideo != NULL)
	{
		//IS QuickTime
		int64 vPosition = static_cast<int64>((float)p_vMediaTimePosition / m_vMagicSpeedValue);
		m_oQuickTimeVideo->CopyFrameIntoBuffer(vPosition, *p_opBufferPrimary);
		(*p_opBufferPrimary) -> SetFrame(p_vMediaTimePosition + 1000000 / m_vWorkFPS);
		return 1;
	}

	//IS AVI
	if(m_pfrm == NULL)
	{
		if ((m_pfrm=AVIStreamGetFrameOpen(m_pstm,NULL))==NULL)
			return 0;
	}
	void* pBitmapData = NULL;
	LPSTR pointerToDIBBits;
	LPSTR lpbi;
	int32 theFrame = static_cast<int32>((float)p_vMediaTimePosition/m_vMagicSpeedValue / (1000000.0 / m_vDwRate));

	if ((lpbi = (LPSTR)AVIStreamGetFrame(m_pfrm, theFrame))==NULL)
		return 0;
	m_pbmih = (LPBITMAPINFOHEADER)lpbi;
	pointerToDIBBits = lpbi + sizeof(BITMAPINFO)-1;// + ::PaletteSize(lpbi);


	m_opSource -> m_vWidth = m_pbmih -> biWidth;
	m_opSource -> m_vHeight = m_pbmih -> biHeight;
	m_opSource -> m_vDepth = m_pbmih -> biBitCount / 8;
	m_opSource -> m_vBytesPerRow = m_pbmih -> biSizeImage / abs(m_pbmih -> biHeight);
	m_opSource -> m_vBufferSizeBytes = m_pbmih -> biSizeImage;

	m_oScale.InitScaleFunctions(m_opSource, &((*p_opBufferPrimary) -> m_oFormat));

//	m_oScale.InitScaleFunctions(m_pbmih -> biWidth, m_pbmih -> biHeight, (*p_opBufferPrimary) -> m_oFormat.m_vWidth, (*p_opBufferPrimary) -> m_oFormat.m_vHeight, m_pbmih -> biBitCount/8, m_pbmih->biSizeImage/abs(m_pbmih->biHeight), (*p_opBufferPrimary) -> m_oFormat.m_vBytesPerRow);

	m_oScale.Scale(pointerToDIBBits, (*p_opBufferPrimary) -> Data());

	(*p_opBufferPrimary) -> SetFrame(p_vMediaTimePosition + 1000000 / 25.0);

	//AVIStreamGetFrameClose(m_pfrm);
	//m_pfrm = NULL;
	return 1;

	//END OF NEW APPROACH
}

EMMediaFormat* EMWinVideoFile::ReadFormatE()
{
	return m_opFormat;
}


void EMWinVideoFile::SeekToFrame(int64 p_vToFrame)
{
	//NOT IN USE! (AT THE MOMENT ANYWAYS)
	//Seeking a specific frame is to be MUCH more accurate than below! Change when time is available
//	if(m_vIsInitialized)
//		m_opTrackOut->SeekToFrame(&p_vToFrame);
}

IPin* EMWinVideoFile::FindAndDiscardRenderer(bool p_vType)
{
	bool FoundRenderer = false;
	ULONG fetched;
	IBaseFilter* Renderer;
	IEnumFilters* EnumFilters;
	IPin* InPin;	
	IPin* OutPin;	
	IEnumPins* EnumPins;
	PIN_DIRECTION oPinDir;

	m_opGraph -> EnumFilters(&EnumFilters);
	EnumFilters -> Reset();
	while (!FoundRenderer && EnumFilters -> Next(1, &Renderer, &fetched) == S_OK)
	{
		Renderer -> EnumPins(&EnumPins);
		EnumPins -> Reset();

		while (EnumPins -> Next(1, &InPin, &fetched) == S_OK)
		{
			InPin -> QueryDirection(&oPinDir);

			if (oPinDir == PINDIR_OUTPUT)
			{
				break;
			}

			InPin -> Release();
			InPin = NULL;
		}

		if (InPin == NULL)
		{
			AM_MEDIA_TYPE pmt;

			EnumPins -> Reset();
			EnumPins->Next(1, &InPin, &fetched);

			HRESULT hr = InPin -> ConnectionMediaType(&pmt);

			if(hr == S_OK)
			{

				if((!p_vType && pmt.majortype == MEDIATYPE_Video) || (p_vType && pmt.majortype == MEDIATYPE_Audio))
				{
					FoundRenderer = true;
//					m_opType = pmt;
				}
				else
				{
					Renderer -> Release();
					Renderer = NULL;
				}

				FreeMediaType(pmt);
			}
		}
		else
		{
			Renderer -> Release();
			Renderer = NULL;
		}

		InPin -> Release();
		InPin = NULL;

		EnumPins -> Release();
		EnumPins = NULL;

	}	

	EnumFilters -> Release();	
	EnumFilters = NULL;

	if(FoundRenderer)
	{
		// Find renderer input
		Renderer -> EnumPins(&EnumPins);
		EnumPins -> Reset();
		EnumPins -> Next(1, &InPin, &fetched);	// first one is only one
		EnumPins -> Release();
		EnumPins = NULL;

		// Find ouput pin on filter it is connected to
		InPin -> ConnectedTo(&OutPin);

		// Disconnect the filters - note that we have to call Disconnect for both pins
		m_opGraph -> Disconnect(InPin);
		m_opGraph -> Disconnect(OutPin);

		InPin -> Release();
		InPin = NULL;

		HRESULT hr = m_opGraph -> RemoveFilter(Renderer);
		if(FAILED(hr)) 
		{
			MessageBox(NULL, "Couldn't remove render-filter from graph", "Message", MB_OK);
		}
		Renderer -> Release();
		Renderer = NULL;

		return OutPin;
	}

	return NULL;
}

IPin* EMWinVideoFile::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
    IEnumPins  *pEnum;
    IPin       *pPin;

    pFilter -> EnumPins(&pEnum);

    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);
        if(PinDir == PinDirThis)
		{
            break;
		}
        pPin->Release();
		pPin = NULL;
    }

    pEnum->Release();
	pEnum = NULL;

    return pPin;  
}

void EMWinVideoFile::SetOutputFormat(EMMediaFormat *p_opFormat)
{

}

#endif