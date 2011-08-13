#include "EMGlobals.h"
#include "EMMediaEngine.h"
#ifdef PLATFORM_WINDOWS //PLATFORM_BEOS

DEFINE_GUID(CLSID_EMReadFile, 0x36a5f770, 0xfe4c, 0x11ce, 0xa8, 0xed, 0x00, 0xaa, 0x00, 0x2f, 0xea, 0xb5);


#include "EMBeVideoFile.h"
#include "EMVideoFile.h"


#include "EMBeMediaUtility.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaEngine.h"
#include "EMMediaFormat.h"
#include "EMMediaIDManager.h"
#include "EMMediaTimer.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"

//#include <Bitmap.h>
//#include <TranslationUtils.h>

EMBeVideoFile::EMBeVideoFile(string p_oFileName)
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
		m_vTotalNumberOfFrames(0)
		//m_opTrackOut(NULL)

{
	m_opFormat = new EMMediaFormat(EM_TYPE_RAW_VIDEO);
	m_vTimeZoomScale = 0;//*(static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	m_vWorkFPS = 0;//*(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	m_vTimeZoomScaleWidth = 0;// static_cast<int64>((1000000.0 / m_vWorkFPS) / m_vTimeZoomScale);

}

EMBeVideoFile::~EMBeVideoFile()
{	
	EMMediaTimer::Instance() -> RemoveListener(this);
	EMMediaEngine::Instance() -> GetSettingsRepository() -> RemoveListener(this);


	if(m_opFormat != NULL)
		delete m_opFormat;

	if(m_opBufferContent != NULL)
		delete m_opBufferContent;

	if(m_opSourceBitMap != NULL)
		DeleteObject(m_opSourceBitMap);
//STOP AND DELETE FILTER GRAPH
//	m_opWinGrabber -> StopGraph();
	m_opGraph->Release();
    m_opMediaControl->Release();
    m_opEvent -> Release();
	m_opSeek -> Release();
//	m_opGrab -> Release();
	m_opIBaseFilterDumpRenderer->Release();
}

float* EMBeVideoFile::GetAspectRatio()
{
	
	return &vAspectRatio;
}

int64 EMBeVideoFile::GetBitmapAtTimePosition(int64* p_vpToTime, void* p_opBitmap)
{
//	m_opITestFilterDumpDumpDumpHeader -> GrabFrame(p_opBitmap, m_vStart_Time);//, m_oBitmapInfoTrash);
//	return 0;
//	LockVideo();
	HRESULT err = S_FALSE;//B_ERROR;
	int64 vTempStartTime;
	int64 vToTime = *p_vpToTime;
	if(m_vIsInitialized)
	{
		//err = m_opTrackOut -> SeekToTime(p_vpToTime, B_MEDIA_SEEK_CLOSEST_BACKWARD);
		m_opITestFilterDumpDumpDumpHeader ->SeekToTime(*p_vpToTime);
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
		//err = m_opTrackOut -> ReadFrames(m_opBufferSecondary -> Data(), &framecount, &m_opMediaHeader);
				//Get the next frame
				m_opITestFilterDumpDumpDumpHeader -> GrabFrame(m_opBufferSecondary -> Data(), m_vStart_Time);//, m_oBitmapInfoTrash);
	}
	else
	{
		//err = m_opTrackOut -> ReadFrames(p_opBitmap, &framecount, &m_opMediaHeader);
				//Get the next frame
				m_opITestFilterDumpDumpDumpHeader -> GrabFrame(p_opBitmap, m_vStart_Time);//, m_oBitmapInfoTrash);
	}
	vTempStartTime = m_vStart_Time;//m_opMediaHeader.start_time;


/*	if(err != B_OK)
	{
		m_opITestFilterDumpDumpDumpHeader -> SeekToTime(*p_vpToTime);

		m_opITestFilterDumpDumpDumpHeader -> GrabFrame(p_opBitmap, m_vStart_Time);//, m_oBitmapInfoTrash);
		
	}
*/

	if(m_vStart_Time == vToTime && m_opBufferSecondary != NULL)
	{
		memcpy(p_opBitmap, m_opBufferSecondary -> Data(), GetSize());
		//err = m_opTrackOut -> ReadFrames(m_opBufferSecondary -> Data(), &framecount, &m_opMediaHeader);
		m_opITestFilterDumpDumpDumpHeader -> GrabFrame(m_opBufferSecondary -> Data(), m_vStart_Time);//, m_oBitmapInfoTrash);
		*p_vpToTime = m_vStart_Time;//m_opMediaHeader.start_time;
		//UnlockVideo();
		return vTempStartTime;
	}

	while(m_vStart_Time < vToTime)
	{
		vTempStartTime = m_vStart_Time;

		if(m_opBufferSecondary != NULL)
		{
			memcpy(p_opBitmap, m_opBufferSecondary -> Data(), GetSize());
			//err = m_opTrackOut -> ReadFrames(m_opBufferSecondary -> Data(), &framecount, &m_opMediaHeader);
			m_opITestFilterDumpDumpDumpHeader -> GrabFrame(m_opBufferSecondary -> Data(), m_vStart_Time);//, m_oBitmapInfoTrash);
		}
		else
		{
			memcpy(m_opBufferContent, p_opBitmap, GetSize());
			//err = m_opTrackOut -> ReadFrames(p_opBitmap, &framecount, &m_opMediaHeader);
			m_opITestFilterDumpDumpDumpHeader -> GrabFrame(p_opBitmap, m_vStart_Time);//, m_oBitmapInfoTrash);
		}
		/*if(err != B_OK)
		{
			emerr << "ERROR:" << strerror(err) <<endl;
			EMDebugger("ERROR! Went out of bounds in EMBeVideoFile::SeekToTime()");
		}
		*/
	}

	if(m_vStart_Time > vToTime)
	{
		*p_vpToTime = m_vStart_Time;//m_opMediaHeader.start_time;
		//UnlockVideo();
		return vTempStartTime;
	}
	
	int64 vRealStartTime = m_vStart_Time;//m_opMediaHeader.start_time;
	//err = m_opTrackOut -> ReadFrames(m_opBufferContent, &framecount, &m_opMediaHeader);
	m_opITestFilterDumpDumpDumpHeader -> GrabFrame(m_opBufferContent, m_vStart_Time);//, m_oBitmapInfoTrash);
	*p_vpToTime = m_vStart_Time;	
//	UnlockVideo();

	return vRealStartTime;
}

int64 EMBeVideoFile::GetDuration()
{
	//Counting microseconds is to be MUCH more accurate than below! Change when time is available
	if(m_vIsInitialized)
	{
		
		return m_vTotalDuration;
	}

	return 0;
}

EMMediaFormat* EMBeVideoFile::GetFormat()
{
		return m_opFormat;
}

/*int32 EMBeVideoFile::GetID() const
{
	return m_vID;
}
*/
int64 EMBeVideoFile::GetSize() const
{
	if(m_vIsInitialized)
	{
		return m_opVideoHeader->bmiHeader.biSizeImage;//biHeight*m_pVideoHeader->bmiHeader.biWidth*(m_pVideoHeader -> bmiHeader.biBitCount/8);
	}

	return 0;
}

bool EMBeVideoFile::InitCheckE()
{
	MessageBox(NULL, "Going into initcheck", "Message for Martin", MB_OK);

	IPin* pDumpDumpDumpInPin;
	IPin* pDecompressorOutPin;
    HRESULT hr;
	AM_MEDIA_TYPE	mt;
	int64 pEarliest;

	//******************************************
	//Create the acctual filter graph
	//
	//
    
    // Create the filter graph manager
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_opGraph);

	//Query some basic interfaces
    m_opGraph -> QueryInterface(IID_IMediaEvent, (void **)&m_opEvent);
	m_opGraph -> QueryInterface(IID_IMediaControl, (void **)&m_opMediaControl);
    m_opGraph -> QueryInterface(IID_IMediaSeeking, (void **)&m_opSeek);


	//Create the whole Filter Graph
    hr = m_opGraph -> RenderFile(L"C:\\Finale.avi", NULL);
	if(hr != S_OK)
		MessageBox(NULL, "Couldn't render Graph", "Message for Martin", MB_OK);

	//
	//
	// Creating the acctual filter graph ends here
	//******************************************

	//******************************************
	//Transform the filter graph and put our own renderer in the end
	//
	//
	//Disconnect normal renderer and put ours in
	pDecompressorOutPin = FindAndDiscardRenderer(false); // false = Video , If there is any audio available save the pre renderer ouputPin for us to use
	FindAndDiscardRenderer(true); //Discard audio (true = Audio) We don't wannt the audio

	if(pDecompressorOutPin != NULL) //Was there any video available at all in this clip??
	{
		//Get an instance of our renderer
		CoCreateInstance(CLSID_EMReadFrame, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&m_opIBaseFilterDumpRenderer);
		m_opGraph->AddFilter(m_opIBaseFilterDumpRenderer, L"DumpDumpDump");
		m_opIBaseFilterDumpRenderer -> QueryInterface(IID_IReadFrame, (void **)&m_opITestFilterDumpDumpDumpHeader);

		//Get the input pin of our renderer
		PIN_DIRECTION pPinDirection = PINDIR_INPUT;
		pDumpDumpDumpInPin = GetPin(m_opIBaseFilterDumpRenderer, pPinDirection);

		//Connect: Decoder_OUTPIN -> DumpRenderer_INPIN
		hr = m_opGraph ->Connect(pDecompressorOutPin, pDumpDumpDumpInPin);
		if(hr==S_OK)
			MessageBox(NULL, "Connected Decoder_OUTPIN -> SampleGrabber_INPIN", "Connection Message", MB_OK);
		else
			MessageBox(NULL, "Failed Connecting Decoder_OUTPIN -> WaveDest_INPIN", "Connection Message", MB_OK);

	} else
	{
		MessageBox(NULL, "Couldn't find any video in this clip!?", "Message", MB_OK);
	}
	
	//Save the connectionformat (RGB_24 as for now, will be dynamical later on.. FIX!!)
	pDumpDumpDumpInPin->ConnectionMediaType(&m_opType);

	//Initialize our renderer
	m_opITestFilterDumpDumpDumpHeader->InitializeRenderer(&m_opType, m_opSeek, m_opMediaControl);

	//
	//
	// Transformation ends here
	//******************************************

	//Get Total number of frames in video file
	m_opSeek->SetTimeFormat(&TIME_FORMAT_FRAME);
	m_opSeek->GetAvailable( &pEarliest, &m_vTotalNumberOfFrames);

	//Get Total duration of the video file in nanoSeconds
	m_opSeek->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	m_opSeek->GetAvailable( &pEarliest, &m_vTotalDuration);
	m_vTotalDuration = static_cast<int64>(m_vTotalDuration / 10); //Have to make it microseconds (Because Rille says so, and because it should comply to BeOS)

	// Get a pointer to the video header.
	m_opVideoHeader = (VIDEOINFOHEADER*)m_opType.pbFormat;


	m_opFormat->CreateVideoFormat(&m_opType);

	vAspectRatio = (static_cast<float>(m_opVideoHeader->bmiHeader.biWidth)/static_cast<float>(m_opVideoHeader->bmiHeader.biHeight));
	m_opBufferContent = new unsigned char[m_opVideoHeader->bmiHeader.biSizeImage];

	m_Position = 0; //Start from the beginning
		
		
	MessageBox(NULL, "Going to run graph", "Message for Martin", MB_OK);
		
	hr = m_opMediaControl->Run();
	m_vIsInitialized = true;
	MessageBox(NULL, "Done with initcheck", "Message for Martin", MB_OK);

	return true;
}

/*bool EMBeVideoFile::LockVideo()
{
//	status_t vAcquireResult = acquire_sem(m_vVideoImageProcessingSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		return false;
	return true;
}
*/
/*void EMBeVideoFile::UnlockVideo()
{
//	release_sem(m_vVideoImageProcessingSemaphore);
}
*/
bool EMBeVideoFile::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_TIME_SEEKED:
			m_vSeeked=true;
			break;
		case EM_MESSAGE_STOP:
			break;
		case EM_SETTING_UPDATED:
			m_vTimeZoomScale = *(static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
			m_vWorkFPS = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
			m_vTimeZoomScaleWidth =  static_cast<int64>((1000000.0 / m_vWorkFPS) / m_vTimeZoomScale);
			break;
		default:
			break;
	};
	
	return true;
}

int64 EMBeVideoFile::NumberOfFramesInFile()
{
	if(m_vIsInitialized)
	{
		return m_vTotalNumberOfFrames;
	}

	return 0;
}

int64* EMBeVideoFile::NumberOfFramesInWFPS(float p_vFramerateCurrentFPS)
{

	if(m_vIsInitialized)
	{

		m_vMultiplier = (10000000 / m_opVideoHeader->AvgTimePerFrame) / p_vFramerateCurrentFPS;
		m_vNumberOfFramesInWFPS = static_cast<int64>(m_vTotalNumberOfFrames/m_vMultiplier)+1;
		return &m_vNumberOfFramesInWFPS;
	}
	else
		return 0;

}

/*BBitmap* EMBeVideoFile::ReadAndScaleData(int64 p_vTime, int16 p_vHeight)
{

		if(p_vHeight != m_vTimeContinuousZoomScaleHeight)
		{
			ResetScaling(0, p_vHeight);
		} else
		{
			m_opDestinationBitMap = new BBitmap(BRect(0,0, m_vDcols-1, m_vDrows-1), m_sFormat.ColorSpace(), false, false);
			//return m_opDestinationBitMap;
		}
		if(m_vOnlyOnce)
		{	
			if(m_opSourceBitMap != NULL)
				delete m_opSourceBitMap;
			m_opSourceBitMap = new BBitmap(BRect(0,0, m_sFormat.Width()-1, m_sFormat.Height()-1), m_sFormat.ColorSpace(), false, false);
			m_vOnlyOnce = false;
		}
		GetBitmapAtTimePosition(&p_vTime, m_opSourceBitMap -> Bits());

		Scale(m_opSourceBitMap -> Bits(), m_opDestinationBitMap -> Bits());
	
		m_vTimeZoomScaleHeight = p_vHeight;

		m_vTimeContinuousZoomScaleWidth = m_vTimeZoomScaleWidth;
		m_vTimeContinuousZoomScaleHeight = p_vHeight;
		return m_opDestinationBitMap;

}*/
HBITMAP EMBeVideoFile::ReadAndScaleData(int64 p_vTime, int16 p_vHeight)
{
		if(p_vHeight != m_vTimeContinuousZoomScaleHeight)
		{
			ResetScaling(0, p_vHeight);
		} else
		{
			//m_opDestinationBitMap = new BBitmap(BRect(0,0, m_vDcols-1, m_vDrows-1), m_sFormat.ColorSpace(), false, false);
			void* pBitmapData;
			BITMAPINFO oBitmapInfoDestination;
			oBitmapInfoDestination.bmiHeader.biHeight = m_vDrows;
			oBitmapInfoDestination.bmiHeader.biWidth = m_vDcols;
			oBitmapInfoDestination.bmiHeader.biBitCount = (m_opFormat->m_vDepth*8);
			oBitmapInfoDestination.bmiHeader.biSizeImage = m_vDrows * m_vDcols * m_opFormat->m_vDepth;
			oBitmapInfoDestination.bmiHeader.biCompression = 0;
			oBitmapInfoDestination.bmiHeader.biPlanes = 1;

			m_opDestinationBitMap = CreateDIBSection(NULL, &oBitmapInfoDestination, DIB_RGB_COLORS, &pBitmapData, NULL, NULL);
			//return m_opDestinationBitMap;
		}
		if(m_vOnlyOnce)
		{	
/*			if(m_opSourceBitMap != NULL)
				DeleteObject( m_opSourceBitMap );
			//m_opSourceBitMap = new BBitmap(BRect(0,0, m_sFormat.Width()-1, m_sFormat.Height()-1), m_sFormat.ColorSpace(), false, false);
			ZeroMemory(&oBitmapInfoSource, sizeof(oBitmapInfoSource));
			oBitmapInfoSource.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			oBitmapInfoSource.bmiHeader.biHeight = m_pVideoHeader->bmiHeader.biHeight;//m_opFormat -> m_vHeight;
			oBitmapInfoSource.bmiHeader.biWidth = m_pVideoHeader->bmiHeader.biWidth;//m_opFormat -> m_vWidth;
			oBitmapInfoSource.bmiHeader.biBitCount = m_pVideoHeader->bmiHeader.biBitCount;//(m_opFormat->m_vDepth*8);
			oBitmapInfoSource.bmiHeader.biSizeImage = m_pVideoHeader->bmiHeader.biHeight * m_pVideoHeader->bmiHeader.biWidth * (m_pVideoHeader->bmiHeader.biBitCount/8);
			oBitmapInfoSource.bmiHeader.biCompression = BI_RGB;
			oBitmapInfoSource.bmiHeader.biPlanes = 1;

			m_opSourceBitMap = CreateDIBSection(0, &oBitmapInfoSource, DIB_RGB_COLORS, &pBitmapData, NULL, 0);
			
					BITMAPINFO sBitmapInfo;
					sBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					sBitmapInfo.bmiHeader.biWidth = 120;//vDimensionX + 1;
					sBitmapInfo.bmiHeader.biHeight = 240;//vDimensionY + 1;
					sBitmapInfo.bmiHeader.biPlanes = 1;
					sBitmapInfo.bmiHeader.biBitCount = 32; // FIXME: Perhaps this shouldn't be hardcoded to 32 bits/pixel?
					sBitmapInfo.bmiHeader.biCompression = BI_RGB;
					void* upBitmapBits;

					m_opSourceBitMap = CreateDIBSection(NULL, &sBitmapInfo, DIB_RGB_COLORS, &upBitmapBits, NULL, NULL);



			// The video header contains the bitmap information. 
			// Copy it into a BITMAPINFO structure.
			BITMAPINFO BitmapInfo;
			ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
			CopyMemory(&BitmapInfo.bmiHeader, &(m_pVideoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));
			BitmapInfo.bmiHeader.biCompression = 0;

			// Create a DIB from the bitmap header, and get a pointer to the buffer.
			HBITMAP m_opSourceBitMap = CreateDIBSection(0, &BitmapInfo, DIB_RGB_COLORS, &pBitmapData, NULL, 0);
*/
			m_vOnlyOnce = false;
			
		}

		DIBSECTION oDibsectionDestination;
		GetObject(m_opDestinationBitMap, sizeof(DIBSECTION), &oDibsectionDestination);

//		DIBSECTION oDibsectionSource;
//		GetObject(m_oBitmapInfoTrash, sizeof(DIBSECTION), &oDibsectionSource);

		// Get a pointer to the video header.
		VIDEOINFOHEADER *pVideoHeader = m_opVideoHeader;//(VIDEOINFOHEADER*)m_opMediaType->pbFormat;

		// The video header contains the bitmap information. 
		// Copy it into a BITMAPINFO structure.
		BITMAPINFO BitmapInfo;
		ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
		CopyMemory(&BitmapInfo.bmiHeader, &(pVideoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));

		void* m_opBuffer = NULL;
		// Create a DIB from the bitmap header, and get a pointer to the buffer.
		m_oBitmapInfoTrash = CreateDIBSection(0, &BitmapInfo, DIB_RGB_COLORS, &m_opBuffer, NULL, 0);	

		//GetBitmapAtTimePosition(&p_vTime, oDibsectionSource.dsBm.bmBits);
		GetBitmapAtTimePosition(&p_vTime, m_opBuffer);

		DIBSECTION oDibsectionSource;
		GetObject(m_oBitmapInfoTrash, sizeof(DIBSECTION), &oDibsectionSource);

	
//		memset(oDibsectionSource.dsBm.bmBits/*pBitmapData*/, 255, oBitmapInfoSource.bmiHeader.biSizeImage);
//		return m_oBitmapInfoTrash;//m_opSourceBitMap;

		Scale(oDibsectionSource.dsBm.bmBits, oDibsectionDestination.dsBm.bmBits);
		//scale2(oDibsectionSource.dsBm.bmBits, oDibsectionDestination.dsBm.bmBits);
		m_vTimeZoomScaleHeight = p_vHeight;

		m_vTimeContinuousZoomScaleWidth = m_vTimeZoomScaleWidth;
		m_vTimeContinuousZoomScaleHeight = p_vHeight;
		MessageBox(NULL, "When you push this button you should get the EMBitmap object !:)!", "Message for Martin", MB_OK);

		return m_opDestinationBitMap;

}


/*BBitmap* EMBeVideoFile::ReadAndScaleDataKeyFrames(int64 p_vTime, int16 p_vHeight)
{

		media_header opmh;
		int64 framecount;
		
		if(p_vHeight != m_vTimeContinuousZoomScaleHeight)
		{
			ResetScaling(0, p_vHeight);
		} else
			m_opDestinationBitMap = new BBitmap(BRect(0,0, m_vDcols-1, m_vDrows-1), m_sFormat.ColorSpace(), false, false);

		ResetScaling(0, p_vHeight);
		
		if(m_vOnlyOnce)
		{	
			if(m_opSourceBitMap != NULL)
				delete m_opSourceBitMap;
			m_opSourceBitMap = new BBitmap(BRect(0,0, m_sFormat.Width()-1, m_sFormat.Height()-1), m_sFormat.ColorSpace(), false, false);
			m_vOnlyOnce = false;
		}

		status_t err = m_opTrackOut -> SeekToTime(&p_vTime);
		if( err != B_OK)
			EMDebugger("Wrong in SeekToTime EMBeVideoFile::ReadAndScaleDataKeyFrames");
			
		err = m_opTrackOut -> ReadFrames(m_opSourceBitMap -> Bits(), &framecount, &opmh);
		if( err != B_OK)
			EMDebugger("Wrong in ReadFrames EMBeVideoFile::ReadAndScaleDataKeyFrames");
		
		Scale(m_opSourceBitMap -> Bits(), m_opDestinationBitMap -> Bits());
	
		m_vTimeZoomScaleHeight = p_vHeight;

		m_vTimeContinuousZoomScaleWidth = m_vTimeZoomScaleWidth;
		m_vTimeContinuousZoomScaleHeight = p_vHeight;
		return m_opDestinationBitMap;

}
*/


int64 EMBeVideoFile::ReadData(EMMediaDataBuffer** p_opBufferPrimary, EMMediaDataBuffer** p_opBufferSecondary, int64 p_vMediaTimePosition, int64 p_vBufferOffset, bool p_vSeeking)
{

    //HRESULT hr;
	//int64 framecount;
	if(p_vSeeking)
	{
		
		m_vSeeked=true;
		if(*p_opBufferSecondary != NULL)
		{
			m_opBufferSecondary = *p_opBufferSecondary;
			(*p_opBufferPrimary) -> SetFrame(GetBitmapAtTimePosition(&p_vMediaTimePosition, (*p_opBufferPrimary) -> Data()));
			m_opBufferSecondary -> SetFrame(p_vMediaTimePosition);
			//TODO: What happens if seeking is made before even having played once?
		} 
		else
		{
			(*p_opBufferPrimary) -> SetFrame(GetBitmapAtTimePosition(&p_vMediaTimePosition, (*p_opBufferPrimary) -> Data()));
		}
		return 1;
	}
	
	if(m_vSeeked == true)
	{
		if(*p_opBufferSecondary == NULL)
		{
			
			(*p_opBufferPrimary) -> SetFrame(GetBitmapAtTimePosition(&p_vMediaTimePosition, (*p_opBufferPrimary) -> Data()));
			*p_opBufferSecondary = 	m_opBufferSecondary;	
			(*p_opBufferSecondary) -> SetFrame(m_vStart_Time);
			m_vSeeked = false;
			return 1;
		}
			m_vSeeked = false;
	}
	
	if(*p_opBufferSecondary != NULL)
	{
		m_opBufferSecondary = *p_opBufferSecondary;

		(*p_opBufferPrimary) -> SetFrame(GetBitmapAtTimePosition(&p_vMediaTimePosition, (*p_opBufferPrimary) -> Data()));
		m_opBufferSecondary -> SetFrame(p_vMediaTimePosition);
	} 
	else
	{
		if(m_opBufferSecondary -> GetFrame() <= p_vMediaTimePosition)
		{
			m_opBufferSwapping = *p_opBufferPrimary;
			*p_opBufferPrimary = m_opBufferSecondary;
			m_opBufferSecondary = m_opBufferSwapping;
			m_opBufferSwapping = NULL;
			memcpy(m_opBufferSecondary -> Data(), (*p_opBufferPrimary) -> Data(), GetSize());			
			do
			{
				//Get the next frame
				//m_opWinGrabber->GrabFrame(m_opBufferSecondary -> Data(), m_vStart_Time);
				m_opITestFilterDumpDumpDumpHeader -> GrabFrame(m_opBufferSecondary -> Data(), m_vStart_Time);//, m_oBitmapInfoTrash);
				//Save start time to the EMMedia Databuffer
				m_opBufferSecondary -> SetFrame(m_vStart_Time);
			} 
			while(m_vStart_Time <= p_vMediaTimePosition);
		} 
	}

	return 1;
}

EMMediaFormat* EMBeVideoFile::ReadFormatE()
{
	return m_opFormat;
}

void EMBeVideoFile::ResetScaling(const long width, const long height)
{

	long srows, scols;
	float xFactor, yFactor = 0;
	float x_f;
	float xfac_inv;
	//Delete old
	if(m_opR != NULL)
	{
		for(int32 vIndex = 0; vIndex < m_vDcols; vIndex++)
		{
			delete m_opR[vIndex];
		}
		delete [] m_opR;
	}
	//End delete old
	srows = m_opVideoHeader->bmiHeader.biHeight;//m_opFormat -> m_vHeight-1;//m_sFormat.Height()-1;
	scols = m_opVideoHeader->bmiHeader.biWidth;//m_opFormat ->m_vWidth;//m_sFormat.Width();
	m_vDcols = width;
	m_vDrows = height;
	if(height != 0)
		yFactor=((float)m_vDrows)/(float)srows;
		
	//We don't wanne decide the width our selves (YES WE DO WANNT THAT, now anyways)
	//But have it scaled in proportion to the hight
	if(width != 0)
		xFactor=(float)m_vDcols/(float)scols; //This row is non proportinal
		
	if(width == 0)
	{
		m_vDcols = (long)(ceil((float)scols*(float)yFactor));
		xFactor=(float)m_vDcols/(float)scols;
	}
	else
		if(height == 0)
		{
			m_vDrows = (long)(ceil((float)srows*(float)xFactor));
			yFactor=(float)m_vDrows/(float)srows;
		
		}
		else
			EMDebugger("ERROR IN ResetScaling");

	//m_opDestinationBitMap = new BBitmap(BRect(0,0, m_vDcols-1, m_vDrows-1), m_sFormat.ColorSpace(), false, false);
	void* pBitmapData;
	BITMAPINFO oBitmapInfoDestination;
	ZeroMemory(&oBitmapInfoDestination, sizeof(oBitmapInfoDestination));
	oBitmapInfoDestination.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	oBitmapInfoDestination.bmiHeader.biWidth = m_vDcols;
	oBitmapInfoDestination.bmiHeader.biHeight = m_vDrows;
	oBitmapInfoDestination.bmiHeader.biBitCount = (m_opVideoHeader->bmiHeader.biBitCount);
	oBitmapInfoDestination.bmiHeader.biSizeImage = m_vDrows * m_vDcols * (oBitmapInfoDestination.bmiHeader.biBitCount/8);
	oBitmapInfoDestination.bmiHeader.biCompression = 0;
	oBitmapInfoDestination.bmiHeader.biPlanes = m_opVideoHeader->bmiHeader.biPlanes;

	m_opDestinationBitMap = CreateDIBSection(NULL, &oBitmapInfoDestination, DIB_RGB_COLORS, &pBitmapData, NULL, NULL);

	m_vxfac = xFactor;
	m_vyfac = yFactor;	


	// Allocate buffer for storing the values of ma and m1
	m_opR = new EMRowValues* [m_vDcols];
	m_opRptr = m_opR;
	for(int32 vIndex = 0; vIndex < m_vDcols; vIndex++)
	{
		m_opR[vIndex]	= new EMRowValues();
	}
	xfac_inv = 1.0 / m_vxfac;
	
	// Fill up the buffer once, to be used for each row
	for (long i=0; i<m_vDcols; i++)
	{
		x_f = i*xfac_inv;
		m_opRptr[i]->x_i = (long)x_f;
		m_opRptr[i]->p = x_f - (float) m_opRptr[i]->x_i;
		m_opRptr[i]->p1 = 1.0 - m_opRptr[i]->p;
	}

}


void EMBeVideoFile::Scale(void *src, void *dest)
{

	unsigned long *spixptr, *spix1, *spix2;
	unsigned char *dpixptr, *dpix;
	unsigned long slb, dlb;
	long i, j; //Register until here
	float p, q, p1, q1;
	float src_y_f;
	long src_y_i, src_x_i;
	unsigned char *a, *b, *c, *d; //was also registered
	float rred, rgreen, rblue, other;
	float yfac_inv;
	float transparency;
			
	yfac_inv = 1.0 / m_vyfac;

	dpixptr = (unsigned char *)dest;//Bits();
	dpix = (unsigned char *)dpixptr;
	spixptr = (unsigned long *)src;//Bits();
	//slb = m_sFormat.u.raw_video.display.bytes_per_row/4;
	slb = (m_opVideoHeader->bmiHeader.biWidth*(m_opVideoHeader -> bmiHeader.biBitCount/8))/4;
	DIBSECTION oDibsectionDestination;
	GetObject(m_opDestinationBitMap, sizeof(DIBSECTION), &oDibsectionDestination);
	dlb = oDibsectionDestination.dsBm.bmWidth*(m_opVideoHeader -> bmiHeader.biBitCount/8);
	
	// Perform the scaling by inverse mapping from dest to source
	// That is, for each point in the destination, find the 
	// corresponding point in the source.
	// Adding +1 to m_vDrows because the demands from the GUI
	// is actually a value not counting zero as a pixel
	//If we don't fo this there will be a white line at the bottom
	for (i=0; i<m_vDrows-1; i++)
	{
		src_y_f = i * yfac_inv;
		src_y_i = (long) src_y_f;
		
		q = src_y_f - (float)src_y_i;
		q1 = 1.0 - q;
		spix1 = spixptr + src_y_i*slb;		//Hamnar på första pixeln i raden, slb=antalet bytes per rad
		spix2 = spixptr + (src_y_i+1)*slb; //src_y_i=raden, hamnar på sista pixeln i raden

		m_opRptr = m_opR;
		transparency = 0.0;

		//char myNumber[20];
		//_itoa(i, myNumber, 10);
		//MessageBox(NULL, "For NR", myNumber, MB_OK);

		for (j =0; j<m_vDcols; j++)
		{
			src_x_i = m_opRptr[j] -> x_i;
			p = m_opRptr[j] -> p;
			p1 = m_opRptr[j] -> p1;

			a = (unsigned char *)(spix1 + src_x_i);
			b = (unsigned char *)(spix1 + src_x_i + 1);
			c = (unsigned char *)(spix2 + src_x_i);
			d = (unsigned char *)(spix2 + src_x_i + 1);
			
			// Compute the interpolated pixel value
			rblue  = (((float)a[0]*p1 + (float)b[0]*p)*q1 + ((float)c[0]*p1 + (float)d[0]*p)*q);
			rgreen = (((float)a[1]*p1 + (float)b[1]*p)*q1 + ((float)c[1]*p1 + (float)d[1]*p)*q);
			rred   = (((float)a[2]*p1 + (float)b[2]*p)*q1 + ((float)c[2]*p1 + (float)d[2]*p)*q);
			//other  = (((float)a[3]*p1 + (float)b[3]*p)*q1 + ((float)c[3]*p1 + (float)d[3]*p)*q);
				
			dpix[0] = (unsigned char) (rblue * (1.0 - transparency));// + (rblue2 * transparency));
			dpix[1] = (unsigned char) (rgreen * (1.0 - transparency));// + (rgreen2 * transparency));
			dpix[2] = (unsigned char) (rred * (1.0 - transparency));// + (rred2 * transparency));
			//dpix[3] = (unsigned char) (other * (1.0 - transparency));// + (rred2 * transparency));
			dpix += 4;
		}		
		
		// Advance to the next destination line
		dpix = (dpixptr += dlb);
	}

}

void EMBeVideoFile::scale2 (void * imgIn, void * imgOut)//, double xscale, double yscale)
{
  int x, y;
  int ddax, dday, xzoom, yzoom, i, j, k, yline;
  unsigned char **image_in, **image_out;  /* input/output image */

  image_in = (unsigned char **)imgIn;
  image_out = (unsigned char **)imgOut;


  /* Calculate the differential amount */
  xzoom = (int) (1.0 / m_vxfac * 1000);
  yzoom = (int) (1.0 / m_vyfac * 1000);
  /* Initialize the output Y value and vertial differential */
  y = 0;
  dday = 0;
  /* Loop over rows in the original image */
  for (i = 0; i < m_opVideoHeader->bmiHeader.biHeight; i++) {
    /* Adjust the vertical accumulated differential, initialize the
     * output X pixel and horizontal accumulated differential */
    int total = i*j;
	dday -= 1000;
    x = 0;
    ddax = 0;
    /* Loop over pixels in the original image */
    for (j = 0; j < m_opVideoHeader->bmiHeader.biWidth; j++) {
      /* Adjust the horizontal accumulated differential */
      ddax -= 1000;
      while (ddax < 0) {
        /* Store values from original image scanline into the scaled
         * buffer until accumulated differential crosses threshold */
        image_out[y][x] = (unsigned char) image_in[j][i];
        x++;
        ddax += xzoom;
      }
    }
    yline = y;
    while (dday < 0) {
      /* The 'outer loop' -- output resized scan lines until the
       * vertical threshold is crossed */
      dday += yzoom;
      for (k = 0; k < x; k++) {
        image_out[y][k] = (unsigned char) image_out[yline][k];
      }
      y++;
      if (y >= m_vDrows)
        y = m_vDrows - 1;
    }
  }
}

void EMBeVideoFile::SeekToFrame(int64 p_vToFrame)
{
	//NOT IN USE! (AT THE MOMENT ANYWAYS)
	//Seeking a specific frame is to be MUCH more accurate than below! Change when time is available
//	if(m_vIsInitialized)
//		m_opTrackOut->SeekToFrame(&p_vToFrame);
}

IPin* EMBeVideoFile::FindAndDiscardRenderer(bool p_vType)
{
	bool FoundRenderer = false;
	ULONG fetched;
	IBaseFilter* Renderer;
	IEnumFilters* EnumFilters;
	IPin* InPin;	// renderer input
	IPin* OutPin;	// decoder or other filter output;
	PIN_INFO pinfo;
	int numoutputpins = 0;
	IEnumPins* EnumPins;

	m_opGraph->EnumFilters(&EnumFilters);
	EnumFilters->Reset();
	do
	{
		EnumFilters->Next(1, &Renderer, &fetched);	// get next filter
		Renderer->EnumPins(&EnumPins);
		EnumPins->Reset();
		numoutputpins = 0;
		while (EnumPins->Next(1, &InPin, &fetched) == S_OK)
		{
			InPin->QueryPinInfo(&pinfo);
			pinfo.pFilter->Release();
			InPin->Release();
			if (pinfo.dir == PINDIR_OUTPUT)
			{
					numoutputpins++;
				break;		// we can jump out if we found an output pin
			}
			
		}

		EnumPins->Release();
		if (numoutputpins == 0)
		{
				//Is this a video or an audio renderer
			AM_MEDIA_TYPE pmt;

			HRESULT hr = InPin->ConnectionMediaType(&pmt);

			if(!p_vType && pmt.majortype == MEDIATYPE_Video)
			{
				MessageBox(NULL, "VIDEO", "MESSAGE", MB_OK);
				FoundRenderer = true;
				m_opType = pmt;
			}
			else if(p_vType && pmt.majortype == MEDIATYPE_Audio)
			{
				MessageBox(NULL, "AUDIO", "MESSAGE", MB_OK);
					FoundRenderer = true;
			}

			if(pmt.majortype == MEDIATYPE_Video)
				MessageBox(NULL, "DEFENETELY VIDEO", "MESSAGE", MB_OK);
			else if(pmt.majortype == MEDIATYPE_Audio)
				MessageBox(NULL, "DEFENETELY AUDIO", "MESSAGE", MB_OK);
			
		}
		else
			Renderer->Release();
	}	while ( FoundRenderer == false && numoutputpins == 0);

	EnumFilters->Release();	

	if(FoundRenderer)
	{
		// Find renderer input
		Renderer->EnumPins(&EnumPins);
		EnumPins->Reset();
		EnumPins->Next(1, &InPin, &fetched);	// first one is only one
		EnumPins->Release();

		// Find ouput pin on filter it is connected to
		InPin->ConnectedTo(&OutPin);

		// Disconnect the filters - note that we have to call Disconnect for both pins
		m_opGraph->Disconnect(InPin);
		m_opGraph->Disconnect(OutPin);

		PIN_INFO oPinInfo;
		HRESULT hr = InPin ->QueryPinInfo(&oPinInfo);

		hr = m_opGraph -> RemoveFilter(oPinInfo.pFilter);
		if(FAILED(hr)) 
		{
			// The stream cannot seek.
			MessageBox(NULL, "Couldn't remove render-filter from graph", "Message", MB_OK);
		}

		return OutPin;
	}
	MessageBox(NULL, "Couldn't Find any renderer of specifyed type", "Message", MB_OK);
	return NULL;
}

IPin* EMBeVideoFile::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
    BOOL       bFound = FALSE;
    IEnumPins  *pEnum;
    IPin       *pPin;
	bool		FoundIt = false;
    pFilter->EnumPins(&pEnum);
    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);
        if (bFound = (PinDir == PinDirThis))
		{
//			MessageBox(NULL, "Found Matching PIN_DIRECTION", "Looking for a PIN", MB_OK);
			FoundIt = true;
            break;
		}
        pPin->Release();
//			MessageBox(NULL, "Found A Non Matching PIN_DIRECTION", "Looking for a PIN", MB_OK);
    }
//	if(!FoundIt)
//			MessageBox(NULL, "DIDN'T FIND A MATCHING PIN", "WARNING WARNING!!!", MB_OK);
    pEnum->Release();
    return (bFound ? pPin : 0);  
}

#endif