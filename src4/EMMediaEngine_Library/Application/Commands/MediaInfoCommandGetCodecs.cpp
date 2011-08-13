#include "MediaInfoCommandGetCodecs.h"

#include "EMMediaEngine.h"
#include "EMMediaSystemInspector.h"

#include <list>

MediaInfoCommandGetCodecs::MediaInfoCommandGetCodecs()
{
}

void* MediaInfoCommandGetCodecs::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//;//cout_commented_out_4_release << "MediaInfoCommandGetCodecs" << endl;
	//TODO: Throw exception if fails:
	return static_cast<void*>(EMMediaSystemInspector::GetInstance() -> GetCodecs());
}

bool MediaInfoCommandGetCodecs::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetCodecs::IsUndoable()
{
	return false;
}