/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
* This class contains pre-allocated BBuffers
* for video as well as for audio. This (1) to be able to
* encapsulate the native BBufferGroup , (2) to hold both
* video and audio buffer at the same place, and (3) to 
* be able to access those group from wherever in the 
* system, using the Singleton-concept.
*
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_BUFFER_REPOSITORY
#define __EM_BE_BUFFER_REPOSITORY

#include "EMMediaGlobals.h"
#include "EMMediaFormat.h"
#include "EMListenerRepository.h"
#include "EMBufferRepository.h"

#include <BufferGroup.h>
class BBitmap;
class BBuffer;

class EMBeBufferRepository : public EMBufferRepository
{
public:
	~EMBeBufferRepository();
	BBufferGroup* GetAudioBufferGroup();
	BBufferGroup* GetVideoBufferGroup();
	bool InitializeAudioE(uint64 p_vBufferSize = EM_AUDIO_BUFFER_SIZE, int32 p_vNumBuffers = 10);
	bool InitializeVideoE(uint64 p_vBufferSize, int32 p_vNumBuffers, media_format* p_format, bool p_vDeleteFlag = false);
	EMMediaFormat* GetCurrentVideoFormat();
	BBitmap** GetVideoBitmaps() const;
	int32 GetNumVideoBitmaps() const;
	BBuffer* GetVideoBuffer();
	void NotifyHistoryBufferDelete();
	void RecycleVideoBuffer(BBuffer* p_opBuffer);
	bool ResetVideoE(media_format* p_format);
	void NotifyVideoFormatChange();
	int32 GetVideoBufferSize();

protected:
	EMBeBufferRepository();
	friend EMBufferRepository;

private:
	static EMBeBufferRepository* m_opInstance;
	int32 m_vID;
	BBufferGroup* m_opAudioBuffers;
	BBufferGroup* m_opVideoBuffers;
	BBufferGroup* m_opVideoBuffersRclm;
	int32 m_vVideoBufferSize;
	int32 m_vVideoBufferSizeRclm;
	BBitmap** m_opBitmapArray;
	BBitmap** m_opBitmapArrayRclm;
	int32 m_vNumVideoBitmaps;
	EMMediaFormat* m_opMediaFormat;	
	BBuffer** m_opBBufferArray;
	int32 vNextVideoBuffer;
	sem_id vVideoSemaphore;
	buffer_clone_info* sInfo;
	bool m_vFirstRun;
};

#endif

#endif
