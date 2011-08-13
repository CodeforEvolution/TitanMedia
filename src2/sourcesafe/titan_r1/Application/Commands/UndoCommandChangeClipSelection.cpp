#include "UndoCommandChangeClipSelection.h"

#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMView.h"
#include "TrackInfo.h"
#include "TrackView.h"

UndoCommandChangeClipSelection::UndoCommandChangeClipSelection(TrackView* p_opTrackView, list<EMGClip*>* p_opClipsToBeSelected) :
m_opTrackView(p_opTrackView)
{
	if(p_opClipsToBeSelected != NULL)
		m_oClipsToBeSelected = *p_opClipsToBeSelected;
}

EMCommand* UndoCommandChangeClipSelection::CloneAndClear()
{
	UndoCommandChangeClipSelection* opCommand = new UndoCommandChangeClipSelection(m_opTrackView, &m_oClipsToBeSelected);
	m_oClipsToBeSelected.clear();
	return opCommand;
}

void* UndoCommandChangeClipSelection::ExecuteE(void* upParameterOne, void* upParameterTwo, void* upParameterThree)
{
	m_oClipsToBeSelected = *(static_cast<list<EMGClip*>*>(upParameterOne));
	return NULL;
}

const char* UndoCommandChangeClipSelection::GetShortDescription()
{
	return "Change Clip Selection";
}

bool UndoCommandChangeClipSelection::IsUndoable()
{
	return false; //true;
}

bool UndoCommandChangeClipSelection::RequiresParameters()
{
	return true;
}

void UndoCommandChangeClipSelection::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGClip*> oCurrentSelectedClips = *(EMGClipRepository::Instance() -> GetSelectedClips());
		EMGClipRepository::Instance() -> SetSelected(&oCurrentSelectedClips, false);
		EMGClipRepository::Instance() -> SetSelected(&m_oClipsToBeSelected, true);
		m_opTrackView -> GetView() -> Invalidate(m_opTrackView -> GetView() -> Bounds());
		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

