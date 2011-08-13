
#include "EMWinQuicktimeVideo.h"

#include "EMMediaEngine.h"

#include "EMBitmap.h"
#include "EMWinBitmap.h"

//#include "EMVideoPreview.h"

#include "FixMath.h"

EMWinQuicktimeVideo::EMWinQuicktimeVideo(std::string p_oFileName) :
	m_oFileName(p_oFileName),
	m_vIsInitialized(false),
	m_gSrcGWorld(NULL),
	m_opSource(EM_new EMMediaFormat(EM_TYPE_RAW_VIDEO)),
	m_opDestination(EM_new EMMediaFormat(EM_TYPE_RAW_VIDEO)),
	m_gOldPort(NULL),
	m_gOldDevice(NULL)
{
	try
	{

		if( InitializeQTML(0) != noErr) //Initialize QTML
		{
			MessageBox(NULL,"You need to install QuickTime 5.0 or higher", "QuickTime failed!", NULL);
			return;
		}

		if(EnterMovies() != noErr) //Initialize Quicktime
		{
			MessageBox(NULL,"You need to install QuickTime 5.0 or higher", "QuickTime failed!", NULL);
		}
	}
	catch(...)
	{
		MessageBox(NULL, "Exception caught while initializing QuickTime!(Needs QuickTime 5.0 or higher)\nWill attempt to continue anyway.", "Titan vs QuickTime", MB_OK | MB_ICONERROR);
	}
}

EMWinQuicktimeVideo::~EMWinQuicktimeVideo()
{
	try
	{
		//SetGWorld (oldPort, oldDevice);     // Restore previous graphics world

		// Restore previous graphics world
		if(m_gOldPort != NULL && m_gOldDevice != NULL)
			SetGWorld(m_gOldPort, m_gOldDevice);

		if(m_gSrcGWorld != NULL)
			DisposeGWorld (m_gSrcGWorld);        // Dispose of movie's graphics world
		//Exit Close Movie and Terminate QTML
		ExitMovies();
		TerminateQTML();

		delete m_opSource;
		delete m_opDestination;
	}
	catch(...)
	{
		MessageBox(NULL, "Exception caught while unitializing QuickTime!\nWill attempt to continue anyway.", "Titan vs QuickTime", MB_OK | MB_ICONERROR);
	}
}


bool EMWinQuicktimeVideo::CheckIfQuickTimeMovie(std::string* p_oFileName, bool p_vAudioVideo)
{
	OSErr errCode;		// Result code
	FSSpec fileSpec;	// Descriptive information on file to open
	short theFile;		// Reference number of movie file
	OSType mediaTypeToCheck;

	try
	{

		m_oFileName = *p_oFileName;

		c2pstr ((char*)m_oFileName.c_str());
		errCode = FSMakeFSSpec
				(0,							// Volume reference number
				0,							// ID of parent directory
				(unsigned char*)m_oFileName.c_str(),	// File name
				&fileSpec);					// Returns a specification record

		p2cstr ((unsigned char*)m_oFileName.c_str());
		if ( errCode != noErr )										// Was there an error?
		{
			return false;
		}

		errCode = OpenMovieFile (&fileSpec, &theFile, fsRdPerm);	// Open the movie file
		if ( errCode != noErr )										// Was there an error?
		{
		return false;
		}


		errCode = NewMovieFromFile (&m_oMovie, theFile, nil, nil, newMovieActive, nil); //Get Movie from file
		
		CloseMovieFile (theFile);                                  // Close the file

		if ( errCode != noErr )															// Was there an error?
		{
			return false;
		}

		if(p_vAudioVideo == 0)
			mediaTypeToCheck = VideoMediaType;
		else if(p_vAudioVideo == 1)
			mediaTypeToCheck = SoundMediaType;

		// remove all tracks except video in modifier movie
		for (int lIndex = 1; lIndex <= GetMovieTrackCount(m_oMovie); lIndex++)
		{
			OSType      dwType;
			Track track = GetMovieIndTrack(m_oMovie, lIndex);

			GetMediaHandlerDescription(GetTrackMedia(track), &dwType, nil, nil);

			if (dwType != mediaTypeToCheck)
			{
				DisposeMovieTrack(track);
				lIndex--;
			}
			else if(dwType == mediaTypeToCheck)
			{
				DisposeMovieTrack(track);
				lIndex--;
				return true;
			}
		}
	}
	catch(...)
	{
		MessageBox(NULL, "Exception caught while using QuickTime!(Needs QuickTime 5.0 or higher)\nWill attempt to continue anyway.", "Titan vs QuickTime", MB_OK | MB_ICONERROR);
	}

	return false;
}

bool EMWinQuicktimeVideo::InitCheckE()
{
	OSErr errCode;		// Result code
		
	c2pstr ((char*)m_oFileName.c_str());

	try
	{
		errCode = FSMakeFSSpec
				(0,							// Volume reference number
				0,							// ID of parent directory
				(unsigned char*)m_oFileName.c_str(),	// File name
				&m_fileSpec);					// Returns a specification record

		p2cstr ((unsigned char*)m_oFileName.c_str());

		if ( errCode != noErr )										// Was there an error?
		{
			return false;
		}

		errCode = OpenMovieFile (&m_fileSpec, &m_theFile, fsRdPerm);	// Open the movie file
		if ( errCode != noErr )										// Was there an error?
		{
		return false;
		}


		errCode = NewMovieFromFile (&m_oMovie, m_theFile, nil, nil, newMovieActive, nil); //Get Movie from file
		
		CloseMovieFile (m_theFile);                                  // Close the file


		// remove all tracks except video in modifier movie
		for (int lIndex = 1; lIndex <= GetMovieTrackCount(m_oMovie); lIndex++)
		{
			 Track       track = GetMovieIndTrack(m_oMovie, lIndex);
			OSType      dwType;

			GetMediaHandlerDescription(GetTrackMedia(track), 
									&dwType, NULL, NULL);
			if (dwType != VideoMediaType) {
			 DisposeMovieTrack(track);
			 lIndex--;
			}
		}
		
		if(GetMovieTrackCount(m_oMovie) == 0)
			return false;

		//Bounds
		GetMovieBox(m_oMovie, &m_sNaturalBounds);
 
		//We skipp the below line (don't know if it is needed)
		//GetGWorld (&oldPort, &oldDevice);   // Save previous graphics world

		//Create a MAC GWorld to pass the frame to
		//GetMovieGWorld (m_oMovie, &m_gSrcGWorld, NULL);

		errCode = QTNewGWorld(&m_gSrcGWorld, k24BGRPixelFormat, &m_sNaturalBounds,NULL,NULL,NULL);
		// Was there an error?
		m_hbitmapSrc = (HBITMAP)GetPortHBITMAP((GrafPtr)m_gSrcGWorld);
		m_hdcSrc = (HDC)GetPortHDC((GrafPtr)m_gSrcGWorld);

		// Save previous graphics
		GetGWorld (&m_gOldPort, &m_gOldDevice);

		//Point to our current graphicWorld
		SetMovieGWorld(m_oMovie, m_gSrcGWorld, GetGWorldDevice(m_gSrcGWorld));

		//Get som attributes
		//Duration
		m_movieTimeScale = GetMovieTimeScale (m_oMovie);
		long movieTime = GetMovieDuration (m_oMovie);
		m_vDuration = (static_cast<double>(movieTime) / static_cast<double>(m_movieTimeScale) * static_cast<double>(1000000.0));


		//Depth (Skipp depthfor now , static at 24bits)


		if ( errCode != noErr )
		{
			return NULL;
		}

		m_opSource -> m_vWidth = m_sNaturalBounds.right - m_sNaturalBounds.left;
		m_opSource -> m_vHeight = m_sNaturalBounds.bottom - m_sNaturalBounds.top;
		m_opSource -> m_vDepth = (**m_gSrcGWorld -> portPixMap).pixelSize / 8;
		m_opSource -> m_vBytesPerRow = m_opSource -> m_vWidth * m_opSource -> m_vDepth;
		m_opSource -> m_vBufferSizeBytes = m_opSource -> m_vBytesPerRow * m_opSource -> m_vHeight;
		m_vIsInitialized = true;

	}
	catch(...)
	{
		MessageBox(NULL, "Exception caught while using QuickTime!(Needs QuickTime 5.0 or higher)\nWill attempt to continue anyway.", "Titan vs QuickTime", MB_OK | MB_ICONERROR);
	}

	return true;
}


HBITMAP EMWinQuicktimeVideo::GetDIBAtTime(int p_Width, int p_Height, _int64 p_vTime)
{
	//InitCheckE();
	// Result code
//	OSErr errCode;
	void* vDestDataBits = DoCreateDIBSection(24, p_Width, p_Height);

	TimeValue		myMediaEngineProvidedTime;
	myMediaEngineProvidedTime = ((m_movieTimeScale * p_vTime / 1000000.0)+0.5);

	//Move to the correct time
	SetMovieTimeValue(m_oMovie, myMediaEngineProvidedTime);
	//UpdateMovie(m_oMovie);
	MoviesTask(m_oMovie, 0);

	DIBSECTION oDibInfo;
	GetObject(m_dibHBitmap, sizeof(DIBSECTION), &oDibInfo);

	m_opDestination -> m_vWidth = p_Width;
	m_opDestination -> m_vHeight = -p_Height;
	m_opDestination -> m_vDepth = 3;
	m_opDestination -> m_vBytesPerRow = oDibInfo.dsBmih.biSizeImage / oDibInfo.dsBm.bmHeight;
	m_opDestination -> m_vBufferSizeBytes = oDibInfo.dsBmih.biSizeImage;

	m_oScale.InitScaleFunctions(m_opSource, m_opDestination);

	m_oScale.Scale((**m_gSrcGWorld->portPixMap).baseAddr, vDestDataBits);

    return m_dibHBitmap;
}


bool EMWinQuicktimeVideo::CopyFrameIntoBuffer(_int64 p_vTime, EMMediaDataBuffer* m_opMediaDataBuffer)
{
	// Result code
//	OSErr errCode;

	TimeValue		myMediaEngineProvidedTime;
	myMediaEngineProvidedTime = ((m_movieTimeScale * p_vTime / 1000000.0)+0.5);

	//*****************'
/*	TimeValue		myCurrTime;
	TimeValue		myNextTime;
	short			myFlags;
	OSType			myTypes[1];
	myFlags = nextTimeMediaSample;//nextTimeStep;									// we want the next frame in the movie's media
	myTypes[0] = VisualMediaCharacteristic;					// we want video samples
	myCurrTime = GetMovieTime(m_oMovie, NULL);
	GetMovieNextInterestingTime(m_oMovie, myFlags, 1, myTypes, myCurrTime, fixed1, &myNextTime, NULL);
*/
  //*************

	//Move to the correct time
//	SetMovieTimeValue(m_oMovie, myNextTime);
	SetMovieTimeValue(m_oMovie, myMediaEngineProvidedTime);
//	UpdateMovie(m_oMovie);

	MoviesTask(m_oMovie, 0L);

//	HBITMAP hbitmapSrc = (HBITMAP)GetPortHBITMAP((GrafPtr)m_gSrcGWorld);
//	HDC hdcSrc = (HDC)GetPortHDC((GrafPtr)m_gSrcGWorld);

//	DIBSECTION oDibInfo2;
//	GetObject(m_hbitmapSrc, sizeof(DIBSECTION), &oDibInfo2);

//	long vBytesPerRowInSource = oDibInfo2.dsBm.bmWidthBytes;//(m_sNaturalBounds.right * m_sNaturalBounds.bottom * 3) / m_sNaturalBounds.bottom;
	m_opMediaDataBuffer -> m_oFormat.m_vHeight = -m_opMediaDataBuffer -> m_oFormat.m_vHeight;
	m_oScale.InitScaleFunctions(m_opSource, &(m_opMediaDataBuffer -> m_oFormat));
	m_opMediaDataBuffer -> m_oFormat.m_vHeight = abs(m_opMediaDataBuffer -> m_oFormat.m_vHeight);

//	m_oScale.InitScaleFunctions(m_sNaturalBounds.right, m_sNaturalBounds.bottom, m_opMediaDataBuffer->m_oFormat.m_vWidth, -m_opMediaDataBuffer -> m_oFormat.m_vHeight, 3, vBytesPerRowInSource, m_opMediaDataBuffer -> m_oFormat.m_vBytesPerRow);

//	memcpy(m_opMediaDataBuffer -> Data(), oDibInfo2.dsBm.bmBits, oDibInfo2.dsBmih.biSizeImage);

	m_oScale.Scale((**m_gSrcGWorld->portPixMap).baseAddr/*oDibInfo2.dsBm.bmBits*/, m_opMediaDataBuffer -> Data());

    return true;
}


void* EMWinQuicktimeVideo::DoCreateDIBSection(WORD	depth, LONG	width, LONG height)
{
	UINT	usage;
//	HDC hdcTemp;
	m_bits = NULL;
	// create an HDC
//	hdcTemp = GetDC(NULL);
//	m_hdcDest = CreateCompatibleDC(NULL); //hdcTemp);
//	ReleaseDC(NULL,hdcTemp);
      	
	BITMAPINFO bmi;// = nil;

	bmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth			= width;
		// important!!! - we specify a top-down DIB (by passing a negative height value)
		//	with origin in the upper left, otherwise our movie image will show up as inverted
	bmi.bmiHeader.biHeight			= height;
	bmi.bmiHeader.biPlanes			= 1;
	bmi.bmiHeader.biBitCount		= depth;
	bmi.bmiHeader.biSizeImage		= width*height*(depth/8);
	bmi.bmiHeader.biCompression		= 0;
	bmi.bmiHeader.biClrUsed			= 0;
	bmi.bmiHeader.biClrImportant	= 0;
    bmi.bmiHeader.biXPelsPerMeter	= 0;
    bmi.bmiHeader.biYPelsPerMeter	= 0;
    bmi.bmiHeader.biClrUsed			= 0;
    bmi.bmiHeader.biClrImportant	= 0;
    bmi.bmiColors[0].rgbBlue		= 0;
    bmi.bmiColors[0].rgbGreen		= 0;
    bmi.bmiColors[0].rgbRed			= 0;
    bmi.bmiColors[0].rgbReserved	= 0;

	usage = DIB_RGB_COLORS;

	switch ( depth )
	{
		case 16:
		case 24:
		case 32:
			{					
				bmi.bmiHeader.biCompression = BI_RGB;
				usage = DIB_RGB_COLORS;
			}
			break;

		default:
			break;
	}

	m_dibHBitmap = CreateDIBSection(/*m_hdcDest*/ NULL, &bmi, usage, &m_bits, NULL, 0);
//	ReleaseDC(NULL, m_hdcDest);
	return m_bits;
}



_int32 EMWinQuicktimeVideo::GetDuration()
{
	if(!m_vIsInitialized)
		return 0;
	else
	{
		return m_vDuration;
	}
}

int EMWinQuicktimeVideo::GetWidth()
{	if(!m_vIsInitialized)
		return 0;
	else
	{
		return m_sNaturalBounds.right - m_sNaturalBounds.left;
	}
}

int EMWinQuicktimeVideo::GetHeight()
{
	if(!m_vIsInitialized)
		return 0;
	else
	{
		return m_sNaturalBounds.bottom - m_sNaturalBounds.top;
	}
}

int EMWinQuicktimeVideo::GetDepth()
{
	if(!m_vIsInitialized)
		return 0;
	else
	{
		return m_vDepth;
	}
}
