#include "EMImageBufferRepository.h"

#include "CommandIDs.h" // FIXME: An application file in the FW, not good
#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMGClip.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
//#include "EMSemaphore.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThread.h"
#include "EMThreadRepository.h"
#include "EMView.h"

const char* EM_IMAGE_BUFFER_THREAD_NAME = "Image buffer thread";
const int EM_IMAGE_BUFFER_THREAD_SLEEP_TIME = 100000; // 0.1 second
// It is important that the buffer size is larger than what any clip might
// need to display at once, or the buffering algorithm might loop forever
const int64 EM_MAXIMUM_BUFFER_SIZE = 20 * 1024 * 1024; // 20 MB buffer
const int64 EM_MAXIMUM_BUFFER_VIDEO_CHUNKS = 200; // No matter what size, this is the maximum amount of image chunks

// Class used when sorting 
class CompareChunkPriorities
{
public:
	int operator() (EMImageChunk* p_opChunkOne, EMImageChunk* p_opChunkTwo)
	{
		return p_opChunkOne -> m_vPriority > p_opChunkTwo -> m_vPriority;
	}
};

EMImageBufferRepository* EMImageBufferRepository::m_opInstance = NULL;

EMImageBufferRepository::EMImageBufferRepository() :
m_opView(NULL),
m_vCalculatePriorities(false),
m_vTotalBufferImageChunks(0),
m_vTotalBufferSize(0),
m_vFramerate(0),
m_vVisiblePositionBegin(-1),
m_vVisiblePositionEnd(-1)
{
	EMSettingsRepository::Instance() -> AddListener(this);
//	m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
//	m_vAudioSampleRate = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	//m_opSemaphore = EMSemaphore::CreateEMSemaphore("Image Buffer Semaphore");
	EMThread* opThread = EMThreadRepository::Instance() -> CreateThread(EM_IMAGE_BUFFER_THREAD_NAME, EM_THREAD_LOW_PRIORITY, EM_IMAGE_BUFFER_THREAD_SLEEP_TIME);
	opThread -> SetPriority(EM_THREAD_LOW_PRIORITY);
	opThread -> AddListener(this);
	EMApplication::Instance() -> AddListener(this);
	//opThread -> Start();
}

EMImageBufferRepository::~EMImageBufferRepository()
{
	EMThreadRepository::Instance() -> DeleteThread(EM_IMAGE_BUFFER_THREAD_NAME);
	/*
	map<int32, vector<EMImageChunk*>* >::iterator oMapIterator = m_oChunks.begin();
	vector<EMImageChunk*>::iterator oListIterator;
	vector<EMImageChunk*>* opVector;
	EMImageChunk* opChunk;
	while(oMapIterator != m_oChunks.end())
	{
		oListIterator = ((*oMapIterator).second) -> begin();
		while(oListIterator != ((*oMapIterator).second) -> end())
		{
			opChunk = *oListIterator;
			oListIterator++;
			delete opChunk;
		}
		opVector = ((*oMapIterator).second);
		oMapIterator++;
		delete opVector;
	}
	map<int32 , uint8*>::iterator oLowResIterator = m_oAudioLowResData.begin();
	uint8* vpData;
	while(oLowResIterator != m_oAudioLowResData.end())
	{
		vpData = (*oLowResIterator).second;
		oLowResIterator++;
		delete [] vpData;
	}
	*/
	ClearData();
	//delete m_opSemaphore;
}

bool EMImageBufferRepository::AudioDataAvailable(EMGClip* p_opClip, int64 p_vBeginPosition, int64 p_vEndPosition)
{
	m_opView -> AssertWindowLocked();

if(m_oClipToFileIDMapping.find(p_opClip) == m_oClipToFileIDMapping.end())
EMDebugger("Clip is not added to EMImageBufferRepository");
//return false;
	//m_opSemaphore -> Acquire();
	int32 vFileID = m_oClipToFileIDMapping[p_opClip];
	vector<EMImageChunk*>* opChunks = m_oChunks[vFileID];
	EMImageFileInfo vInfo = m_oMediaFileInfo[vFileID];

	uint32 vStartIndex = GetAudioChunkIndex(p_vBeginPosition, opChunks, vInfo.m_vNrChannels);
	uint32 vEndIndex = GetAudioChunkIndex(p_vEndPosition, opChunks, vInfo.m_vNrChannels);
	for(uint32 vIndex = vStartIndex; vIndex <= vEndIndex; vIndex++)
	{
		if(!(*opChunks)[vIndex] -> m_vIsValid)
		{
			//m_opSemaphore -> Release();
			return false;
		}
	}

	// All chunks are available, the clip can draw itself
	//m_opSemaphore -> Release();
	return true;
}

void EMImageBufferRepository::AudioFileAdded(int32 p_vFileID)
{
	if(m_oMediaFileInfo.find(p_vFileID) != m_oMediaFileInfo.end()) // If already added
		return;

	//m_opSemaphore -> Acquire();
	int vChannels = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS, &p_vFileID)));
	int64 vHiResSlots = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_SAMPLES, &p_vFileID)));
	int32 vHiResChunks = vHiResSlots / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE;
	if(vHiResSlots % EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE > 0)
		vHiResChunks++;
	int64 vLoResSlots = vHiResSlots / EM_LOW_RES_AUDIO_SKIP_RATE;
	if(vHiResSlots % EM_LOW_RES_AUDIO_SKIP_RATE > 0)
		vLoResSlots++;
	uint8* vpLowResContent = EM_new uint8[vLoResSlots];
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_LOWRES_CONTENT, &p_vFileID, vpLowResContent, &vLoResSlots);
	m_oAudioLowResData[p_vFileID] = vpLowResContent;
	EMImageChunk* opChunk;
	vector<EMImageChunk*>* opChunks = EM_new vector<EMImageChunk*>();

	for(int32 vIndex = 0; vIndex < vHiResChunks; vIndex++)
	{
		opChunk = EM_new EMImageChunk(EM_TYPE_ANY_AUDIO);
		opChunk -> m_vBeginPosition = vIndex * EM_TICKS_PER_SECOND * EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE / (m_vAudioSampleRate * vChannels);
		opChunk -> m_vEndPosition = ((vIndex + 1) * EM_TICKS_PER_SECOND * EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE / (m_vAudioSampleRate * vChannels)) - 1;
		opChunks -> push_back(opChunk);
	}

	m_oChunks[p_vFileID] = opChunks;
	EMImageFileInfo oInfo;
	oInfo.m_vFileID = p_vFileID;
	oInfo.m_eMediaType = EM_TYPE_ANY_AUDIO;
	oInfo.m_vChunkSize = EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE;
	oInfo.m_vFileLength = (vHiResSlots * EM_TICKS_PER_SECOND) / (vChannels * m_vAudioSampleRate);
	oInfo.m_vNrHiResSamples = vHiResSlots;
	oInfo.m_vNrLoResSamples = vLoResSlots;
	oInfo.m_vNrChunks = vHiResChunks;
	oInfo.m_vNrChannels = vChannels;
	m_oMediaFileInfo[p_vFileID] = oInfo;
	//m_opSemaphore -> Release();
}

void EMImageBufferRepository::AudioFileRemoved(int32 p_vFileID, bool p_vSemaphore)
{
	//if(p_vSemaphore)
	//	m_opSemaphore -> Acquire();
	delete m_oAudioLowResData[p_vFileID];
	m_oAudioLowResData.erase(p_vFileID);
	FileRemoved(p_vFileID);
	//if(p_vSemaphore)
	//	m_opSemaphore -> Release();
}

void EMImageBufferRepository::CalculatePriorities()
{
	//m_opSemaphore -> Acquire();
	list<EMImageChunk*>::iterator oIterator = m_oPriorities.begin();
	while(oIterator != m_oPriorities.end())
	{
		// FIXME: Calculate a decent priority
		(*oIterator) -> m_vPriority = 123456; //abs((*oIterator) -> m_vBeginPosition - (static_cast<int>(m_oVisibleArea.m_vLeft + (m_oVisibleArea.GetWidth() / 2)))) * (*oIterator) -> m_vChunkSize;
		oIterator++;
	}
	m_oPriorities.sort();
	//m_opSemaphore -> Release();
}

bool EMImageBufferRepository::ClearData()
{
	//m_opView -> AssertWindowLocked();

	//m_opSemaphore -> Acquire();
int vDeletedChunks(0);
	map<int32, vector<EMImageChunk*>* >::iterator oMapIterator = m_oChunks.begin();
	vector<EMImageChunk*>::iterator oVectorIterator;
	vector<EMImageChunk*>* opVector;
	EMImageChunk* opChunk;

	while(oMapIterator != m_oChunks.end())
	{
		oVectorIterator = ((*oMapIterator).second) -> begin();
		while(oVectorIterator != ((*oMapIterator).second) -> end())
		{
			opChunk = *oVectorIterator;
			oVectorIterator++;
			delete opChunk;
			//DeleteChunk(opChunk);
vDeletedChunks++;
		}
		opVector = ((*oMapIterator).second);
		oMapIterator++;
		delete opVector;
	}
	m_oChunks.clear();

	map<int32 , uint8*>::iterator oLowResIterator = m_oAudioLowResData.begin();
	uint8* vpData;
	while(oLowResIterator != m_oAudioLowResData.end())
	{
		vpData = (*oLowResIterator).second;
		oLowResIterator++;
		delete [] vpData;
	}
	m_oAudioLowResData.clear();

	m_oPriorities.clear();
	m_oClipToFileIDMapping.clear();
	m_oInvalidatedRegions.clear();
	m_oMediaFileInfo.clear();

	m_vTotalBufferSize = 0;
	m_vTotalBufferImageChunks = 0;

	//m_opSemaphore -> Release();

	return true;
}

void EMImageBufferRepository::ClipAdded(EMGClip* p_opClip, int32 p_vMediaFileID)
{
	m_opView -> AssertWindowLocked();

	if(p_opClip -> GetType() != EM_TYPE_ANY_AUDIO && p_opClip -> GetType() != EM_TYPE_ANY_VIDEO) // Only interested in audio and video clips
		return;

	if(m_oMediaFileInfo.find(p_vMediaFileID) == m_oMediaFileInfo.end()) // The file isn't added yet to this object, let's add it
	{
		if(p_opClip -> GetType() == EM_TYPE_ANY_AUDIO)
			AudioFileAdded(p_vMediaFileID);
		else
			VideoFileAdded(p_vMediaFileID);
	}

	//m_opSemaphore -> Acquire();
	m_oClipToFileIDMapping[p_opClip] = p_vMediaFileID;
	//m_opSemaphore -> Release();
}

void EMImageBufferRepository::ClipMoved(EMGClip* p_opClip)
{
	m_opView -> AssertWindowLocked();

	//m_opSemaphore -> Acquire();
	m_vCalculatePriorities = true;
	//m_opSemaphore -> Release();
}

void EMImageBufferRepository::ClipRemoved(EMGClip* p_opClip)
{
	m_opView -> AssertWindowLocked();

	if(p_opClip -> GetType() != EM_TYPE_ANY_AUDIO && p_opClip -> GetType() != EM_TYPE_ANY_VIDEO) // Only interested in audio and video clips
		return;

	if(m_oClipToFileIDMapping.find(p_opClip) == m_oClipToFileIDMapping.end())
		return; // The clips has never been added to this object

	//m_opSemaphore -> Acquire();
	int32 vMediaID = m_oClipToFileIDMapping[p_opClip];
	EMMediaType eType = p_opClip -> GetType();
	m_oClipToFileIDMapping.erase(p_opClip);
	m_oInvalidatedRegions.erase(p_opClip);
	bool vClipsAreLeft(false);

	map<EMGClip*, int32>::iterator oIterator = m_oClipToFileIDMapping.begin();
	while(oIterator != m_oClipToFileIDMapping.end())
	{
		if((*oIterator).second == vMediaID)
		{
			vClipsAreLeft = true; // A clip is found with the same media id
			break;
		}
		oIterator++;
	}

	if(!vClipsAreLeft)
	{
		if(eType == EM_TYPE_ANY_VIDEO)
			VideoFileRemoved(vMediaID, false);
		else
			AudioFileRemoved(vMediaID, false);
	}

	//m_opSemaphore -> Release();
}

void EMImageBufferRepository::DeleteChunk(EMImageChunk* p_opChunk)
{
if(!p_opChunk -> m_vIsValid)
{
EMDebugger("Hmm, chunk is not valid");
return; // Some memory may leak
}
	if(p_opChunk -> m_eType == EM_TYPE_ANY_VIDEO)
	{
		int16 vBitmapWidth = static_cast<int16>((p_opChunk -> m_opVideoBufferChunk -> Bounds()).GetWidth());
		m_vTotalBufferSize -= 4 * static_cast<int16>(p_opChunk -> m_vFrameHeight) * vBitmapWidth;
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DELETE_VIDEO_CLIP_CONTENT, p_opChunk -> m_opVideoBufferChunk);
eo << "Video chunk is deleted" << ef;
		p_opChunk -> m_opVideoBufferChunk = NULL;
		m_vTotalBufferImageChunks--;
	}
	else
	{
		m_vTotalBufferSize -= 2 * EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE;
		delete p_opChunk -> m_vpAudioBufferChunk;
eo << "Audio chunk is deleted" << ef;
		p_opChunk -> m_vpAudioBufferChunk = NULL;
	}
	p_opChunk -> m_vIsValid = false;
}

// Assumes that all clips connected to this file are removed before this method is called
void EMImageBufferRepository::FileRemoved(int32 p_vFileID)
{
	EMImageChunk* opChunk;
	vector<EMImageChunk*>* opChunks;
	opChunks = m_oChunks[p_vFileID];

	vector<EMImageChunk*>::iterator oIterator = opChunks -> begin();
	while(oIterator != opChunks -> end())
	{
		opChunk = *oIterator;
		oIterator++;
		if(opChunk -> m_vIsValid)
			m_oPriorities.remove(opChunk);
		delete opChunk;
/*
		if(opChunk -> m_vIsValid)
		{
			DeleteChunk(opChunk);
			m_oPriorities.remove(opChunk);
		}
*/
	}

	m_oChunks.erase(p_vFileID);
	delete opChunks;
	m_oMediaFileInfo.erase(p_vFileID);
}

void EMImageBufferRepository::FillAudioChunk(EMImageChunk* p_opChunk, int32 p_vFileID)
{
	p_opChunk -> m_vpAudioBufferChunk = EM_new int16[EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE];
	int32 vChunkSize = EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE;
	int64 vBeginPosition = p_opChunk -> m_vBeginPosition;
	list<void*> oParameters;
	oParameters.push_back(&vChunkSize);
	oParameters.push_back(&vBeginPosition);
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_HIGHRES_CONTENT, &p_vFileID, p_opChunk -> m_vpAudioBufferChunk, &oParameters);
	p_opChunk -> m_vIsValid = true;
	m_vTotalBufferSize += 2 * EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE;
}

void EMImageBufferRepository::FillVideoChunk(EMImageChunk* p_opChunk, int32 p_vFileID, int16 p_vHeight)
{
if(p_opChunk -> m_opVideoBufferChunk != NULL)
{
EMDebugger("The video chunk to be filled is not NULL");
//delete p_opChunk -> m_opVideoBufferChunk;
// Some memory will leak
}
	int64 vBeginPosition = p_opChunk -> m_vBeginPosition;
	// Temporarily unlock the window to allow the video preview to draw images while caching this image
	m_opView -> UnlockWindow();
	EMBitmap* opVideoBufferChunk = static_cast<EMBitmap*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT, &p_vFileID, &vBeginPosition, &p_vHeight));
	m_opView -> LockWindow();
	p_opChunk -> m_opVideoBufferChunk = opVideoBufferChunk;//static_cast<EMBitmap*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT, &p_vFileID, &vBeginPosition, &p_vHeight));
	p_opChunk -> m_vFrameHeight = p_vHeight;
	p_opChunk -> m_vIsValid = true;
	int16 vBitmapWidth = static_cast<int16>((p_opChunk -> m_opVideoBufferChunk -> Bounds()).GetWidth());
	// Assume each pixel takes up 4 bytes, do not count the size of any header etc.
	m_vTotalBufferSize += 4 * p_vHeight * vBitmapWidth;
	m_vTotalBufferImageChunks++;
//eo << "EMImageBufferRepository::FillVideoChunk: m_vTotalBufferSize: " << (double) m_vTotalBufferSize << ef;
}

// When the framerate is changed, the whole video chunk structure must be rebuilt since the number of chunks
// for each video file is dependant on the framerate and thus changed
void EMImageBufferRepository::FramerateChanged()
{
	if(m_oMediaFileInfo.size() == 0)
		return;

	//if(m_opView -> LockWindow())
	{
		list<int32> oVideoFileIDs;
		map<int32, EMImageFileInfo>::iterator oMediaFileIterator = m_oMediaFileInfo.begin();
		while(oMediaFileIterator != m_oMediaFileInfo.end())
		{
			if((*oMediaFileIterator).second.m_eMediaType == EM_TYPE_ANY_VIDEO)
				oVideoFileIDs.push_back((*oMediaFileIterator).first);
			oMediaFileIterator++;
		}

		list<int32>::iterator oFileIDIterator = oVideoFileIDs.begin();
		while(oFileIDIterator != oVideoFileIDs.end())
		{
			VideoFileRemoved(*oFileIDIterator, false);
			VideoFileAdded(*oFileIDIterator, false);
			oFileIDIterator++;
		}
		//m_opView -> UnlockWindow();
	}
}

vector<EMImageChunk*>* EMImageBufferRepository::GetData(EMGClip* p_opClip)
{
	m_opView -> AssertWindowLocked();

	//m_opSemaphore -> Acquire();
	vector<EMImageChunk*>* opData = m_oChunks[m_oClipToFileIDMapping[p_opClip]];
	//m_opSemaphore -> Release();

	return opData;
}

EMImageFileInfo EMImageBufferRepository::GetFileInfo(EMGClip* p_opClip)
{
	m_opView -> AssertWindowLocked();

	//m_opSemaphore -> Acquire();
	EMImageFileInfo oInfo = m_oMediaFileInfo[m_oClipToFileIDMapping[p_opClip]];
	//m_opSemaphore -> Release();

	return oInfo;
}

uint32 EMImageBufferRepository::GetAudioChunkIndex(int64 p_vPosition, vector<EMImageChunk*>* p_opChunks, int p_vChannels)
{
	uint32 vIndex = static_cast<uint32>((p_vPosition * p_vChannels * m_vAudioSampleRate) / (EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE * EM_TICKS_PER_SECOND));
	if(vIndex >= p_opChunks -> size())
		vIndex = p_opChunks -> size() - 1;

	return vIndex;
}

uint32 EMImageBufferRepository::GetVideoChunkIndex(int64 p_vPosition, int p_vSkipRate, vector<EMImageChunk*>* p_opChunks)
{
	m_opView -> AssertWindowLocked();

	uint32 vIndex = static_cast<uint32>((m_vFramerate * p_vPosition) / (EM_TICKS_PER_SECOND * p_vSkipRate)) * p_vSkipRate;
	if(vIndex >= p_opChunks -> size() && p_opChunks -> size() > 1) // It is assumed that this is a picture if it only has one frame
		vIndex = p_opChunks -> size() - 1;

	return vIndex;
}

void EMImageBufferRepository::GetFrameWidthAndSkipRate(EMGClip* p_opClip, int32 p_vFrameHeight, int32* p_vpFrameWidth, int* p_vpSkipRate)
{
	m_opView -> AssertWindowLocked();

	//m_opSemaphore -> Acquire();
	GetFrameWidthAndSkipRate(m_oClipToFileIDMapping[p_opClip], p_vFrameHeight, p_vpFrameWidth, p_vpSkipRate);
	//m_opSemaphore -> Release();
}

void EMImageBufferRepository::GetFrameWidthAndSkipRate(int32 p_vFileID, int32 p_vFrameHeight, int32* p_vpFrameWidth, int* p_vpSkipRate)
{
	EMImageFileInfo oInfo = m_oMediaFileInfo[p_vFileID];
	float vMaxWidth = EM_TICKS_PER_SECOND / (m_vFramerate * m_vTimeZoomScale);
	*p_vpFrameWidth	= oInfo.m_vAspectRatio * p_vFrameHeight;

	if(*p_vpFrameWidth <= vMaxWidth)
		*p_vpSkipRate = 1;
	else
	{
		*p_vpSkipRate = static_cast<int>(*p_vpFrameWidth / vMaxWidth);
		if((*p_vpFrameWidth * *p_vpSkipRate) > (vMaxWidth * *p_vpSkipRate))
			(*p_vpSkipRate)++;
	}

	*p_vpFrameWidth = vMaxWidth * *p_vpSkipRate;
}

uint8* EMImageBufferRepository::GetLowResAudio(EMGClip* p_opClip)
{
	m_opView -> AssertWindowLocked();

	//m_opSemaphore -> Acquire();
	uint8* vpAudio = m_oAudioLowResData[m_oClipToFileIDMapping[p_opClip]];
	//m_opSemaphore -> Release();

	return vpAudio;
}

EMImageBufferRepository* EMImageBufferRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMImageBufferRepository();

	return m_opInstance;
}

void EMImageBufferRepository::Invalidate(EMGClip* p_opClip, int64* p_vpClipBeginPosition, int64* p_vpClipEndPosition, int64* p_vpFileBeginPosition, int64* p_vpFileEndPosition)
{
	m_opView -> AssertWindowLocked();

eo << "Invalidating: " << *p_vpClipBeginPosition << ", " << *p_vpClipEndPosition << ", " << *p_vpFileBeginPosition << ", " << *p_vpFileEndPosition << ef;
	//m_opSemaphore -> Acquire();
	if(m_oInvalidatedRegions.find(p_opClip) != m_oInvalidatedRegions.end())
	{
		if(*p_vpClipBeginPosition < m_oInvalidatedRegions[p_opClip].m_vClipRegionBegin)
		{
			m_oInvalidatedRegions[p_opClip].m_vClipRegionBegin = *p_vpClipBeginPosition;
			m_oInvalidatedRegions[p_opClip].m_vFileRegionBegin = *p_vpFileBeginPosition;
		}
		if(*p_vpClipEndPosition > m_oInvalidatedRegions[p_opClip].m_vClipRegionEnd)
		{
			m_oInvalidatedRegions[p_opClip].m_vClipRegionEnd = *p_vpClipEndPosition;
			m_oInvalidatedRegions[p_opClip].m_vFileRegionEnd = *p_vpFileEndPosition;
		}
	}
	else
	{
		m_oInvalidatedRegions[p_opClip] = EMImageRegion();
		m_oInvalidatedRegions[p_opClip].m_vClipRegionBegin = *p_vpClipBeginPosition;
		m_oInvalidatedRegions[p_opClip].m_vClipRegionEnd = *p_vpClipEndPosition;
		m_oInvalidatedRegions[p_opClip].m_vFileRegionBegin = *p_vpFileBeginPosition;
		m_oInvalidatedRegions[p_opClip].m_vFileRegionEnd = *p_vpFileEndPosition;
	}
	//m_opSemaphore -> Release();
}
/*
void EMImageBufferRepository::Lock()
{
		m_opSemaphore -> Acquire();
}
*/
void EMImageBufferRepository::MakeRoomForNewChunk()
{
//eo << "EMImageBufferRepository::MakeRoomForNewChunk (before): nr chunks in list: " << m_oPriorities.size() << ", buffer size: " << (double) m_vTotalBufferSize << ef;
	EMImageChunk* opChunk;
	// Remove the oldest chunks until the total buffer size is ok
	while(m_vTotalBufferSize > EM_MAXIMUM_BUFFER_SIZE || m_vTotalBufferImageChunks > EM_MAXIMUM_BUFFER_VIDEO_CHUNKS)
	{
if(m_oPriorities.size() == 0)
{
EMDebugger("Cannot make more room, there's nothing in the buffer");
return; // Some memory will leak
}
		opChunk = m_oPriorities.back();
		m_oPriorities.pop_back();

		DeleteChunk(opChunk);
	}

//eo << "EMImageBufferRepository::MakeRoomForNewChunk (after): nr chunks in list: " << m_oPriorities.size() << ", buffer size: " << (double) m_vTotalBufferSize << ef;
}

bool EMImageBufferRepository::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_SETTING_UPDATED:
		if(m_opView -> LockWindow())
		{
eo << "EMImageBufferRepository: EM_SETTING_UPDATED" << ef;
			bool vFramerateChanged(false);
			//m_opSemaphore -> Acquire();
			m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
			m_vVisiblePositionBegin = static_cast<int64>(m_oVisibleArea.m_vLeft * m_vTimeZoomScale);
			m_vVisiblePositionEnd = static_cast<int64>(m_oVisibleArea.m_vRight * m_vTimeZoomScale);
			m_vAudioSampleRate = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
			float vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
			if(vFramerate != m_vFramerate)
			{
				m_vFramerate = vFramerate;
				vFramerateChanged = true;
			}
			//m_opSemaphore -> Release();
			if(vFramerateChanged)
				FramerateChanged();

			m_opView -> UnlockWindow();
		}
		return true;

	case EM_QUIT_PHASE_TWO:
		EMThreadRepository::Instance() -> GetThread(EM_IMAGE_BUFFER_THREAD_NAME) -> Suspend();
		return true;
	}

	return false;
}

void EMImageBufferRepository::PrepareToClearData()
{
	// Suspending the thread
	EMThreadRepository::Instance() -> GetThread(EM_IMAGE_BUFFER_THREAD_NAME) -> Suspend();
}

void EMImageBufferRepository::ReadyForUse()
{
	// Starting the thread
	EMThreadRepository::Instance() -> GetThread(EM_IMAGE_BUFFER_THREAD_NAME) -> Start();
}

void EMImageBufferRepository::SetEnabled(bool p_vEnabled)
{
// This is a future feature
}

void EMImageBufferRepository::SetView(EMView* p_opView)
{
	// No need to lock the view here, since the thread isn't supposed to be started yet

	if(m_opView != NULL)
		EMDebugger("EMImageBufferRepository already has a view");

	m_opView = p_opView;
	EMThreadRepository::Instance() -> GetThread(EM_IMAGE_BUFFER_THREAD_NAME) -> Start();
}

void EMImageBufferRepository::SetVisibleArea(EMRect p_oRect)
{
	m_opView -> AssertWindowLocked();

	//m_opSemaphore -> Acquire();
	m_oVisibleArea = p_oRect;
	m_vVisiblePositionBegin = static_cast<int64>(m_oVisibleArea.m_vLeft * m_vTimeZoomScale);
	m_vVisiblePositionEnd = static_cast<int64>(m_oVisibleArea.m_vRight * m_vTimeZoomScale);
eo << "Visible area: " << m_vVisiblePositionBegin << ", " << m_vVisiblePositionEnd << ef;
	//m_opSemaphore -> Release();
}

void EMImageBufferRepository::ThreadRun(EMThread* p_opThread)
{
	if(m_oInvalidatedRegions.size() == 0) // Not thread safe, but probably sufficient. Prevents a lot of locks/unlocks, which is good
		return;

	if(m_opView -> LockWindow())
	{
		//m_opSemaphore -> Acquire();
		if(m_oInvalidatedRegions.size() > 0)
		{
			map<EMGClip*, EMImageRegion>::iterator oClipIterator = m_oInvalidatedRegions.begin();
	//;//cout_commented_out_4_release << "Starting to cache data" << endl;
			int32 vFileID;
			EMImageRegion oRegion((*oClipIterator).second);
			int vSkipRate;
			int32 vFrameWidth;
			uint32 vCurrentChunkIndex = UINT32_MAX;
			vector<EMImageChunk*>* opChunks;

			vFileID = m_oClipToFileIDMapping[(*oClipIterator).first];
			opChunks = m_oChunks[vFileID];

			if((*oClipIterator).first -> GetType() == EM_TYPE_ANY_VIDEO)
				GetFrameWidthAndSkipRate(vFileID, ((*oClipIterator).first -> Frame()).GetHeight(), &vFrameWidth, &vSkipRate);
			else
				vSkipRate = 1;

			if(m_vVisiblePositionBegin <= oRegion.m_vClipRegionEnd && m_vVisiblePositionEnd >= oRegion.m_vClipRegionBegin)
			{
eo << "Checking region: clip: " << oRegion.m_vClipRegionBegin << " to " << oRegion.m_vClipRegionEnd << ", file: " << oRegion.m_vFileRegionBegin << " to " << oRegion.m_vFileRegionEnd << ", m_vVisiblePositionBegin: " << m_vVisiblePositionBegin << ", m_vVisiblePositionEnd: " << m_vVisiblePositionEnd << ef;
				uint32 vStartIndex;
				uint32 vEndIndex;

				if(oRegion.m_vClipRegionBegin < m_vVisiblePositionBegin)
				{
					oRegion.m_vFileRegionBegin += (m_vVisiblePositionBegin - oRegion.m_vClipRegionBegin);
					oRegion.m_vClipRegionBegin += (m_vVisiblePositionBegin - oRegion.m_vClipRegionBegin);
				}
				
				if(oRegion.m_vClipRegionEnd > m_vVisiblePositionEnd)
				{
					oRegion.m_vFileRegionEnd += (m_vVisiblePositionEnd - oRegion.m_vClipRegionEnd);
					oRegion.m_vClipRegionEnd += (m_vVisiblePositionEnd - oRegion.m_vClipRegionEnd);
				}

eo << "Checking region: clip: " << oRegion.m_vClipRegionBegin << " to " << oRegion.m_vClipRegionEnd << ", file: " << oRegion.m_vFileRegionBegin << " to " << oRegion.m_vFileRegionEnd << ", m_vVisiblePositionBegin: " << m_vVisiblePositionBegin << ", m_vVisiblePositionEnd: " << m_vVisiblePositionEnd << ef;
				if(opChunks -> size() == 1 && (*opChunks)[0] -> m_eType == EM_TYPE_ANY_VIDEO) // If it's a picture file
				{
					if(!((*opChunks)[0] -> m_vIsValid))
					{
						MakeRoomForNewChunk();
						FillVideoChunk((*opChunks)[0], vFileID, ((*oClipIterator).first -> Frame()).GetHeight());
						m_oPriorities.push_front((*opChunks)[0]);
					}
				}

				else
				{
					if((*oClipIterator).first -> GetType() == EM_TYPE_ANY_VIDEO)
					{
						vStartIndex = GetVideoChunkIndex(oRegion.m_vFileRegionBegin, vSkipRate, opChunks);
						vEndIndex = GetVideoChunkIndex(oRegion.m_vFileRegionEnd, vSkipRate, opChunks);
					}
					else
					{
						EMImageFileInfo vInfo = m_oMediaFileInfo[vFileID];
						vStartIndex = GetAudioChunkIndex(oRegion.m_vFileRegionBegin, opChunks, vInfo.m_vNrChannels);
						vEndIndex = GetAudioChunkIndex(oRegion.m_vFileRegionEnd, opChunks, vInfo.m_vNrChannels);
					}

eo << "vStartIndex: " << vStartIndex << ", vEndIndex: " << vEndIndex << ef;
					for(vCurrentChunkIndex = vStartIndex; vCurrentChunkIndex <= vEndIndex; vCurrentChunkIndex += vSkipRate)
					{
						if(vCurrentChunkIndex >= opChunks -> size())
						{
eo << "vCurrentChunkIndex >= opChunks -> size(): " << vCurrentChunkIndex << ", " << opChunks -> size() << ef;
							break;
						}

						if((*opChunks)[vCurrentChunkIndex] -> m_vBeginPosition > oRegion.m_vFileRegionEnd)
						{
eo << "(*opChunks)[vCurrentChunkIndex] -> m_vBeginPosition > oRegion.m_vFileRegionEnd): " << (*opChunks)[vCurrentChunkIndex] -> m_vBeginPosition << ", " << oRegion.m_vFileRegionEnd << ef;
							break;
						}

eo << "Checking chunk with index " << vCurrentChunkIndex << ": ";
						if(!((*opChunks)[vCurrentChunkIndex] -> m_vIsValid))
						{
eo << "invalid (filling it)..." << ef;
							MakeRoomForNewChunk();

							if((*opChunks)[vCurrentChunkIndex] -> m_eType == EM_TYPE_ANY_AUDIO)
								FillAudioChunk((*opChunks)[vCurrentChunkIndex], vFileID);
							else
								FillVideoChunk((*opChunks)[vCurrentChunkIndex], vFileID, ((*oClipIterator).first -> Frame()).GetHeight());
							m_oPriorities.push_front((*opChunks)[vCurrentChunkIndex]);
eo << "done filling" << ef;
						}
						else
eo << "valid" << ef;
					}
				}
eo << "Caching of data complete" << ef;		

				EMGClip* opClip = (*oClipIterator).first;

				// Since the invalidated region might have changed (due to a temporary window unlock in FillVideo/AudioBuffer), the invalidated region may not be erased
				/*
				uint32 vNewStartIndex;
				uint32 vNewEndIndex;
				EMImageRegion oNewRegion((*oClipIterator).second);
				if(oNewRegion.m_vClipRegionBegin < m_vVisiblePositionBegin)
				{
					oNewRegion.m_vFileRegionBegin += (m_vVisiblePositionBegin - oNewRegion.m_vClipRegionBegin);
					oNewRegion.m_vClipRegionBegin += (m_vVisiblePositionBegin - oNewRegion.m_vClipRegionBegin);
				}
				
				if(oNewRegion.m_vClipRegionEnd > m_vVisiblePositionEnd)
				{
					oNewRegion.m_vFileRegionEnd += (m_vVisiblePositionEnd - oNewRegion.m_vClipRegionEnd);
					oNewRegion.m_vClipRegionEnd += (m_vVisiblePositionEnd - oNewRegion.m_vClipRegionEnd);
				}
				if((*oClipIterator).first -> GetType() == EM_TYPE_ANY_VIDEO)
				{
					vNewStartIndex = GetVideoChunkIndex(oNewRegion.m_vFileRegionBegin, vSkipRate, opChunks);
					vNewEndIndex = GetVideoChunkIndex(oNewRegion.m_vFileRegionEnd, vSkipRate, opChunks);
				}
				else
				{
					EMImageFileInfo vInfo = m_oMediaFileInfo[vFileID];
					vNewStartIndex = GetAudioChunkIndex(oNewRegion.m_vFileRegionBegin, opChunks, vInfo.m_vNrChannels);
					vNewEndIndex = GetAudioChunkIndex(oNewRegion.m_vFileRegionEnd, opChunks, vInfo.m_vNrChannels);
				}
eo << "vNewStartIndex: " << vNewStartIndex << ", vNewEndIndex: " << vNewEndIndex << ef;

				if(vStartIndex == vNewStartIndex && vEndIndex == vNewEndIndex)
					m_oInvalidatedRegions.erase(oClipIterator);
				else
eo << "start/end index has changed, keeping invalidated region" << ef;
				//m_opSemaphore -> Release();
				//if(m_opView -> LockWindow())
				{
					m_opView -> Invalidate(EMRect(oRegion.m_vClipRegionBegin / m_vTimeZoomScale, (opClip -> Frame()).m_vTop + 1, oRegion.m_vClipRegionEnd / m_vTimeZoomScale, (opClip -> Frame()).m_vBottom - 1));//    opClip -> Frame() & m_oVisibleArea);
					//m_opView -> UnlockWindow();
				}
				*/
				if(oRegion.m_vClipRegionBegin == (*oClipIterator).second.m_vClipRegionBegin && oRegion.m_vClipRegionEnd == (*oClipIterator).second.m_vClipRegionEnd)
				{
eo << "Invalidating and erasing region" << ef;
					m_oInvalidatedRegions.erase(oClipIterator);
					m_opView -> Invalidate(EMRect(oRegion.m_vClipRegionBegin / m_vTimeZoomScale, (opClip -> Frame()).m_vTop + 1, oRegion.m_vClipRegionEnd / m_vTimeZoomScale, (opClip -> Frame()).m_vBottom - 1));//    opClip -> Frame() & m_oVisibleArea);
				}
				else
				{
eo << "Adjusting region without invalidating" << ef;
					if((*oClipIterator).second.m_vClipRegionBegin < m_vVisiblePositionBegin)
					{
						(*oClipIterator).second.m_vFileRegionBegin += (m_vVisiblePositionBegin - (*oClipIterator).second.m_vClipRegionBegin);
						(*oClipIterator).second.m_vClipRegionBegin += (m_vVisiblePositionBegin - (*oClipIterator).second.m_vClipRegionBegin);
					}
					
					if((*oClipIterator).second.m_vClipRegionEnd > m_vVisiblePositionEnd)
					{
						(*oClipIterator).second.m_vFileRegionEnd += (m_vVisiblePositionEnd - (*oClipIterator).second.m_vClipRegionEnd);
						(*oClipIterator).second.m_vClipRegionEnd += (m_vVisiblePositionEnd - (*oClipIterator).second.m_vClipRegionEnd);
					}
				}
			}
			else
			{
eo << "Skipping region: clip: " << oRegion.m_vClipRegionBegin << " to " << oRegion.m_vClipRegionEnd << ", file: " << oRegion.m_vFileRegionBegin << " to " << oRegion.m_vFileRegionEnd << ", m_vVisiblePositionBegin: " << m_vVisiblePositionBegin << ", m_vVisiblePositionEnd: " << m_vVisiblePositionEnd << ef;
				m_oInvalidatedRegions.erase(oClipIterator);
				//m_opSemaphore -> Release();
			}
		}
		//else
			//m_opSemaphore -> Release();

		m_opView -> UnlockWindow();
	}
}
/*
void EMImageBufferRepository::Unlock()
{
	m_opSemaphore -> Release();
}
*/
bool EMImageBufferRepository::VideoDataAvailable(EMGClip* p_opClip, int64 p_vBeginPosition, int64 p_vEndPosition, int32 p_vFrameHeight)
{
	m_opView -> AssertWindowLocked();

if(m_oClipToFileIDMapping.find(p_opClip) == m_oClipToFileIDMapping.end())
EMDebugger("Clip is not added to EMImageBufferRepository");
//return false;

	//m_opSemaphore -> Acquire();
	int32 vFileID = m_oClipToFileIDMapping[p_opClip];
	vector<EMImageChunk*>* opChunks = m_oChunks[vFileID];
	int vSkipRate;
	int32 vFrameWidth;
	bool vAllAreValid = true;
	GetFrameWidthAndSkipRate(vFileID, p_vFrameHeight, &vFrameWidth, &vSkipRate);
	uint32 vStartIndex;
	uint32 vEndIndex;

	if(opChunks -> size() > 1)
	{
		vStartIndex = GetVideoChunkIndex(p_vBeginPosition, vSkipRate, opChunks);
		vEndIndex = GetVideoChunkIndex(p_vEndPosition, vSkipRate, opChunks);
	}
	else // It's a picture file (one frame only)
	{
		vStartIndex = 0;
		vEndIndex = 0;
	}

	for(uint32 vIndex = vStartIndex; vIndex <= vEndIndex; vIndex += vSkipRate)
	{
		if(!(*opChunks)[vIndex] -> m_vIsValid)
		{
			//m_opSemaphore -> Release();
			return false;
		}
		// If cached frames are of an incorrect height, they are made invalid
		if((*opChunks)[vIndex] -> m_vFrameHeight != p_vFrameHeight)
		{
			EMImageChunk* opChunk = (*opChunks)[vIndex];
			DeleteChunk(opChunk);
			vAllAreValid = false;
		}
	}

	// All chunks are available, the clip can draw itself
	//m_opSemaphore -> Release();

	return vAllAreValid;
}

void EMImageBufferRepository::VideoFileAdded(int32 p_vFileID, bool p_vSemaphore)
{
	m_opView -> AssertWindowLocked();

	if(m_oMediaFileInfo.find(p_vFileID) != m_oMediaFileInfo.end()) // If already added
		return;

	//if(p_vSemaphore)
	//	m_opSemaphore -> Acquire();
	int32 vHiResChunks = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_NUM_FRAMES, &p_vFileID, &m_vFramerate)));
	EMImageChunk* opChunk;
	vector<EMImageChunk*>* opChunks = EM_new vector<EMImageChunk*>();

	for(int32 vIndex = 0; vIndex < vHiResChunks; vIndex++)
	{
		opChunk = EM_new EMImageChunk(EM_TYPE_ANY_VIDEO);
		opChunk -> m_vBeginPosition = static_cast<int64>((vIndex * EM_TICKS_PER_SECOND) / m_vFramerate);
		opChunk -> m_vEndPosition = static_cast<int64>(((vIndex + 1) * EM_TICKS_PER_SECOND) / m_vFramerate - 1); // Interesting value when caching video?
		opChunks -> push_back(opChunk);
	}

	m_oChunks[p_vFileID] = opChunks;
	EMImageFileInfo oInfo;
	oInfo.m_vFileID = p_vFileID;
	oInfo.m_eMediaType = EM_TYPE_ANY_VIDEO;
	oInfo.m_vChunkSize = 1;

	if(vHiResChunks == 1)
		oInfo.m_vFileLength = INT64_MAX;
	else
		oInfo.m_vFileLength = static_cast<int64>((vHiResChunks * EM_TICKS_PER_SECOND) / m_vFramerate);

	oInfo.m_vNrChunks = vHiResChunks;
	oInfo.m_vAspectRatio = *(static_cast<float*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ASPECT_RATIO, &p_vFileID)));
	m_oMediaFileInfo[p_vFileID] = oInfo;
	//if(p_vSemaphore)
	//	m_opSemaphore -> Release();
}

// Assumes that all clips connected to this file are removed before this method is called
void EMImageBufferRepository::VideoFileRemoved(int32 p_vFileID, bool p_vSemaphore)
{
	m_opView -> AssertWindowLocked();

	//if(p_vSemaphore)
	//	m_opSemaphore -> Acquire();
	FileRemoved(p_vFileID);
	//if(p_vSemaphore)
	//	m_opSemaphore -> Release();
}





