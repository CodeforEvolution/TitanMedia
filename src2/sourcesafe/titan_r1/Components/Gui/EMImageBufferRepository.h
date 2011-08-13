/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef EM_IMAGE_BUFFER_REPOSITORY
#define EM_IMAGE_BUFFER_REPOSITORY

#include "EMBitmap.h"
#include "EMListener.h"
#include "EMMediaGlobals.h"
#include "EMProjectDataContainer.h"
#include "EMThreadListener.h"

#include <list>
#include <map>
#include <vector>

const int EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE = 10000;

class EMGClip;
class EMListenerRepository;
//class EMSemaphore;
class EMView;

class EMImageChunk
{
public:
	EMMediaType m_eType;
	EMBitmap* m_opVideoBufferChunk;
	int64 m_vBeginPosition;
	int64 m_vEndPosition; // For audio chunks only?
	bool m_vIsValid;
	int16* m_vpAudioBufferChunk;
	int64 m_vPriority;
	int32 m_vFrameHeight; // For video chunks only
	
	EMImageChunk(EMMediaType p_eType) :
	m_eType(p_eType),
	m_opVideoBufferChunk(NULL),
	m_vIsValid(false),
	m_vpAudioBufferChunk(NULL)
	{
	}	
	~EMImageChunk()
	{
		delete m_vpAudioBufferChunk;
		delete m_opVideoBufferChunk;
	}
};

class EMImageRegion
{
public:
	int64 m_vClipRegionBegin;
	int64 m_vClipRegionEnd;
	int64 m_vFileRegionBegin;
	int64 m_vFileRegionEnd;
};

class EMImageFileInfo
{
public:
	EMMediaType m_eMediaType;
	float m_vAspectRatio; // For video chunks only
	int m_vChunkSize;
	int32 m_vFileID;
	int64 m_vFileLength;
	int64 m_vNrHiResSamples; // For audio chunks only
	int64 m_vNrLoResSamples; // For audio chunks only
	int64 m_vNrChunks;
	int64 m_vNrChannels; // For audio chunks only
};

class EMImageBufferRepository : public EMThreadListener, public EMListener, public EMProjectDataContainer
{
public:
	EMImageBufferRepository();
	~EMImageBufferRepository();
	bool AudioDataAvailable(EMGClip* p_opClip, int64 p_vBeginPosition, int64 p_vEndPosition);
	bool ClearData();
	void ClipAdded(EMGClip* p_opClip, int32 p_vMediaFileID);
	void ClipMoved(EMGClip* p_opClip);
	void ClipRemoved(EMGClip* p_opClip);
	void GetFrameWidthAndSkipRate(EMGClip* p_opClip, int32 p_vFrameHeight, int32* p_vpFrameWidth, int* p_vpSkipRate);
	uint32 GetVideoChunkIndex(int64 p_vPosition, int p_vSkipRate, vector<EMImageChunk*>* p_opChunks);
	static EMImageBufferRepository* Instance();
	vector<EMImageChunk*>* GetData(EMGClip* p_opClip);
	EMImageFileInfo GetFileInfo(EMGClip* p_opClip);
	uint8* GetLowResAudio(EMGClip* p_opClip);
	void Invalidate(EMGClip* p_opClip, int64* p_vpClipBeginPosition, int64* p_vpClipEndPosition, int64* p_vpFileBeginPosition, int64* p_vpFileEndPosition);
	//void Lock();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void PrepareToClearData();
	void SetEnabled(bool p_vEnabled);
	void ReadyForUse();
	void SetView(EMView* p_opView);
	void SetVisibleArea(EMRect p_oRect);
	void ThreadRun(EMThread* p_opThread);
	//void Unlock();
	bool VideoDataAvailable(EMGClip* p_opClip, int64 p_vBeginPosition, int64 p_vEndPosition, int32 p_vFrameHeight);

private:
	void AudioFileAdded(int32 p_vFileID);
	void AudioFileRemoved(int32 p_vFileID, bool p_vSemaphore = true);
	void CalculatePriorities();
	void DeleteChunk(EMImageChunk* p_opChunk);
	void FileRemoved(int32 p_vFileID);
	void FillAudioChunk(EMImageChunk* p_opChunk, int32 p_vFileID);
	void FillVideoChunk(EMImageChunk* p_opChunk, int32 p_vFileID, int16 p_vHeight);
	void FramerateChanged();
	uint32 GetAudioChunkIndex(int64 p_vPosition, vector<EMImageChunk*>* p_opChunks, int p_vChannels);
	void GetFrameWidthAndSkipRate(int32 p_vFileID, int32 p_vFrameHeight, int32* p_vpFrameWidth, int* p_vpSkipRate);
	void MakeRoomForNewChunk();
	void VideoFileAdded(int32 p_vFileID, bool p_vSemaphore = true);
	void VideoFileRemoved(int32 p_vFileID, bool p_vSemaphore = true);
	
private:
	map<int32, uint8*> m_oAudioLowResData;
	map<int32, vector<EMImageChunk*>* > m_oChunks;
	map<EMGClip*, int32> m_oClipToFileIDMapping;
	list<EMImageChunk*> m_oPriorities;
	map<EMGClip*, EMImageRegion> m_oInvalidatedRegions;
	map<int32, EMImageFileInfo> m_oMediaFileInfo;
	static EMImageBufferRepository* m_opInstance;
	//EMSemaphore* m_opSemaphore;
	EMView* m_opView;
	EMRect m_oVisibleArea;
	int32 m_vAudioSampleRate; // Cached setting
	bool m_vCalculatePriorities;
	float m_vFramerate; // Cached setting
	int64 m_vTimeZoomScale; // Cached setting
	int32 m_vTotalBufferImageChunks; // The number of image chunks in the buffer
	int64 m_vTotalBufferSize; // The total size of all chunks in bytes
	int64 m_vVisiblePositionBegin;
	int64 m_vVisiblePositionEnd;
};

#endif