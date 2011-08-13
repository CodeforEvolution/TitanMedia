#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinVideoClipRepository.h"
#include "EMFileInputDescriptor.h"
#include "EMMediaIDManager.h"
#include "EMMediaUtility.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMWinVideoTransitionClipRepository.h"

#include <algorithm>

EMWinVideoClipRepository::EMWinVideoClipRepository()
	:	EMMediaClipRepository(EM_TYPE_ANY_GFX), m_opPreviousClip(NULL)
{
	m_vID = EMMediaIDManager::MakeID();
}

EMWinVideoClipRepository::~EMWinVideoClipRepository() //Deleted by MediaProject
{
}

int32 EMWinVideoClipRepository::GetID() const
{
	return m_vID;
}

bool EMWinVideoClipRepository::IsSoloActivated()
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
int64 EMWinVideoClipRepository::FramesToNextClip(int64 p_vFromFrame)
{

	//TODO: Handle loop-points. We have to make sure we can offset a buffer 
	//even if the "next" buffer should lie just in the beginning of a loop, and
	//we're currently at the very end of the looped region.

	int64 vDistance = -1;
	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		if(Current() -> GetStart() >= p_vFromFrame && Current())
		{
			if(vDistance == -1 || Current() -> GetStart() - p_vFromFrame < vDistance)
				vDistance = Current() -> GetStart() - p_vFromFrame;
		}
		Next();
	}
	UnlockContainer();
	return (vDistance == -1 ? static_cast<int64>(static_cast<uint64>(-1)) : vDistance);
}

bool EMWinVideoClipRepository::GetClipsInTimeslot(list<EMMediaClip*>* p_opList, int64 p_vTime)
{
	p_opList -> clear();

	LockContainer();
	Rewind();

	while(Current() != NULL)
	{
		if(static_cast<EMVideoClip*>(Current())->IsActiveNow(p_vTime))
		{
			if(Current() -> GetTrack() -> IsSoloed())
			{
				p_opList -> clear();
				p_opList -> push_front(static_cast<EMVideoClip*>(Current()));
				break;
			}
			else if(Current() -> GetTrack() -> GetMuteState() == EM_MUTE_OFF)
			{
				int32 vPrio = Current() -> GetTrack() -> GetPriority();
	
				list<EMMediaClip*>::iterator i;			

				for(i = p_opList -> begin(); i != p_opList -> end(); ++i)
				{
					int32 vPrio2 = (*i) -> GetTrack() -> GetPriority();

					if(vPrio2 > vPrio)
						break;		
				}
				p_opList -> insert(i, Current());
			}

		} 
		else
		{
			Current() -> m_vShouldSeek = true;
		}
		Next();
	}

	UnlockContainer();

	return p_opList -> size() != 0;	
}

void EMWinVideoClipRepository::GetNextBuffers(list<EMMediaDataBuffer*>* p_opList, EMMediaType p_eType, int64 p_vTimeNow, bool p_vSeeking)
{
//	Lock();
	//;//cout_commented_out_4_release<<"I'm IN EMWinVideoClipRepository"<<endl;	
	int64 vOffset = 0;
	int64 vNumBuffers = 0;
	int64 vNow = p_vTimeNow;
	int64 vFramesToNextClip = FramesToNextClip(vNow);
	bool vSoloMode = IsSoloActivated();
	list<int32> oID;

	bool vMartin = true;

	if(vMartin)
	{
/*		list<EMVideoClip*> opList;
		if(GetClipsInTimeslot(&opList,p_vTimeNow))
		{
			EMMediaClip* opCurrentClip = opList.front();

			int64 vIONumFrames = 1;
			int64 vFramePosition = 0;
			if(p_vTimeNow > opCurrentClip -> GetMediaStart())
				vFramePosition = p_vTimeNow - opCurrentClip -> GetMediaStart(); //Make sure we get the file-orientation corrent (discard MarkIN values here!

			if (m_opPreviousClip != opCurrentClip)
			{
				p_vSeeking = true;
			}


			EMMediaDataBuffer* opMediaBuffer = opCurrentClip -> GetBuffer();																
			if(opMediaBuffer == NULL)
			{
				return;
			}
			EMMediaDataBuffer* opSecondaryBuffer = opCurrentClip -> GetBuffer();
			//Because the videoclips share buffers within the same track, a "clip-change" will have to recycle all buffers for reuse for the new clip
			if(opSecondaryBuffer == NULL && m_opPreviousClip != NULL && m_opPreviousClip != opCurrentClip)
			{
				opCurrentClip -> RecycleAllBuffers();
				opMediaBuffer = opCurrentClip -> GetBuffer();
				opSecondaryBuffer = opCurrentClip -> GetBuffer();
			}

			m_opPreviousClip = opCurrentClip;

			opCurrentClip -> GetDescriptor() -> ReadFrames(&opMediaBuffer, &opSecondaryBuffer, vFramePosition, vOffset, vIONumFrames, p_vSeeking);
			opMediaBuffer -> m_vSizeUsed = opMediaBuffer -> m_vSizeAvailable;

			if(opMediaBuffer == NULL)
			{
				return;
			}


			p_opList -> push_back(opMediaBuffer);
		}
		else
		{
			LockContainer();
			Rewind();

			EMMediaBufferSource* opSilentBufferSource = Current()->GetTrack()->GetSilenceGenerator();
			EMMediaDataBuffer* opBuffer = opSilentBufferSource -> GetBuffer();
			
			if(opBuffer == NULL)
				return;

			opBuffer -> m_vSizeUsed = opBuffer -> m_vSizeAvailable;
			p_opList -> push_back(opBuffer);
			UnlockContainer();
		}	*/
	}


	if(!vMartin)
	{
		LockContainer();
		Rewind();
		while(Current() != NULL)
		{
			if((Current() -> GetType() & p_eType) > 0 )
			{
				//Make sure we don't play muted tracks and only play from soloed tracks if there's at least one soloed track in the repository!
				EMMediaClip* opCurrentClip = Current();
				EMMediaTrack* opCurrentTrack = opCurrentClip -> GetTrack();
/*				if((p_eType & EM_TYPE_ANY_VIDEO) > 0 && ! vSoloMode && opCurrentTrack -> GetMuteState() == EM_MUTE_OFF)
				{
						opCurrentClip = GetPriorityClip(vNow, vFramesToNextClip, p_opList, &oID);
						if(opCurrentClip == NULL)
							break;
						opCurrentTrack = opCurrentClip -> GetTrack();
				}*/
			
			
				if((! vSoloMode && opCurrentTrack -> GetMuteState() == EM_MUTE_OFF) ||
					(vSoloMode && opCurrentTrack -> IsSoloed()))
				{
					vOffset = 0;
					int64 vCurrentStartFrame = opCurrentClip -> GetStart();
					int64 vCurrentDuration = opCurrentClip -> GetActiveLength();
					EMMediaDataBuffer* opMediaBuffer = opCurrentClip -> GetBuffer();
	//				opMediaBuffer -> SetFrame(EMMediaTimer::Instance() -> VideoThenFrame()); //p_vTimeNow;
					if(opMediaBuffer == NULL)
						EMDebugger("EMMediaDataBuffer is NULL!");
					
					int64 vFramesPerBuffer = EMMediaUtility::Instance() -> BytesToFrames(opMediaBuffer -> m_vSizeAvailable, &(opMediaBuffer -> m_oFormat));
			
					//if there's less than a whole buffer's duration to the next clip start, measured from "now"
					if(! (vCurrentStartFrame <= vNow && vCurrentStartFrame + vCurrentDuration >= vNow) &&
						vFramesToNextClip < vFramesPerBuffer && vFramesToNextClip != 0)
						vOffset = vFramesToNextClip;
			
			
					//If "this" clip intersects the current frame
					if(vCurrentStartFrame <= vNow + vOffset &&
						vCurrentStartFrame + vCurrentDuration >= vNow)	
					{
						int64 vIONumFrames = vFramesPerBuffer;
						if(vNow + vIONumFrames > vCurrentStartFrame + vCurrentDuration)
						{
							//if one buffer of data from this files will end up beoynd the Mark-out point!
							vIONumFrames = (vCurrentStartFrame + vCurrentDuration) - vNow;
						}
						int64 vFramePosition = 0;
						if(vNow > opCurrentClip -> GetMediaStart())
							vFramePosition = vNow - opCurrentClip -> GetMediaStart(); //Make sure we get the file-orientation corrent (discard MarkIN values here!

																
						EMMediaDataBuffer* opSecondaryBuffer = opCurrentClip -> GetBuffer();
						//Because the videoclips share buffers within the same track, a "clip-change" will have to recycle all buffers for reuse for the new clip
						if(opSecondaryBuffer == NULL && m_opPreviousClip != NULL && m_opPreviousClip != opCurrentClip)
						{
							opCurrentClip -> RecycleAllBuffers();
							opMediaBuffer = opCurrentClip -> GetBuffer();
							opSecondaryBuffer = opCurrentClip -> GetBuffer();
						}

						m_opPreviousClip = opCurrentClip;


						bool vResult = opCurrentClip -> GetDescriptor() -> ReadFrames(&opMediaBuffer, &opSecondaryBuffer, vFramePosition, vOffset, vIONumFrames, p_vSeeking);
						vFramePosition = 0;
						if(vResult)
						{
							opMediaBuffer -> m_vSizeUsed = opMediaBuffer -> m_vSizeAvailable;
							if((p_eType & EM_TYPE_ANY_VIDEO) > 0)
							{
								opMediaBuffer -> m_vPriority = opCurrentClip -> GetTrack() -> GetPriority();
								opMediaBuffer -> m_vFader = opCurrentTrack -> GetFader();
								opMediaBuffer -> m_vPan = opCurrentTrack -> GetPan();
							}
							oID.push_back(opCurrentTrack -> GetID());
						
//						if(EMMediaTimer::Instance() -> ThenFrame() == opMediaBuffer -> m_vFrame)
//						{
							p_opList -> push_back(opMediaBuffer);
//							EMSignalMonitor::Instance() -> PushBack(val, EMMediaTimer::Instance() -> ThenFrame());
							vNumBuffers++;
//							if((opCurrentTrack -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
//								;//cout_commented_out_4_release << "Pushed back the ID (" << opCurrentTrack -> GetID() << " of a video track!" << endl;
//						}
//						else
//							opMediaBuffer -> Recycle();
						}
						else
							opMediaBuffer -> Recycle();
					}
					else
						opMediaBuffer -> Recycle();
				}
			}
			if(Current() != NULL)
				Next();
		}

		UnlockContainer();
	} //if(0)
	
	if(!vMartin)
	{
		EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
		EMMediaTrackRepository* opTracks = opProject -> GetUsedTracks();

		opTracks -> LockContainer();
		opTracks -> Rewind();
	
	//	opProject -> GetUnusedTracks() -> LockContainer(); //Seem to lock the whole application??!!
		opProject -> GetUnusedTracks() -> Rewind();
	
		EMMediaTrack* opCurrentTrack = opTracks -> Current();
	
		if(opCurrentTrack == NULL)
			opCurrentTrack = opProject -> GetUnusedTracks() -> Current();
	
		while(opCurrentTrack != NULL)
		{	

			if((((find(oID.begin(), oID.end(), opCurrentTrack -> GetID()) == oID.end()) && 
				(opCurrentTrack -> GetType() & p_eType) > 0)) ||
				(oID.size() == 0 && (opCurrentTrack -> GetType() & p_eType) > 0))
			{
				if(opCurrentTrack == NULL)
					EMDebugger("ERROR! Current track in opTracks seems to be NULL, in ClipRepository::GetBuffers");
				EMMediaBufferSource* opSilentBufferSource = opCurrentTrack -> GetSilenceGenerator();
				if(opSilentBufferSource == NULL)
					EMDebugger("ERROR! Silence generator seems to be NULL, in ClipRepository::GetBuffers");
				EMMediaDataBuffer* opBuffer = opSilentBufferSource -> GetBuffer();
				opBuffer -> m_vSizeUsed = opBuffer -> m_vSizeAvailable;

				if(opBuffer == NULL)
					EMDebugger("ERROR! Buffer returned from silence generator seems to be NULL, in ClipRepository::GetBuffers");
				//;//cout_commented_out_4_release<<"Pushing one Silent Buffer"<<endl;
				p_opList -> push_back(opBuffer);
				vNumBuffers++;
			}
			opTracks -> Next();
			opCurrentTrack = opTracks -> Current();
		}
	
//		opProject -> GetUnusedTracks() -> UnlockContainer();
		opTracks -> UnlockContainer();

//		Unlock();
	}//if(0)

}

bool EMWinVideoClipRepository::InitCheckE()
{
	return true;
}

EMMediaClip* EMWinVideoClipRepository::GetPriorityClip(int64 p_vNow, int64 p_vFramesToNextClip, list<EMMediaDataBuffer*>* p_opList, list<int32>* p_oID)
{
	int64 vOffset = 0;
	bool vFirstEncounter(true);
	EMMediaClip* opCurrentClip = NULL;
	while(Current() != NULL)
	{
		if((Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0 && Current() -> GetTrack() -> GetMuteState() == EM_MUTE_OFF)
		{
			int64 vCurrentStartFrame = Current() -> GetStart();
			int64 vCurrentDuration = Current() -> GetActiveLength();

			if((vCurrentStartFrame <= p_vNow + vOffset) && (vCurrentStartFrame + vCurrentDuration >= p_vNow) &&
				((Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0))
			{
				if(vFirstEncounter)
				{
					opCurrentClip = Current();
					vFirstEncounter = false;
				}
				Next();
				if(Current() != NULL)
				{
					if((Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0 && Current() -> GetTrack() -> GetMuteState() == EM_MUTE_OFF)
					{
						vCurrentStartFrame = Current() -> GetStart();
						vCurrentDuration = Current() -> GetActiveLength();
						while((Current() != NULL && ((vCurrentStartFrame > p_vNow + vOffset) || (vCurrentStartFrame + vCurrentDuration < p_vNow))) || ((Current() != NULL && ((Current() -> GetType() & EM_TYPE_ANY_VIDEO) <= 0))))
						{
							Next();
							if(Current() != NULL && Current() -> GetTrack() -> GetMuteState() == EM_MUTE_OFF)
							{
								vCurrentStartFrame = Current() -> GetStart();
								vCurrentDuration = Current() -> GetActiveLength();
							}
						}
						if(Current() != NULL && (Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0 && Current() -> GetTrack() -> GetMuteState() == EM_MUTE_OFF)
						{
							EMMediaClip* opSecondaryClip = Current();
							if(opCurrentClip -> GetTrack() -> GetPriority() > opSecondaryClip -> GetTrack() -> GetPriority())
								opCurrentClip = Current();
						} else
						{
							bool latcholajbans = true;
						}
					}
					else
					{
					}
				}
				else
				{
				}
			}
			else
			{
				Next();
			}
		}
		else
			Next();
	}

	if(opCurrentClip != NULL)
	{
		p_oID -> push_back(opCurrentClip -> GetTrack() -> GetID());						
	}
	return opCurrentClip;
}

void EMWinVideoClipRepository::SeekTo(int64 p_vNewSongFrame)
{
}

void EMWinVideoClipRepository::SeekAndDisplay(int64 p_vNow)
{
//	Lock();
	float vAudioFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	int64 vNewFrame = static_cast<int64>(p_vNow * vAudioFrameRate / 1000000.0);

	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		EMMediaClip* opCurrentClip = Current();
		if(Current() != NULL)
		{
			int64 vCurrentStartFrame = opCurrentClip -> GetStart();
			int64 vCurrentDuration = opCurrentClip -> GetActiveLength();
			int64 vFirstFrame = opCurrentClip -> GetMediaStart();
			if(vCurrentStartFrame <= vNewFrame && vCurrentStartFrame + vCurrentDuration >= vNewFrame)
			{
				if ((opCurrentClip -> GetDescriptor() -> GetFormat() -> m_eType & EM_TYPE_ANY_VIDEO) > 0)
				{
					float vAudioFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
					int64 vOffset = static_cast<int64>((vNewFrame - vFirstFrame) * 1000000 / vAudioFrameRate);
					opCurrentClip -> GetDescriptor() -> SeekAndDisplay(vOffset);
					break;
				}
			}
			else
				opCurrentClip -> GetDescriptor() -> SeekTo(0);
		} 
		else
			break;
		Next();
	}
	UnlockContainer();

//	Unlock();
}

#endif
