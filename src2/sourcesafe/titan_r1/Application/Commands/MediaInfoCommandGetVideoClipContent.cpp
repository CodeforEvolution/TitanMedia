#include "MediaInfoCommandGetVideoClipContent.h"
#include "EMGlobals.h"
#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolVideoEntry.h"
#include "EMMediaPoolImageEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClip.h"
#include "EMBitmap.h"
#include "EMWinBitmap.h"

////TEMPORARY
#include "EMWinVideoFile.h"
#include "EMVideoFile.h"
//#include "EMWinBitmap.h"
//#include "EMBitmap.h"
////END TEMPO

#include <iostream>
#include <list>

MediaInfoCommandGetVideoClipContent::MediaInfoCommandGetVideoClipContent()
{
	m_vMediaPoolEntryID = -1;
}

void* MediaInfoCommandGetVideoClipContent::ExecuteE(void* p_upMediaPoolEntryID, void* p_upTime, void* p_upHeight)
{
	if(p_upMediaPoolEntryID == NULL && p_upTime == NULL && p_upHeight == NULL)
	{	//If all parameters are NULL we should interpret this as a "reset" of the member attributes!
		//This is to be able to handle a new project being created! Added by Richard November 8th 2001.
		m_vMediaPoolEntryID = -1;
		m_opEntry = NULL;
		return NULL;
	}
	
	int32 vMediaPoolEntryID = *static_cast<int32*>(p_upMediaPoolEntryID);
	int16 vpHeight = *static_cast<int16*>(p_upHeight);
	//vpHeight = 150;
	int64 vpTime = *static_cast<int64*>(p_upTime);
	//MessageBox(NULL, "Gädda med gräddsås och potatis", "Message for Martin", MB_OK);

/*	EMWinVideoFile* agnus = new EMWinVideoFile("AGNUS");//new static_cast<EMWinVideoFile*>(EMVideoFile::Create("agnus"));
	MessageBox(NULL, "Gädda med gräddsås och potatis2", "Message for Martin", MB_OK);
	agnus -> InitCheckE();
	MessageBox(NULL, "Gädda med gräddsås och potatis3", "Message for Martin", MB_OK);
	//cout << "MediaInfoCommandGetVideoClipContent" << endl;
	//TODO: Throw exception if fails:

	//TEMPO
	HBITMAP opBitmap = agnus->ReadAndScaleData(vpTime, vpHeight);
	EMBitmap* opEMBeBitmapContainer = new EMWinBitmap(opBitmap);

	MessageBox(NULL, "Now returning to Martin", "Message for Martin", MB_OK);
	return opEMBeBitmapContainer;
	//END TEMPO
	*/

	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	
	opPool -> LockContainer();

	//If it is the same media pool entry as before, there is no need to search for it
	//Else the previous entry should have its stream closed (to save streams as 75 is the highest allowed open, for AVI)
	EMMediaPoolEntry* opEntry = NULL;
	if(vMediaPoolEntryID != m_vMediaPoolEntryID)
	{
		if(m_vMediaPoolEntryID != -1)
		{
			if((m_opEntry -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
			{
				if(EMMediaEngine::Instance() -> GetMediaProject() -> LockVideo())
				{	//Close the stream as it is not used anymore (at the moment anyways)
					static_cast<EMMediaPoolVideoEntry*>(m_opEntry) -> CloseAVIStream();
					EMMediaEngine::Instance() -> GetMediaProject() -> UnlockVideo();
				}
			}
		}
		opEntry = opPool -> Find(vMediaPoolEntryID);
		m_opEntry = opEntry;
		m_vMediaPoolEntryID = vMediaPoolEntryID;
	}
	else
	{
		//Same as before. As we already have it saved from previous attempts, just hand the address over.
		opEntry = m_opEntry;
	}

	opPool -> UnlockContainer();
	if(opEntry != NULL)
	{
		if((opEntry -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
		{
			if(EMMediaEngine::Instance() -> GetMediaProject() -> LockVideo())
			{
				EMBitmap* opBitmap = static_cast<EMMediaPoolVideoEntry*>(opEntry) -> GetVideoContent(vpTime, vpHeight);
				EMMediaEngine::Instance() -> GetMediaProject() -> UnlockVideo();
				return static_cast<void*>(opBitmap);
			}
			else
				EMDebugger("ERROR! MediaInfoCommandGetVideoContent couldn't acquire video lock!");
		}
		else if((opEntry -> GetType() & EM_TYPE_ANY_IMAGE) > 0)
		{
			if(EMMediaEngine::Instance() -> GetMediaProject() -> LockVideo())
			{
				EMBitmap* opBitmap = static_cast<EMMediaPoolImageEntry*>(opEntry) -> GetImage(vpHeight);
				EMMediaEngine::Instance() -> GetMediaProject() -> UnlockVideo();
				return static_cast<void*>(opBitmap);
			}
			else
				EMDebugger("ERROR! MediaInfoCommandGetVideoContent couldn't acquire video lock!");

		}
		else
			EMDebugger("ERROR! This seems to be a NON video CLIP (Should be video .. FIXXXX (Martin?)");
	}
	else
		cout << "ERROR! MediaInfoCommandGetVideoClipContent could not find pool entry ID" << endl;
	return NULL;

}

bool MediaInfoCommandGetVideoClipContent::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoClipContent::IsUndoable()
{
	return false;
}