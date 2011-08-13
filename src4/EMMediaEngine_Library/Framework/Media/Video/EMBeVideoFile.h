/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
* This class opens a file, finds out which format it is
* and reads from it, filling a buffer with data.
*
* It can also seek to a frame as well as time.
*
* Richard Kronfält, 00-11-82
*******************************************************/

#include "EMGlobals.h"
#include "EMConstants.h"

#ifdef PLATFORM_WINDOWS //PLATFORM_BEOS

#ifndef __EM_BE_VIDEO_FILE
#define __EM_BE_VIDEO_FILE

#include "EMMediaGlobals.h"
#include "EMVideoFile.h"
#include "ITestFilter.h"
//#include "EMWinGrabBuffer.h"
#include "EMMediaMatrix.h"

#include "EMBeMediaFile.h"

#include <math.h>
#include <dshow.h>
#include <qedit.h>
#include <stdio.h>
//#include <iostream.h>
#include <string>

#include "EMListener.h"

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

class EMBeVideoFile : public EMVideoFile
{
public:
	~EMBeVideoFile();
	IPin* FindAndDiscardRenderer(bool p_vType);
	float* GetAspectRatio();
	int64 GetBitmapAtTimePosition(int64* p_vToTime, void* p_opBitmap);
	int64 GetDuration();
	EMMediaFormat* GetFormat();
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	int64 GetSize() const;
	bool InitCheckE();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	int64 NumberOfFramesInFile();
	int64* NumberOfFramesInWFPS(float p_vFramerateCurrentFPS);
	HBITMAP ReadAndScaleData(int64 p_vTime, int16 p_vHeight);
//	BBitmap* ReadAndScaleDataKeyFrames(int64 p_vTime, int16 p_vHeight);
	int64 ReadData(EMMediaDataBuffer** p_opBufferPrimary, EMMediaDataBuffer** p_opBufferSecondary, int64 p_vMediaTimePosition, int64 p_vBufferOffset = 0, bool p_vSeeking = false);
	EMMediaFormat* ReadFormatE();
	void ResetScaling(const long width, const long height);
	void Scale(void *src, void *dest);
	void scale2 (void * imgIn, void * imgOut);//, double xscale, double yscale);
	void SeekToFrame(int64 p_vToFrame);
	
	EMBeVideoFile(string p_oFileName);
protected:
	friend EMVideoFile;

private:
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

	float m_vMultiplier;
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
	float m_vxfac;
	float m_vyfac;

//Windows Specific interfaces
	HANDLE m_opConsoleHandle;
	IMediaControl*	m_opMediaControl;
	IMediaSeeking* m_opSeek; 
	IMediaEvent* m_opEvent;
	IVideoFrameStep* m_opVideoFrameStep;
    IGraphBuilder*	m_opGraph;
	AM_MEDIA_TYPE m_opType;
	BITMAPINFO m_oBitmapInfo;
	HBITMAP m_oBitmapInfoTrash;
	REFERENCE_TIME m_Position;
	VIDEOINFOHEADER* m_opVideoHeader;
	IBaseFilter* m_opIBaseFilterDumpRenderer;
	ITestFilter* m_opITestFilterDumpDumpDumpHeader;

};

#endif

#endif