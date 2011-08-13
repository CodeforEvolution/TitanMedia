#include "MediaCommandSetEncoderFamily.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMWinMediaSingletonMuxer.h"

MediaCommandSetEncoderFamily::MediaCommandSetEncoderFamily()
{
}

void* MediaCommandSetEncoderFamily::ExecuteE(void* p_vFamilyID, void* , void*)
{
	EMMediaFamily vFamilyID = *static_cast<EMMediaFamily*>(p_vFamilyID);

	EMWinMediaSingletonMuxer::Instance() -> m_vCurrentlyUsedFamily = vFamilyID;

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	return NULL;
}

bool MediaCommandSetEncoderFamily::RequiresParameters()
{
	return true;
}

bool MediaCommandSetEncoderFamily::IsUndoable()
{
	return false;
}
