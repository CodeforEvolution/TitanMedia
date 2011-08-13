/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* Initializes the filter for reading a frame from a clip,
* and the acctual reading function and seeking functions.
* Also scaling bitmaps.
*  01-04-10
*******************************************************/
#include "EMGlobals.h"
#include "EMConstants.h"

#ifdef PLATFORM_WINDOWS //PLATFORM_BEOS

#ifndef __EM_WIN_VIDEO_FILE
#define __EM_WIN_VIDEO_FILE

#include <windows.h>
#include <stdio.h>


#include <Vfw.h>

#include "EMMediaGlobals.h"
#include "EMVideoFile.h"
#include "EMReadFrameInterface.h"

//#include "EMWinGrabBuffer.h"
#include "EMMediaMatrix.h"

//#include "EMBeMediaFile.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaFormat.h"

#include <math.h>
#include <dshow.h>
#include <qedit.h>
//#include <iostream.h>
#include <string>
#include "EMBitmap.h"
#include "EMWinBitmap.h"
//#include "EMListener.h"
#include "EMScalingUtils.h"
#include "EMVideoPreview.h"
#include "EMWinQuicktimeVideoSuper.h"

class EMMediaDataBuffer;
class EMMediaFormat;

class EMRowValues
{
public:
	EMRowValues(){};
	long x_i;
	float p;
	float p1;
};

class EMWinVideoFile : public EMVideoFile
{
public:
	~EMWinVideoFile();
	IPin* FindAndDiscardRenderer(bool p_vType);
	float* GetAspectRatio();
	int64 GetBitmapAtTimePosition(int64* p_vToTime, void* p_opBitmap);
	int64 GetDuration();
	EMMediaFormat* GetFormat();
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	int64 GetSize() const;
	bool InitCheckE();
	//bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	int64 NumberOfFramesInFile();
	int64* NumberOfFramesInWFPS(float p_vFramerateCurrentFPS);
	HBITMAP ReadAndScaleData(int64 p_vTime, int16 p_vHeight);
//	BBitmap* ReadAndScaleDataKeyFrames(int64 p_vTime, int16 p_vHeight);
	int64 ReadData(EMMediaDataBuffer** p_opBufferPrimary, EMMediaDataBuffer** p_opBufferSecondary, int64 p_vMediaTimePosition, int64 p_vBufferOffset = 0, bool p_vSeeking = false);
	EMMediaFormat* ReadFormatE();
	void SeekToFrame(int64 p_vToFrame);

	virtual void SetOutputFormat(EMMediaFormat *p_opFormat);

	const char* GetDimensionString();
	const char* GetFrameRateString();
	void SetMagicSpeedValue(float p_vMagicSpeedValue);
	float GetMagicSpeedValue();

	EMWinVideoFile(string p_oFileName);

	void CloseAVIStream();

	//***********Operation "murder thread" 2001-09-24***********
	bool IAMConnected();
	void SetIAMConnected(bool p_vIAMConnected);
	void ReConnectGraph();
	void DisconnectGraph();
	//**********************************************************

protected:
	friend EMVideoFile;

private:
	EMMediaFormat* m_opSource;
	EMMediaFormat* m_opDestination;
	float vAspectRatio;
	unsigned char* m_opBufferContent;
	EMMediaDataBuffer* m_opBufferSecondary;
	EMMediaDataBuffer* m_opBufferSwapping;
	long m_vDcols;
//	BBitmap* m_opDestinationBitMap;
	HBITMAP m_opDestinationBitMap;
	long m_vDrows;
//	BMediaFile* m_opFile;
	string m_oFileName;
	EMMediaFormat* m_opFormat;
//	media_format m_sFormat;
	bool m_vIsInitialized;
//	media_header m_opMediaHeader;
	int64 m_vStart_Time;

	double m_vMultiplier;
	int64 m_vNumberOfFramesInWFPS;
	bool m_vOnlyOnce;
	EMRowValues** m_opR;
	EMRowValues** m_opRptr;
	bool m_vSeeked;
//	BBitmap* m_opSourceBitMap;
	HBITMAP m_opSourceBitMap;
	int64 m_vTimeZoomScale;
	int64 m_vTimeZoomScaleWidth;
	int64 m_vTimeContinuousZoomScaleHeight;
	int64 m_vTimeContinuousZoomScaleWidth;
	int64 m_vTimeZoomScaleHeight;
	int64 m_vTotalDuration;
	int64 m_vTotalNumberOfFrames;
//	BMediaTrack* m_opTrackOut;
//	BBitmap* m_opTrashBitmap;
	float m_vWorkFPS;

//Windows Specific interfaces
	HANDLE m_opConsoleHandle;
	HANDLE m_opSeekSemaphore;
	IMediaSeeking* m_opSeek; 
	IVideoFrameStep* m_opVideoFrameStep;
    IGraphBuilder*	m_opGraph;
	AM_MEDIA_TYPE m_opType;
	BITMAPINFO m_oBitmapInfo;
	HBITMAP m_oBitmapInfoTrash;
	REFERENCE_TIME m_Position;
	VIDEOINFOHEADER* m_opVideoHeader;
//	IBaseFilter* m_opIBaseFilterEMReadFrame;
	EMReadFrameInterface* m_opIReadFrame;

	EMMediaDataBuffer* m_opMediaDataBuffer;

	bool m_vShouldShutdown;
	bool m_vNeedToSyncWithTime;
	bool m_vNeedToSyncWithTimeDone;
	unsigned char* m_opTempBuffer;
	int64 m_opPreviewWindowSize;
	int64 m_vBufferOffset;
	int32 m_vBelongedToUniqueID;

	string m_oDimensionString;
	string m_oFrameRateString;
	//**************************Operation "murder threads" 2001-09-24*****************************************
	IPin* m_pSplitterInPin;
	IPin* m_pSplitterOutPin;
	IPin* m_pSourceOutPin;
	IPin* m_pDecoderInPin;
	IPin* m_pDecoderOutPin;
	IPin* m_pRendererInPin;
	IBaseFilter* myIBaseFilter;
	bool m_vIAMConnected;
	IMediaControl* m_opMediaControl;
	float m_vDwRate;

	//New approach 2001-09-26 :))))
	LPBITMAPINFOHEADER m_pbmih;
	PAVIFILE m_pavi;
	PAVISTREAM m_pstm;
	PGETFRAME m_pfrm;
	AVISTREAMINFOW m_psi;
	EMScalingUtils	m_oScale;
	EMVideoPreview* m_opVideoPreview;

	//Quicktime Approach
	EMWinQuicktimeVideoSuper* m_oQuickTimeVideo;
	HDC m_hdcDest;

	//Clip Speed Attributes
	float m_vMagicSpeedValue;
	//*********************************************************************************************************

};

#endif

#endif