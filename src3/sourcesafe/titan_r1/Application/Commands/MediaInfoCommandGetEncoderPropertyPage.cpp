#include "MediaInfoCommandGetEncoderPropertyPage.h"

#include "EMMediaEngine.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinQuickTimeWriterSuper.h"

MediaInfoCommandGetEncoderPropertyPage::MediaInfoCommandGetEncoderPropertyPage()
{
}

void* MediaInfoCommandGetEncoderPropertyPage::ExecuteE(void* p_vEncoderID, void*, void*)
{
	int32 vpEncoderID = *static_cast<int32*>(p_vEncoderID);
	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		//Temporarily set to false
		EMWinQuickTimeWriterSuper::Instance() -> GetEncoderPropertyPages(vpEncoderID, true);

		return NULL;
	}
	EMWinMediaSingletonMuxer::Instance() ->GetEncoderPropertyPages(vpEncoderID, true);
	return NULL;
}

bool MediaInfoCommandGetEncoderPropertyPage::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEncoderPropertyPage::IsUndoable()
{
	return false;
}