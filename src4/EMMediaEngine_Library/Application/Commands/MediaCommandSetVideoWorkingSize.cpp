#include "MediaCommandSetVideoWorkingSize.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandSetVideoWorkingSize::MediaCommandSetVideoWorkingSize()
{
}

void* MediaCommandSetVideoWorkingSize::ExecuteE(void* p_vWidth, void* p_vHeight, void*)
{
	int32 vWidth = *static_cast<int32*>(p_vWidth);
	int32 vHeight = *static_cast<int32*>(p_vHeight);

	EMMediaEngine::Instance() -> GetMediaProject() -> SetVideoWorkingSize(vWidth, vHeight);
	
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	return NULL;
}

bool MediaCommandSetVideoWorkingSize::RequiresParameters()
{
	return true;
}

bool MediaCommandSetVideoWorkingSize::IsUndoable()
{
	return false;
}
