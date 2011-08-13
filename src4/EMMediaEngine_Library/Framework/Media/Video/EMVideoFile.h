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

#ifndef __EM_VIDEO_FILE
#define __EM_VIDEO_FILE
#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#include <string>

//TEMP
#include <dshow.h>
#include <qedit.h>
//TEMP
//#include "EMListener.h"
#include "EMBitmap.h"
class EMMediaDataBuffer;
class EMMediaFormat;
class EMSemaphore;

class EMVideoFile// : public EMListener
{
public:
	static EMVideoFile* Create(string p_oFileName);
	virtual ~EMVideoFile();
	virtual float* GetAspectRatio() = 0;
	virtual int64 GetBitmapAtTimePosition(int64* p_vToTime, void* p_opBitmap) = 0;
	virtual int64 GetDuration() = 0;
	virtual EMMediaFormat* GetFormat() = 0;
	int32 GetID() const;
	virtual int64 GetSize() const = 0;
	virtual bool InitCheckE() = 0;
	static bool LockVideo();
	static void UnlockVideo();
	//virtual bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage) = 0;
	virtual int64 NumberOfFramesInFile() = 0;
	virtual int64* NumberOfFramesInWFPS(float p_vFramerateCurrentFPS) = 0;
	virtual int64 ReadData(EMMediaDataBuffer** p_opBufferPrimary, EMMediaDataBuffer** p_opBufferSecondary, int64 p_vMediaTimePosition, int64 p_vBufferOffset = 0, bool p_vSeeking = false) = 0;
	virtual EMMediaFormat* ReadFormatE() = 0;
//	virtual void ResetScaling(const long width, const long height) = 0;
//	virtual void Scale(void *src, void *dest) = 0;
	virtual void SeekToFrame(int64 p_vToFrame) = 0;
	virtual HBITMAP ReadAndScaleData(int64 p_vTime, int16 p_vHeight) = 0; //TEMP

	virtual void SetOutputFormat(EMMediaFormat *p_opFormat) = 0;

	virtual const char* GetDimensionString() = 0;

	virtual void SetMagicSpeedValue(float p_vMagicSpeedValue) = 0;
	virtual float GetMagicSpeedValue() = 0;


protected:
	EMVideoFile(string p_oFileName);

private:
	string m_oFileName;
	int32 m_vID;
	static EMSemaphore* m_opVideoImageProcessingSemaphore;
};

#endif
