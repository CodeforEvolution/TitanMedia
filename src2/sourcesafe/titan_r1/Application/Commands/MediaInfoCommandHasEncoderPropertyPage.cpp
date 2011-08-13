#include "MediaInfoCommandHasEncoderPropertyPage.h"

#include "EMMediaEngine.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinQuickTimeWriterSuper.h"

MediaInfoCommandHasEncoderPropertyPage::MediaInfoCommandHasEncoderPropertyPage()
{
}

void* MediaInfoCommandHasEncoderPropertyPage::ExecuteE(void* p_vEncoderID, void*, void*)
{
	int32 vpEncoderID = *static_cast<int32*>(p_vEncoderID);
	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		//Temporarily set to false
		m_vHasPropertyPage = EMWinQuickTimeWriterSuper::Instance() -> GetEncoderPropertyPages(vpEncoderID, false);

		return static_cast<void*>(&m_vHasPropertyPage);
	}

	m_vHasPropertyPage = EMWinMediaSingletonMuxer::Instance() ->GetEncoderPropertyPages(vpEncoderID, false);
	return static_cast<void*>(&m_vHasPropertyPage);
}

bool MediaInfoCommandHasEncoderPropertyPage::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandHasEncoderPropertyPage::IsUndoable()
{
	return false;
}