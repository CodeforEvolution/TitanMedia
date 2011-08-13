#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeAudioClipRepository.h"
#include "EMBeFileInputDescriptor.h"
#include "EMMediaIDManager.h"
#include "EMBeMediaUtility.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"

#include <algorithm>

EMBeAudioClipRepository::EMBeAudioClipRepository()
	:	EMMediaClipRepository(EM_TYPE_ANY_AUDIO),
		m_opSystemAudioFormat(NULL)
{
	m_vID = EMMediaIDManager::MakeID();
	EMBeMediaUtility::push(this, "EMBeAudioClipRepository");
}

EMBeAudioClipRepository::~EMBeAudioClipRepository() //Deleted by MediaProject
{
	delete m_opSystemAudioFormat;
	EMBeMediaUtility::pop("EMBeAudioClipRepository");
}

int32 EMBeAudioClipRepository::GetID() const
{
	return m_vID;
}

bool EMBeAudioClipRepository::IsSoloActivated()
{
	LockContainer();
	try
	{
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
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMBeAudioClipRepository::IsSoloActivated()");
	}
	UnlockContainer();
	return false;
}

//Dont call from within a Rewind()/Current()/Next() loop, or things will go badly!
int64 EMBeAudioClipRepository::FramesToNextClip(int64 p_vFromFrame)
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
			if(Current() -> GetStart() >= p_vFromFrame && Current())
			{
				if(vDistance == -1 || Current() -> GetStart() - p_vFromFrame < vDistance)
					vDistance = Current() -> GetStart() - p_vFromFrame;
			}
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMBeAudioClipRepository::FramesToNextClip");
	}
	UnlockContainer();
	return (vDistance == -1 ? static_cast<int64>(static_cast<uint64>(-1)) : vDistance);
}

void EMBeAudioClipRepository::GetNextBuffers(/*EMMediaTrack* p_opForTrack,*/ list<EMMediaDataBuffer*>* p_opList, EMMediaType p_eType, int64 p_vTimeNow, bool)
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
			if((Current() -> GetType() & p_eType) > 0 )
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

					int64 vFramesPerBuffer = EMBeMediaUtility::BytesToFrames(EM_AUDIO_BUFFER_SIZE, m_opSystemAudioFormat);

					//if there's less than a whole buffer's duration to the next clip start, measured from "now"
					if(! (vCurrentStartFrame <= vNow && vCurrentStartFrame + vCurrentDuration >= vNow) &&
						vFramesToNextClip < vFramesPerBuffer && vFramesToNextClip != 0)
						vOffset = vFramesToNextClip;

					//If "this" clip intersects the current frame
					if(vCurrentStartFrame <= vNow + vOffset &&
						vCurrentStartFrame + vCurrentDuration >= vNow)
					{
						EMMediaDataBuffer* opMediaBuffer = opCurrentClip -> GetBuffer();
						if(opMediaBuffer == NULL)
						{
							EMBeMediaUtility::show();
							EMDebugger("ERROR! Couldn't get a buffer from the clip!");
						}
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
								opMediaBuffer -> m_vFader = opCurrentTrack -> GetFader();
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
							opMediaBuffer -> Recycle();
					}
				}
			}

			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMBeAudioClipRepository::GetNextBuffers");
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
		EMDebugger("ERROR! Exception in EMBeAudioClipRepository::GetNextBuffers");
	}
	opTracks -> UnlockContainer();
	opProject -> GetUnusedTracks() -> UnlockContainer();
}

bool EMBeAudioClipRepository::InitCheckE()
{
	if(m_opSystemAudioFormat == NULL)
		m_opSystemAudioFormat = EM_new EMMediaFormat(EM_TYPE_RAW_AUDIO);
	return true;
}

#endif
