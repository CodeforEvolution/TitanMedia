#include "UndoCommandChangeClipResize.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMRect.h"
#include "EMView.h"
#include "TrackInfo.h"
#include "TrackView.h"

// The parameters in the list are (in correct order): position, mark in, mark out
UndoCommandChangeClipResize::UndoCommandChangeClipResize(TrackView* p_opTrackView, EMGClip* p_opClip, list<int64>* p_opClipParameters, EMRect p_oClipFrame) :
m_oClipFrame(p_oClipFrame),
m_opClip(p_opClip),
m_opTrackView(p_opTrackView)
{
	if(p_opClipParameters != NULL)
		m_oClipParameters = *p_opClipParameters;
}

EMCommand* UndoCommandChangeClipResize::CloneAndClear()
{
	UndoCommandChangeClipResize* opCommand = new UndoCommandChangeClipResize(m_opTrackView, m_opClip, &m_oClipParameters, m_oClipFrame);
	m_oClipParameters.clear();
	return opCommand;
}

void* UndoCommandChangeClipResize::ExecuteE(void* upParameterOne, void* upParameterTwo, void* upParameterThree)
{
	m_opClip = static_cast<EMGClip*>(upParameterOne);
	m_oClipParameters = *(static_cast<list<int64>*>(upParameterTwo));
	m_oClipFrame = *(static_cast<EMRect*>(upParameterThree));
	return NULL;
}

const char* UndoCommandChangeClipResize::GetShortDescription()
{
	return "Resize Clips";
}

bool UndoCommandChangeClipResize::IsUndoable()
{
	return true;
}

bool UndoCommandChangeClipResize::RequiresParameters()
{
	return true;
}

void UndoCommandChangeClipResize::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		int32 vClipID = m_opClip -> GetMediaClipID();
		EMRect oInvalidateFrame;
		if(m_opClip -> GetSelected())
			oInvalidateFrame = EMRect(EMGClipRepository::Instance() -> SelectedFrame());
		else
			oInvalidateFrame = m_opClip -> Frame();

		int64 vPosition = m_oClipParameters.front();
		m_oClipParameters.pop_front();
		m_opClip -> MoveClip(0, vPosition - (m_opClip -> GetPosition()));

		int64 vNewClipMarkIn = m_oClipParameters.front();
		m_oClipParameters.pop_front();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_IN, &vClipID, &vNewClipMarkIn);

		int64 vNewClipMarkOut = m_oClipParameters.front();
		m_oClipParameters.pop_front();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_OUT, &vClipID, &vNewClipMarkOut);

		m_opClip -> SetFrame(m_oClipFrame);
		EMGClipRepository::Instance() -> CalculateSelectedFrame();

		if(m_opClip -> GetSelected())
			oInvalidateFrame = oInvalidateFrame | (EMRect(EMGClipRepository::Instance() -> SelectedFrame()));
		else
			oInvalidateFrame = oInvalidateFrame | (m_opClip -> Frame());

		m_opTrackView -> GetView() -> Invalidate(oInvalidateFrame);
		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

