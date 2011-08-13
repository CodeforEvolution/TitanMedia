/*******************************************************
* Creator: Richard Kronfält
* Portability: Non-Native
*-------------------------------------------------------
* 
* This class has two main purposes: 
* 1. 	Store all the tracks, and supply methods for accessing them, and
* 2.	Collect and process the buffers in real-time during playback and route them to the processing
*		nodes in the system.
*
* The second task of this class is accomplished through asking the clip repositories for the "next" set
* of buffers for a certain type (e.g. EM_TYPE_RAW_VIDEO) and then processes the resulting list of buffers.
* The list of buffers will, for instance, be 5 buffers long, if there are 5 video clips active at "this" 
* particular moment in songtime.
*
* By the way, the buffers aren't BBuffers. They're container class (EMMediaDataBuffer) instances that 
* encapsulate a char-array, size/used variables, type-coding, format and some other stuff.
* They're not converted into BBuffers until they reach the physical output's processing method.
*
*
* ROUTING OF BUFFERS 
* The routing of buffers into the different nodes of processors (effect-slots, physical outputs, clip-mixers,
* track-mixers, etc) is accomplished through 1 basic thing:
*
* >>>> Each EMMediaDataBuffer knows it's next destination! <<<<
*
* A destination is a base-type which contains certain methods, such as SetDestination, GetDestination, and of 
* course ProcessBuffer(list<...>...);
* The ProcessBuffer method is given a list of buffers (not BBuffers) which it does something with. It is 
* expected to either (A) return NULL (in which case it should Recycle() each of the EMMediaDataBuffers it received), or 
* (B) return one EMMediaDataBuffer (one of the received  ones), in which case it must set that buffer's NEXT
* destination. If it doesn't update the destination of the buffer-to-return you'll have an infinite loop on 
* your hands. 
*
* NOTE: Recycle must always be called on ALL EMMediaDataBuffers at some time, just like when using 
* Be's native BBuffer-class.
*
*
* ADDITIONAL DUTIES
* In addition to routing buffers, this class' responsibility is also too produce silence, and send it to each of the
* destinations, if there were no buffers at "this point in songtime". 
* Further, this class must ask for video buffers more frequently than it asks for audio buffers, since one
* audio buffer is about 92.8 ms long (in 44k1hz, 2 channel, 16 bits, 16384 bytes per buffer) and contains perhaps
* 4096 frames, while a video buffer contains one frame out of 25 per second, which equals 40 ms.
*
* Phew... That's about it.. :-)
*
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_PROJECT
#define __EM_MEDIA_PROJECT

#include "EMInputRepository.h"
#include "EMListener.h"
#include "EMMediaGlobals.h"
#include "EMMediaItemContainer.h"
#include "EMMediaTrack.h"
#include "EMOutputRepository.h"
#include "EMSilentVideoSource.h"
#include "EMThreadListener.h"

#include <list>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <process.h>
#endif

class EMListenerRepository;
class EMMediaClipRepository;
class EMMediaEffectTrackRepository;
class EMMediaMatrix;
class EMMediaMIDIEventRepository;
class EMMediaPool;
class EMMediaTrackRepository;
class EMMediaTransitionTrackRepository;
class EMProjectDataLoader;
class EMProjectDataSaver;
class EMRecordingMIDITrackRepository;
class EMSemaphore;
class EMSilentAudioSource;
class EMThread;

class EMMediaProject : public EMListener, public EMThreadListener
{
public:
	EMMediaProject();
	virtual ~EMMediaProject();
	bool ClearThreads();
	EMMediaClipRepository* GetAudioClipRepository() const;
	int32 GetID() const;
	EMMediaPool* GetMediaPool() const;
	EMMediaClipRepository* GetMIDIClipRepository() const;
	EMMediaMIDIEventRepository* GetMIDIEventRepository() const;
	int64 GetProjectEndTimeInFrames();
	EMRecordingMIDITrackRepository* GetRecordingMIDITracks() const;
	EMMediaClipRepository* GetVideoClipRepository() const;
	EMMediaClipRepository* GetVideoTransitionClipRepository() const;
	EMMediaTransitionTrackRepository* GetVideoTransitionTrackRepository() const;
	EMMediaTrackRepository* GetUnusedTracks() const;
	EMMediaTrackRepository* GetUsedTracks() const;
	bool InitCheckE();
	bool IsArmed() const;
	bool IsDirty();
	bool IsPlaying() const;
	bool IsRenderingAudio() const;
	bool IsRenderingVideo() const;
	bool LoadData(EMProjectDataLoader* p_upLoader);
	static bool LockAudio();
	static bool LockVideo();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void OnThreadCreated(EMThread* p_opThread);
	void OnThreadKilled(EMThread* p_opThread);
	void PlaySingleBuffer();
	bool PrepareToPlayE();
	void RunAudio();
	void RunVideo();
	bool SaveData(EMProjectDataSaver* p_upSaver);
	void SeekAndDisplay(int64 p_vNow);
	void SetArmed(bool p_vFlag);
	void SetDirty(bool p_vFlag);
	void SetPreview();	
	void SetRenderingAudio(bool p_vFlag);
	void SetRenderingVideo(bool p_vFlag);
	void SetVideoWorkingSize(int32 p_vWidth, int32 p_vHeight);
	void ShouldProduceAudio(bool p_vShouldproduceAudio, bool p_vEmergencyStop = false);
	void ShouldProduceVideo(bool p_vShouldproduceVideo, bool p_vEmergencyStop = false);
	void StartPlaybackE();
	void StartRecordingE();
	void StopPlaybackE();
	void StopRecordingE();
	void StopVideoAudioRenderingAtOnce();
	void ThreadRun(EMThread* p_opThread);
	static void UnlockAudio();
	static void UnlockVideo();

private:
	list<EMMediaDataBuffer* >* GetLeastReferencedDestination(list<list<EMMediaDataBuffer* >* >* p_opSets);
	EMMediaDataBuffer** GetLeastReferencedDestination(EMMediaDataBuffer* (*p_opDestinations)[64]);
	list<EMMediaDataBuffer*>* GetList();
	EMMediaTrack* GetNextTrack(EMMediaTrack* p_opTrack);
	EMSilentVideoSource* GrabAnySilenceGenerator();
	void MergeList(list<EMMediaBufferSource*>*opClipList, list<EMMediaClip*>*opVideoList, list<EMMediaClip*>*opTransList);
	void PrepareLists();
	void RecycleList(list<EMMediaDataBuffer*>* p_opList);
	void RunBuffer(EMMediaDataBuffer** p_opBuffer, bool p_vIgnoreTransition = false);
	void SplitLists(list<EMMediaDataBuffer*>* p_opBufferList, list<list<EMMediaDataBuffer*>*>* p_opSets);

	int32 m_vID;
	bool m_vTimeToShutdown;
	bool m_vShouldProduceAudio;
	bool m_vShouldProduceVideo;
	bool m_vAudioOn;
	bool m_vVideoOn;
	bool m_vMidiOn;
	EMMediaClipRepository* m_opAudioClipRepository;
	EMMediaClipRepository* m_opVideoClipRepository;
	EMMediaClipRepository* m_opVideoTransitionClipRepository;
	EMMediaClipRepository* m_opMIDIClipRepository;
	EMMediaEffectTrackRepository* m_opEffectTrackRepository;
	EMMediaTransitionTrackRepository* m_opVideoTransitionTrackRepository;
	EMMediaPool* m_opMediaPool;
	bool m_vStarted;
	bool m_vStopped;
	int32 m_vProcessSingleAudioBuffer;
	int32 m_vProcessSingleVideoBuffer;
	EMSemaphore* m_opSemaphore;
	EMMediaTrackRepository* m_opUsedTracks;
	EMMediaTrackRepository* m_opUnusedTracks;
	int64 m_vNumAudioSlots;
	int64 m_vNumVideoSlots;
	list<EMMediaDataBuffer*>** m_opPreAllocatedDestinationLists;
	bool m_vIsArmed;
	bool m_vIsRenderingAudio;
	bool m_vIsRenderingVideo;
	float m_vVideoWorkingFrameRate;
	static EMSemaphore* m_opAudioProcessingSemaphore;
	static EMSemaphore* m_opVideoProcessingSemaphore;
	bool vFrameRateChangedSwitch;
	bool m_vAudioThreadDead;
	bool m_vVideoThreadDead;
	EMMediaMIDIEventRepository* m_opMIDIEventRepository;
	EMRecordingMIDITrackRepository* m_opRecordingMIDITrackRepository;
	EMThread* m_opAudioThread;
	EMThread* m_opVideoThread;
	bool m_vAudioIsActive;
	bool m_vVideoIsActive;
	EMMediaDataBuffer* m_opAudioBufferList[64];
	EMMediaDataBuffer* m_opAudioDestinations[64][64];
	bool m_vIsDirty;
	uint32 m_vProjectBPM;
	string m_oProjectSignature;
	int64 m_vCurrentLatestEnding;
};

#endif