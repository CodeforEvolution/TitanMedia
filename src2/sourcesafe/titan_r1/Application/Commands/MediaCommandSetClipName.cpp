#include "MediaCommandSetClipName.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaBufferSource.h"

MediaCommandSetClipName::MediaCommandSetClipName(int32 p_vClipID, const char* p_vpOldName)
	:	m_vClipID(p_vClipID)
		//m_oOldName(p_oOldName)
{
	eo << "Con" << ef;
} 

MediaCommandSetClipName::~MediaCommandSetClipName()
{
	eo << "Des" << ef;
}

void MediaCommandSetClipName::ExecuteE()
{
	eo << "Not here!" << ef;
}

void* MediaCommandSetClipName::ExecuteE(void* p_upMediaClipID, void* p_upString, void*)
{
	EMMediaClip* opClip = NULL;

	int32* vpID = static_cast<int32*>(p_upMediaClipID);
	int32 vMediaClipID = *vpID;
	string oNewClipName = static_cast<const char*>(p_upString);

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();
	
	opAudioClipRepository -> LockContainer();
	opClip = opAudioClipRepository -> Find(vMediaClipID);
	opAudioClipRepository -> UnlockContainer();
	if(opClip == NULL)
	{
		opVideoClipRepository -> LockContainer();
		opClip = opVideoClipRepository -> Find(vMediaClipID);
		opVideoClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opMIDIClipRepository -> LockContainer();
		opClip = opMIDIClipRepository -> Find(vMediaClipID);
		opMIDIClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opVideoTransitionClipRepository -> LockContainer();
		opClip = opVideoTransitionClipRepository -> Find(vMediaClipID);
		opVideoTransitionClipRepository -> UnlockContainer();
	}

	if(opClip != NULL)
	{
		//m_oOldName = opClip -> GetName();
		m_vClipID = vMediaClipID;
		opClip -> SetName(oNewClipName);
		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	}
	else
		m_vClipID = -1;
	return NULL;
}
/*	int32 vMediaClipID = *(static_cast<int32*>(p_upMediaClipID));
	string oNewClipName(static_cast<const char*>(p_upString));

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();
	
	opAudioClipRepository -> LockContainer();
	EMMediaClip* opClip = NULL;
	try
	{
		opClip = opAudioClipRepository -> Find(vMediaClipID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetClipName::ExecuteE");
	}
	opAudioClipRepository -> UnlockContainer();
	if(opClip == NULL)
	{
		opVideoClipRepository -> LockContainer();
		try
		{
			opClip = opVideoClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipName::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opMIDIClipRepository -> LockContainer();
		try
		{
			opClip = opMIDIClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipName::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opVideoTransitionClipRepository -> LockContainer();
		try
		{
			opClip = opVideoTransitionClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipName::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
	}

	if(opClip != NULL)
	{
		m_oOldName = opClip -> GetName();
		m_vClipID = vMediaClipID;
		opClip -> SetName(oNewClipName);
		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	}
	else
	{
		m_vClipID = -1;
	} 

	return NULL;
}*/

bool MediaCommandSetClipName::RequiresParameters()
{
	return true;
}

bool MediaCommandSetClipName::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetClipName::CloneAndClear()
{
	return EM_new MediaCommandSetClipName(m_vClipID, NULL);
}

void MediaCommandSetClipName::UndoE()
{
	if(m_vClipID != -1)
	{
		EMMediaEngine* opEngine = EMMediaEngine::Instance();
		EMMediaProject* opProject = opEngine -> GetMediaProject();

		EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
		EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();

		opAudioClipRepository -> LockContainer();
		EMMediaClip* opClip = NULL;
		try
		{
			opClip = opAudioClipRepository -> Find(m_vClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipName::UndoE");
		}
		opAudioClipRepository -> UnlockContainer();
		if(opClip == NULL)
		{
			opVideoClipRepository -> LockContainer();
			try
			{
				opClip = opVideoClipRepository -> Find(m_vClipID);
			}
			catch(...)
			{
				EMDebugger("ERROR! Exception in MediaCommandSetClipName::UndoE");
			}
			opVideoClipRepository -> UnlockContainer();
		}
			
		opClip -> SetName(NULL);
	}
}
