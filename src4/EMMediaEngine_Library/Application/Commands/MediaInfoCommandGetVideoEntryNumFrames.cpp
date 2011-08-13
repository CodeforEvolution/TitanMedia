#include "MediaInfoCommandGetVideoEntryNumFrames.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolVideoEntry.h"
#include "EMMediaProject.h"
#include "EMMediaClip.h"
#include "EMMediaFormat.h"

#include <list>

MediaInfoCommandGetVideoEntryNumFrames::MediaInfoCommandGetVideoEntryNumFrames()
{
}

void* MediaInfoCommandGetVideoEntryNumFrames::ExecuteE(void* p_upMediaPoolEntryID, void* p_vFramerate, void*)
{
	//cout << "MediaInfoCommandGetVideoEntryNumFrames" << endl;
	//TODO: Throw exception if fails:
	int32 vMediaPoolEntryID = *static_cast<int32*>(p_upMediaPoolEntryID);
	float vFramerate = *static_cast<float*>(p_vFramerate);
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	EMMediaPoolEntry* opEntry = NULL;
	opPool -> LockContainer();
	try
	{
		opEntry = opPool -> Find(vMediaPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetVideoEntryNumFrames::ExecuteE");
	}
	opPool -> UnlockContainer();

	if(opEntry != NULL)
	{
//		if((opEntry -> GetType() & EM_TYPE_ANY_VIDEO) > 0 )
//		{
			return static_cast<void*>(opEntry -> NumberOfFramesInWFPS(vFramerate));
//		}
//		else
//		{
//			m_vDummy = 1;
//			return static_cast<void*>(&m_vDummy);
//		}
	}
	else
		EMDebugger("ERROR! MediaInfoCommandGetVideoEntryNumFrames could not find pool entry ID");

	return NULL;
}

bool MediaInfoCommandGetVideoEntryNumFrames::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoEntryNumFrames::IsUndoable()
{
	return false;
}