#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMIDIClipRepository.h"
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
#include "EMMIDIClip.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaMIDIEventRepository.h"

#include <algorithm>

EMWinMIDIClipRepository::EMWinMIDIClipRepository()
	:	EMMediaClipRepository(EM_TYPE_MIDI)
{
	m_vID = EMMediaIDManager::MakeID();
	//EMBeMediaUtility::push(this, "EMWinMIDIClipRepository");
} 

EMWinMIDIClipRepository::~EMWinMIDIClipRepository() //Deleted by MediaProject
{
	//EMBeMediaUtility::pop("EMWinMIDIClipRepository");
} 

int32 EMWinMIDIClipRepository::GetID() const
{
	return m_vID;
}

bool EMWinMIDIClipRepository::IsSoloActivated()
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
int64 EMWinMIDIClipRepository::FramesToNextClip(int64 p_vFromFrame)
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
		EMDebugger("ERROR! Exception in EMWinMIDIClipRepository::FramesToNextClip");
	}
	UnlockContainer();
	return (vDistance == -1 ? static_cast<int64>(static_cast<uint64>(-1)) : vDistance);
}

void EMWinMIDIClipRepository::GetNextBuffers(list<EMMediaDataBuffer*>* p_opList, EMMediaType p_eType, int64 p_vTimeNow, bool p_vFlag)
{
	EMDebugger("ERROR! Can't end up here!!!!!!!!!");
	int64 vOffset = 0;
	int64 vNumBuffers = 0;
	int64 vNow = 1000000; //p_vTimeNow;
	int64 vFramesToNextClip = FramesToNextClip(vNow);
	bool vSoloMode = IsSoloActivated();
	list<int32> oID;

	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if(! Current() -> IsObjectDeleted()) //Make sure we don't process deleted-but-unpurged clips
			{
				//Make sure we don't play muted tracks and only play from soloed tracks if there's at least one soloed track in the repository!
				EMMIDIClip* opCurrentClip = static_cast<EMMIDIClip*>(Current());
				EMMediaMIDITrack* opCurrentTrack = static_cast<EMMediaMIDITrack*>(opCurrentClip -> GetTrack());

				if((! vSoloMode && opCurrentTrack -> GetMuteState() == EM_MUTE_OFF) ||
					(vSoloMode && opCurrentTrack -> IsSoloed()))
				{
					vOffset = 0;
					int64 vCurrentStartTime = opCurrentClip -> GetStart(); //opOrderedEventList -> First() -> GetPosition() -> GetStart();
					int64 vCurrentDuration = opCurrentClip -> GetActiveLength();

//					int64 vFramesPerBuffer = 0; //EMBeMediaUtility::BytesToFrames(EM_MIDI_BUFFER_SIZE, m_opSystemMIDIFormat);

					//if there's less than a whole buffer's duration to the next clip start, measured from "now"
//					if(! (vCurrentStartFrame <= vNow && vCurrentStartFrame + vCurrentDuration >= vNow) &&
//						vFramesToNextClip < vFramesPerBuffer && vFramesToNextClip != 0)
//						vOffset = vFramesToNextClip;

					//If "this" clip intersects the current frame
					if(vCurrentStartTime <= vNow + vOffset &&
						vCurrentStartTime + vCurrentDuration >= vNow)
					{
						EMMediaDataBuffer* opMediaBuffer = NULL;
						
						opMediaBuffer = opCurrentClip -> GetBuffer();
						if(opMediaBuffer != NULL)
						{
							opMediaBuffer -> SetFrame(EMMediaTimer::Instance() -> MIDIThenTime());

//							int64 vIONumFrames = vFramesPerBuffer;
//							if(vNow + vIONumFrames > vCurrentStartFrame + vCurrentDuration)
//							{
								//if one buffer of data from this files will end up beoynd the Mark-out point!
//								vIONumFrames = (vCurrentStartFrame + vCurrentDuration) - vNow;
//							}
							
//							int64 vFramePosition = 0;
//							if(vNow > opCurrentClip -> GetMediaStart())
//								vFramePosition = vNow - opCurrentClip -> GetMediaStart(); //Make sure we get the file-orientation corrent (discard MarkIN values here!
//							EMMediaDataBuffer* opEmptyBufferPointer = NULL;

							//TODO: READ EVENTS INSTEAD!
//							bool vResult = opCurrentClip -> GetDescriptor() -> ReadFrames(&opMediaBuffer, &opEmptyBufferPointer, vFramePosition, vOffset, vIONumFrames);

/*							list<EMMediaMIDIEvent*>* opBufferedEvents = reinterpret_cast<list<EMMediaMIDIEvent*>*>(opMediaBuffer -> Data());
							opBufferedEvents -> clear();

							opCurrentClip -> LockContainer();
							opCurrentClip -> Rewind();
							while(opCurrentClip -> Current() != NULL)
							{
								EMMediaMIDIEvent* opEvent = opCurrentClip -> Current();
								if(opEvent -> GetPosition() -> GetStart() >= vNow)
								{
									opBufferedEvents -> push_back(opEvent);
								}
								opCurrentClip -> Next();
							}
							opCurrentClip -> UnlockContainer();

							opMediaBuffer -> m_vSizeUsed = opBufferedEvents -> size(); */

							if((p_eType & EM_TYPE_MIDI) > 0)
							{
								opMediaBuffer -> m_vFader = opCurrentTrack -> GetFader();
								opMediaBuffer -> m_vPan = opCurrentTrack -> GetPan();
								oID.push_back(opCurrentTrack -> GetID());
							}
							p_opList -> push_back(opMediaBuffer);
							vNumBuffers++;
						}
					}
				}
			}

			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
}

bool EMWinMIDIClipRepository::InitCheckE()
{

	return true;
}

bool EMWinMIDIClipRepository::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	return false;
}

#endif
