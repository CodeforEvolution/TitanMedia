#include "MediaInfoCommandGetNumberTransitions.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaVideoTransitionClip.h"
#include "EMWinVideoTransitionClipRepository.h"
#include "EMPluginRepository.h"


MediaInfoCommandGetNumberOfTransitions::MediaInfoCommandGetNumberOfTransitions()
{
}

void MediaInfoCommandGetNumberOfTransitions::ExecuteE() {}


void* MediaInfoCommandGetNumberOfTransitions::ExecuteE(void* p_upOnlyNativeTrans, void*, void*)
{
	EMPluginRepository *opPlugs = EMPluginRepository::Instance();

	m_vNumTrans = 0;

	opPlugs -> LockContainer();
	opPlugs -> Rewind();
	bool vOnlyNativeTransitions = (p_upOnlyNativeTrans == NULL ? false : *static_cast<bool*>(p_upOnlyNativeTrans));

	while(opPlugs -> Current() != NULL)
	{
		if(vOnlyNativeTransitions)
		{
			if(opPlugs -> Current() -> GetPluginType() == EM_NATIVE_VIDEO_TRANSITION)
			{
				++m_vNumTrans;
			}
		}
		else if(opPlugs -> Current() -> GetPluginType() == EM_WINDOWS_DIRECTX_VIDEO_TRANSITION  || opPlugs -> Current() -> GetPluginType() == EM_NATIVE_VIDEO_TRANSITION)
			++m_vNumTrans;
		opPlugs -> Next();
	}

	opPlugs -> UnlockContainer();

	return static_cast<void*>(&m_vNumTrans);
}


bool MediaInfoCommandGetNumberOfTransitions::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetNumberOfTransitions::IsUndoable()
{
	return false;
}
