#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinAudioClipRepository.h"
#include "EMFileInputDescriptor.h"
#include "EMMediaIDManager.h"
//#include "EMBeMediaUtility.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMMediaUtility.h"
#include <algorithm>

EMWinAudioClipRepository::EMWinAudioClipRepository()
	:	EMMediaClipRepository(EM_TYPE_ANY_AUDIO),
		m_opSystemAudioFormat(NULL),
		m_vBPM(120)
{
	m_vID = EMMediaIDManager::MakeID();
	//EMBeMediaUtility::push(this, "EMWinAudioClipRepository");
}

EMWinAudioClipRepository::~EMWinAudioClipRepository() //Deleted by MediaProject
{
	delete m_opSystemAudioFormat; 
	//EMBeMediaUtility::pop("EMWinAudioClipRepository");
}

int32 EMWinAudioClipRepository::GetID() const
{
	return m_vID;
}

bool EMWinAudioClipRepository::IsSoloActivated()
{
	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		if(Current() -> GetTrack() == NULL)
			EMDebugger("ERROR! Track was NULL in some clip!");
		else if(Current() -> GetTrack() -> IsSoloed())
		{
			UnlockContainer();
			return true;
		}
		Next();
	}
	UnlockContainer();
	return false;
}

//Dont call from within a Rewind()/Current()/Next() loop, or things will go badly!
int64 EMWinAudioClipRepository::FramesToNextClip(int64 p_vFromFrame)
{

	//TODO: Handle loop-points. We have to make sure we can offset a buffer 
	//even if the "next" buffer should lie just in the beginning of a loop, and
	//we're currently at the very end of the looped region.

	int64 vDistance = -1;
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if(Current() -> GetStart() >= p_vFromFrame && Current() && 
				! Current() -> IsObjectDeleted()) //Make sure we don't calculate on deleted-but-unpurged clips
			{
				if(vDistance == -1 || Current() -> GetStart() - p_vFromFrame < vDistance)
					vDistance = Current() -> GetStart() - p_vFromFrame;
			}
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMWinAudioClipRepository::FramesToNextClip");
	}
	UnlockContainer();
	return (vDistance == -1 ? static_cast<int64>(static_cast<uint64>(-1)) : vDistance);
}


uint32 EMWinAudioClipRepository::GetNextBuffers(EMMediaDataBuffer** p_opArray, EMMediaType p_eType, int64 p_vTimeNow, bool p_vSeeking)
{
	uint32 vArrayPos = 0;
	int64 vOffset = 0;
	int64 vNumBuffers = 0;
	int64 vNow = p_vTimeNow;
	int64 vFramesToNextClip = FramesToNextClip(vNow);
	bool vSoloMode = IsSoloActivated();
	list<int32> oID;

	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if((Current() -> GetType() & p_eType) > 0 && 
				! Current() -> IsObjectDeleted()) //Make sure we don't process deleted-but-unpurged clips
			{
				//Make sure we don't play muted tracks and only play from soloed tracks if there's at least one soloed track in the repository!
				EMMediaClip* opCurrentClip = Current();
				EMMediaTrack* opCurrentTrack = opCurrentClip -> GetTrack();

				if((! vSoloMode && opCurrentTrack -> GetMuteState() == EM_MUTE_OFF) ||
					(vSoloMode && opCurrentTrack -> IsSoloed()))
				{
					vOffset = 0;
					int64 vCurrentStartFrame = opCurrentClip -> GetStart();
					int64 vCurrentDuration = opCurrentClip -> GetActiveLength();

					int64 vFramesPerBuffer = 0;

					//if there's less than a whole buffer's duration to the next clip start, measured from "now"
					if(! (vCurrentStartFrame <= vNow && vCurrentStartFrame + vCurrentDuration >= vNow) &&
						vFramesToNextClip < vFramesPerBuffer && vFramesToNextClip != 0)
						vOffset = vFramesToNextClip;

					//If "this" clip intersects the current frame
					if(vCurrentStartFrame <= vNow + vOffset &&
						vCurrentStartFrame + vCurrentDuration >= vNow)
					{
						EMMediaDataBuffer* opMediaBuffer = NULL;
						
						/*while((opMediaBuffer = opCurrentClip -> GetBuffer()) == NULL)
							::Sleep(10);*/

						opMediaBuffer = opCurrentClip -> GetBuffer();
						if(opMediaBuffer == NULL)
						{
							//EMBeMediaUtility::show();
							//eo << "ERROR! Couldn't get a buffer from the clip!" << ef;
						}
						else
						{
							opMediaBuffer -> SetFrame(EMMediaTimer::Instance() -> AudioThenFrame());

							int64 vIONumFrames = vFramesPerBuffer;
							if(vNow + vIONumFrames > vCurrentStartFrame + vCurrentDuration)
							{
								//if one buffer of data from this files will end up beoynd the Mark-out point!
								vIONumFrames = (vCurrentStartFrame + vCurrentDuration) - vNow;
							}
							
							//To prevent the processing to go beyond the loop-end point, either during looping or during rendering
		//					if(EMMediaTimer::Instance() -> IsLooped() && vNow + vIONumFrames > EMMediaTimer::Instance() -> GetLoopEnd())
		//						vIONumFrames = vIONumFrames - ((vNow + vIONumFrames) - EMMediaTimer::Instance() -> GetLoopEnd());

							int64 vFramePosition = 0;
							if(vNow > opCurrentClip -> GetMediaStart())
								vFramePosition = vNow - opCurrentClip -> GetMediaStart(); //Make sure we get the file-orientation corrent (discard MarkIN values here!
							EMMediaDataBuffer* opEmptyBufferPointer = NULL;
							bool vResult = opCurrentClip -> GetDescriptor() -> ReadFrames(&opMediaBuffer, &opEmptyBufferPointer, vFramePosition, vOffset, vIONumFrames);
							vFramePosition = 0;
							if(vResult)
							{
								opMediaBuffer -> m_vSizeUsed = opMediaBuffer -> m_vSizeAvailable;
								if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
								{
									int64 vPos = EMMediaUtility::Instance() -> FramesToTime(p_vTimeNow, EMMediaUtility::Instance() -> GetSystemAudioFormat());
									int32 vVolumeValue = opCurrentTrack->GetRubberBandValue(vPos, 1);
									int32 vPanValue = opCurrentTrack->GetRubberBandValue(vPos, 2);
									float vVolumeMultiplier = 1.0;
									if(vVolumeValue != -1)
										vVolumeMultiplier = static_cast<float>(vVolumeValue / 127.0);
									if(vPanValue == -1)
										vPanValue = opCurrentTrack -> GetPan();
									
									opMediaBuffer -> m_vFader = static_cast<long>((float)opCurrentTrack -> GetFader() * vVolumeMultiplier);

									opMediaBuffer -> m_vPan = static_cast<long>(vPanValue);

									oID.push_back(opCurrentTrack -> GetID());
								}

		//						if(EMMediaTimer::Instance() -> ThenFrame() == opMediaBuffer -> m_vFrame)
		//						{
									p_opArray[vArrayPos++] = opMediaBuffer;
		//							int64 val = EMBeMediaUtility::FindMaxNum(static_cast<signed short*>(opMediaBuffer -> Data()), opMediaBuffer -> m_vSizeUsed / (EM_AUDIO_NUM_CHANNELS * EM_AUDIO_SAMPLESIZE), 1);
		//							EMSignalMonitor::Instance() -> PushBack(val, EMMediaTimer::Instance() -> ThenFrame());
									vNumBuffers++;
		//							if((opCurrentTrack -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
		//								;//cout_commented_out_4_release << "Pushed back the ID (" << opCurrentTrack -> GetID() << " of a video track!" << endl;
		//							;//cout_commented_out_4_release << "Pushing back a buffer from track " << opCurrentTrack -> GetID() << ". It's destination is " << opMediaBuffer -> m_opDestination -> GetID() << endl;
		//						}
		//						else
		//							opMediaBuffer -> Recycle();
							}
							else
							{
								opMediaBuffer -> Recycle();
							}
						}
					}
				}
			}

			Next();
		}
	}
	catch(...)
	{
		//EMDebugger("ERROR! Exception in EMWinAudioClipRepository::GetNextBuffers");
		eo << "WARNING! Exception caught in GetNextBuffers in audio clip repository (1)!" << ef;
	}
	UnlockContainer();

	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaTrackRepository* opTracks = opProject -> GetUsedTracks();

	opTracks -> LockContainer();
	try
	{
		opTracks -> Rewind();
		
		opProject -> GetUnusedTracks() -> LockContainer();
		opProject -> GetUnusedTracks() -> Rewind();
		
		EMMediaTrack* opCurrentTrack = opTracks -> Current();
		
		if(opCurrentTrack == NULL)
			opCurrentTrack = opProject -> GetUnusedTracks() -> Current();
		
		while(opCurrentTrack != NULL)
		{	
			if(((find(oID.begin(), oID.end(), opCurrentTrack -> GetID()) == oID.end() && 
				(opCurrentTrack -> GetType() & p_eType) > 0)) ||
				(oID.size() == 0 && (opCurrentTrack -> GetType() & p_eType) > 0))
			{
				if(opCurrentTrack == NULL)
					EMDebugger("ERROR! Current track in opTracks seems to be NULL, in ClipRepository::GetBuffers");
				EMMediaBufferSource* opSilentBufferSource = opCurrentTrack -> GetSilenceGenerator();
				if(opSilentBufferSource == NULL)
					EMDebugger("ERROR! Silence generator seems to be NULL, in ClipRepository::GetBuffers");
				
				EMMediaDataBuffer* opBuffer = opSilentBufferSource -> GetBuffer();
				if(opBuffer == NULL)
					EMDebugger("ERROR! Buffer returned from silence generator seems to be NULL, in ClipRepository::GetBuffers");
				
				opBuffer -> SetFrame(EMMediaTimer::Instance() -> AudioThenFrame());
				p_opArray[vArrayPos++] = opBuffer;
				vNumBuffers++;
			}
			opTracks -> Next();
			opCurrentTrack = opTracks -> Current();
		}
	}
	catch(...)
	{
		//EMDebugger("ERROR! Exception in EMWinAudioClipRepository::GetNextBuffers");
		eo << "WARNING! Exception caught in GetNextBuffers in audio clip repository (2)!" << ef;
	}
	opTracks -> UnlockContainer();
	opProject -> GetUnusedTracks() -> UnlockContainer();
	p_opArray[vArrayPos] = NULL;
	return vArrayPos;
}

void EMWinAudioClipRepository::GetNextBuffers(/*EMMediaTrack* p_opForTrack,*/ list<EMMediaDataBuffer*>* p_opList, EMMediaType p_eType, int64 p_vTimeNow, bool)
{
//	Lock();
	
	int64 vOffset = 0;
	int64 vNumBuffers = 0;
	int64 vNow = p_vTimeNow;
	int64 vFramesToNextClip = FramesToNextClip(vNow);
	bool vSoloMode = IsSoloActivated();
	list<int32> oID;

	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if((Current() -> GetType() & p_eType) > 0 && 
				! Current() -> IsObjectDeleted()) //Make sure we don't process deleted-but-unpurged clips
			{
				//Make sure we don't play muted tracks and only play from soloed tracks if there's at least one soloed track in the repository!
				EMMediaClip* opCurrentClip = Current();
				EMMediaTrack* opCurrentTrack = opCurrentClip -> GetTrack();

				if((! vSoloMode && opCurrentTrack -> GetMuteState() == EM_MUTE_OFF) ||
					(vSoloMode && opCurrentTrack -> IsSoloed()))
				{
					vOffset = 0;
					int64 vCurrentStartFrame = opCurrentClip -> GetStart();
					int64 vCurrentDuration = opCurrentClip -> GetActiveLength();

					int64 vFramesPerBuffer = 0;

					//if there's less than a whole buffer's duration to the next clip start, measured from "now"
					if(! (vCurrentStartFrame <= vNow && vCurrentStartFrame + vCurrentDuration >= vNow) &&
						vFramesToNextClip < vFramesPerBuffer && vFramesToNextClip != 0)
						vOffset = vFramesToNextClip;

					//If "this" clip intersects the current frame
					if(vCurrentStartFrame <= vNow + vOffset &&
						vCurrentStartFrame + vCurrentDuration >= vNow)
					{
						EMMediaDataBuffer* opMediaBuffer = NULL;
						
						/*while((opMediaBuffer = opCurrentClip -> GetBuffer()) == NULL)
							::Sleep(10);*/

						opMediaBuffer = opCurrentClip -> GetBuffer();
						if(opMediaBuffer == NULL)
						{
							//EMBeMediaUtility::show();
							//eo << "ERROR! Couldn't get a buffer from the clip!" << ef;
						}
						else
						{
							opMediaBuffer -> SetFrame(EMMediaTimer::Instance() -> AudioThenFrame());

							int64 vIONumFrames = vFramesPerBuffer;
							if(vNow + vIONumFrames > vCurrentStartFrame + vCurrentDuration)
							{
								//if one buffer of data from this files will end up beoynd the Mark-out point!
								vIONumFrames = (vCurrentStartFrame + vCurrentDuration) - vNow;
							}
							
							//To prevent the processing to go beyond the loop-end point, either during looping or during rendering
		//					if(EMMediaTimer::Instance() -> IsLooped() && vNow + vIONumFrames > EMMediaTimer::Instance() -> GetLoopEnd())
		//						vIONumFrames = vIONumFrames - ((vNow + vIONumFrames) - EMMediaTimer::Instance() -> GetLoopEnd());

							int64 vFramePosition = 0;
							if(vNow > opCurrentClip -> GetMediaStart())
								vFramePosition = vNow - opCurrentClip -> GetMediaStart(); //Make sure we get the file-orientation corrent (discard MarkIN values here!
							EMMediaDataBuffer* opEmptyBufferPointer = NULL;
							bool vResult = opCurrentClip -> GetDescriptor() -> ReadFrames(&opMediaBuffer, &opEmptyBufferPointer, vFramePosition, vOffset, vIONumFrames);
							vFramePosition = 0;
							if(vResult)
							{
								opMediaBuffer -> m_vSizeUsed = opMediaBuffer -> m_vSizeAvailable;
								if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
								{
									int64 vPos = EMMediaUtility::Instance() -> FramesToTime(p_vTimeNow, EMMediaUtility::Instance() -> GetSystemAudioFormat());
									int32 vVolumeValue = opCurrentTrack->GetRubberBandValue(vPos, 1);
									float vVolumeMultiplier = 1.0;
									if(vVolumeValue != -1)
										vVolumeMultiplier = static_cast<float>(vVolumeValue / 127.0);
									
									opMediaBuffer -> m_vFader = static_cast<long>((float)opCurrentTrack -> GetFader() * vVolumeMultiplier);
									opMediaBuffer -> m_vPan = opCurrentTrack -> GetPan();
									oID.push_back(opCurrentTrack -> GetID());
								}

		//						if(EMMediaTimer::Instance() -> ThenFrame() == opMediaBuffer -> m_vFrame)
		//						{
									p_opList -> push_back(opMediaBuffer);
		//							int64 val = EMBeMediaUtility::FindMaxNum(static_cast<signed short*>(opMediaBuffer -> Data()), opMediaBuffer -> m_vSizeUsed / (EM_AUDIO_NUM_CHANNELS * EM_AUDIO_SAMPLESIZE), 1);
		//							EMSignalMonitor::Instance() -> PushBack(val, EMMediaTimer::Instance() -> ThenFrame());
									vNumBuffers++;
		//							if((opCurrentTrack -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
		//								;//cout_commented_out_4_release << "Pushed back the ID (" << opCurrentTrack -> GetID() << " of a video track!" << endl;
		//							;//cout_commented_out_4_release << "Pushing back a buffer from track " << opCurrentTrack -> GetID() << ". It's destination is " << opMediaBuffer -> m_opDestination -> GetID() << endl;
		//						}
		//						else
		//							opMediaBuffer -> Recycle();
							}
							else
							{
								opMediaBuffer -> Recycle();
							}
						}
					}
				}
			}

			Next();
		}
	}
	catch(...)
	{
		//EMDebugger("ERROR! Exception in EMWinAudioClipRepository::GetNextBuffers");
		eo << "WARNING! Exception caught in GetNextBuffers in audio clip repository (1)!" << ef;
	}
	UnlockContainer();

	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaTrackRepository* opTracks = opProject -> GetUsedTracks();

	opTracks -> LockContainer();
	try
	{
		opTracks -> Rewind();
		
		opProject -> GetUnusedTracks() -> LockContainer();
		opProject -> GetUnusedTracks() -> Rewind();
		
		EMMediaTrack* opCurrentTrack = opTracks -> Current();
		
		if(opCurrentTrack == NULL)
			opCurrentTrack = opProject -> GetUnusedTracks() -> Current();
		
		while(opCurrentTrack != NULL)
		{	
			if(((find(oID.begin(), oID.end(), opCurrentTrack -> GetID()) == oID.end() && 
				(opCurrentTrack -> GetType() & p_eType) > 0)) ||
				(oID.size() == 0 && (opCurrentTrack -> GetType() & p_eType) > 0))
			{
				if(opCurrentTrack == NULL)
					EMDebugger("ERROR! Current track in opTracks seems to be NULL, in ClipRepository::GetBuffers");
				EMMediaBufferSource* opSilentBufferSource = opCurrentTrack -> GetSilenceGenerator();
				if(opSilentBufferSource == NULL)
					EMDebugger("ERROR! Silence generator seems to be NULL, in ClipRepository::GetBuffers");
				
				EMMediaDataBuffer* opBuffer = opSilentBufferSource -> GetBuffer();
				if(opBuffer == NULL)
					EMDebugger("ERROR! Buffer returned from silence generator seems to be NULL, in ClipRepository::GetBuffers");
				
				opBuffer -> SetFrame(EMMediaTimer::Instance() -> AudioThenFrame());
				p_opList -> push_back(opBuffer);
				vNumBuffers++;
			}
			opTracks -> Next();
			opCurrentTrack = opTracks -> Current();
		}
	}
	catch(...)
	{
		//EMDebugger("ERROR! Exception in EMWinAudioClipRepository::GetNextBuffers");
		eo << "WARNING! Exception caught in GetNextBuffers in audio clip repository (2)!" << ef;
	}
	opTracks -> UnlockContainer();
	opProject -> GetUnusedTracks() -> UnlockContainer();

}

bool EMWinAudioClipRepository::InitCheckE()
{
	if(m_opSystemAudioFormat == NULL)
		m_opSystemAudioFormat = EM_new EMMediaFormat(EM_TYPE_RAW_AUDIO);
	return true;
}

void EMWinAudioClipRepository::AdjustToBPM(uint32 p_vBPM)
{
	if(! *(static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_LOCKED_TO_TEMPO))))
		return;

	float vOldBPM = static_cast<float>(m_vBPM);
	float vNewBPM = static_cast<float>(p_vBPM);

	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		Current() -> SetStart(static_cast<int64>(static_cast<float>(Current() -> GetStart()) * vOldBPM / vNewBPM));
		Next();
	}
	UnlockContainer();
}

#endif
