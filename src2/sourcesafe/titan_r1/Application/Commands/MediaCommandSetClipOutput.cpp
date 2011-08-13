#include "MediaCommandSetClipOutput.h"

#include "EMMediaEngine.h"
#include "EMMediaClipRepository.h"
#include "EMOutputRepository.h"
#include "EMMediaTrack.h"
#include "EMMediaProject.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEffectTrack.h"

MediaCommandSetClipOutput::MediaCommandSetClipOutput(int32 p_vRecentClipID, int32 p_vRecentOutputID)
	:	m_vRecentClipID(p_vRecentClipID),
		m_vRecentOutputID(p_vRecentOutputID)
{
}

void* MediaCommandSetClipOutput::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	EMMediaClip* opClip = NULL;
	EMMediaClipRepository* opClips[2];

	EMMediaEngine* opEngine = EMMediaEngine::Instance();

	opClips[0] = opEngine -> GetMediaProject() -> GetAudioClipRepository();
	opClips[1] = opEngine -> GetMediaProject() -> GetVideoClipRepository();

	EMOutputRepository* opOutputs = EMOutputRepository::Instance();

	int32 vClipID = *(static_cast<int32*>(p_opParameterOne));
	int32 vOutputID = *(static_cast<int32*>(p_opParameterTwo));
	bool vFxTrack = true; //*(static_cast<bool*>(p_opParameterThree));

	try
	{	
		for(int i = 0; i < 2 && opClip == NULL; ++i)
		{
			opClips[i] -> LockContainer();
			opClip = opClips[i] -> Find(vClipID);
			opClips[i] -> UnlockContainer();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetClipOutput::ExecuteE");
	}

	if(opClip == NULL)
	{
		EMDebugger("MediaCommandSetClipOutput - no Clip found");
		return NULL;
	}


	if(vFxTrack)
	{
		EMMediaEffectTrack* opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(vOutputID);

		if(opFXTrack != NULL)
		{
			opFXTrack -> SetOutput(opClip -> GetDestination());
			opClip -> SetDestination(opFXTrack);
			opClip -> SetEffectTrackID(opFXTrack -> GetID());
		}
		else
			EMDebugger("MediaCommandSetClipOutput - no FX found");
	}
	else
	{	
		opOutputs -> LockContainer();
		EMRealtimeOutputDescriptor* opOutput = NULL;
		try
		{
			opOutput = opOutputs -> Find(vOutputID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipOutput::ExecuteE");
		}
		opOutputs -> UnlockContainer();
		
		if(opOutput != NULL)
		{
			m_vRecentClipID = vClipID;
			m_vRecentOutputID = vOutputID;
			opClip -> SetDestination(opOutput);
			EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
		}
		else
			EMDebugger("MediaCommandSetClipOutput - no Output found");
	}

	return NULL;
}

bool MediaCommandSetClipOutput::RequiresParameters()
{
	return true;
}

void MediaCommandSetClipOutput::UndoE()
{
	if(m_vRecentClipID != -1 && m_vRecentOutputID != -1)
	{
		EMMediaEngine* opEngine = EMMediaEngine::Instance();
		EMMediaClipRepository* opAudioClips = opEngine -> GetMediaProject() -> GetAudioClipRepository();
		EMMediaClipRepository* opVideoClips = opEngine -> GetMediaProject() -> GetVideoClipRepository();
		EMOutputRepository* opOutputs = EMOutputRepository::Instance();
		
		opAudioClips -> LockContainer();
		EMMediaClip* opClip = NULL;
		try
		{
			opClip = opAudioClips -> Find(m_vRecentClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipOutput::UndoE");
		}
		opAudioClips -> UnlockContainer();
		if(opClip == NULL)
		{
			opVideoClips -> LockContainer();
			try
			{
				opClip = opVideoClips -> Find(m_vRecentClipID);
			}
			catch(...)
			{
				EMDebugger("ERROR! Exception in MediaCommandSetClipOutput::UndoE");
			}
			opVideoClips -> UnlockContainer();
		}
			
		EMRealtimeOutputDescriptor* opOutput = NULL;

		opOutputs -> LockContainer();
		try
		{
			opOutput = opOutputs -> Find(m_vRecentOutputID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipOutput::UndoE");
		}
		opOutputs -> UnlockContainer();
		if(opClip != NULL && opOutput != NULL)
			opClip -> SetDestination(opOutput);
	}
	else
		EMDebugger("Bad stored track or bad stored output ID in SetTrackOutput Command");
}

bool MediaCommandSetClipOutput::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetClipOutput::CloneAndClear()
{
	return EM_new MediaCommandSetClipOutput(m_vRecentClipID, m_vRecentOutputID);
}

