#include "MediaInfoCommandGetClipName.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"

#include <list>

MediaInfoCommandGetClipName::MediaInfoCommandGetClipName()
{
}

void* MediaInfoCommandGetClipName::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	string oString = "";
	//eo << "MediaInfoCommandGetClipName" << ef;
	//TODO: Throw exception if fails:
	int32 vMediaClipID = *static_cast<int32*>(p_upMediaClipID);
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();
	
	EMMediaClip* opClip = NULL;
	opAudioClipRepository -> LockContainer();
	try
	{
		opClip = opAudioClipRepository -> Find(vMediaClipID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetClipName::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipName::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipName::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipName::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		EMDebugger("ERROR! Could not find media clip id in clip repository!");
	
	oString = opClip -> GetName();

	for(int v = 0; v < oString.length()+1; m_vpReturnValue[v++] = '\0')
		;
	memcpy(m_vpReturnValue, oString.c_str(), oString.length());
	return static_cast<void*>(m_vpReturnValue);
}

bool MediaInfoCommandGetClipName::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetClipName::IsUndoable()
{
	return false;
}