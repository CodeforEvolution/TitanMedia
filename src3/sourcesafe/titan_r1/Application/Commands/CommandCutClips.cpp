#include "CommandCutClips.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMRect.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "MediaPoolView.h"
#include "TrackView.h"

CommandCutClips::CommandCutClips(TrackView* p_opTrackView, list<EMGClip*> p_oCutClips, list<EMGClip*> p_oNewClips) :
m_oCutClips(p_oCutClips),
m_oNewClips(p_oNewClips),
m_opTrackView(p_opTrackView)
{
}

CommandCutClips::CommandCutClips(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

EMCommand* CommandCutClips::CloneAndClear()
{
	CommandCutClips* opCommand = new CommandCutClips(m_opTrackView, m_oCutClips, m_oNewClips);
	m_oCutClips.clear();
	m_oNewClips.clear();
	return opCommand;
}

// Ownership of p_upParameterOne transferred to this object
void* CommandCutClips::ExecuteE(void* p_upParameterOne, void*  p_upParameterTwo, void* p_upParameterThree)
{
	const char* vpFileName;
	int32* vpEntry;

	if(m_opTrackView -> GetView() -> LockWindow())
	{
		EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
		list<EMGClip*>* opClips = static_cast<list<EMGClip*>*>(p_upParameterOne);
		int64 vPosition = *(static_cast<int64*>(p_upParameterTwo));

		EMGClip* opClip = *(opClips ->begin());
		int32 vMediaClipID = opClip -> GetMediaClipID();

		list<EMGClip*>::iterator oIterator = opClips -> begin();
		while(oIterator != opClips -> end())
		{
			if((*oIterator) -> GetType() != EM_TYPE_TRANSITION) // Transition clips cannot be split
				m_oCutClips.push_back(*oIterator);
			oIterator++;
		}

		int64 vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
		m_oNewClips = *(opClipRepository -> SplitClips(&m_oCutClips, vPosition, vTimeZoomScale));

		// FIXME: Hmm, this code here seems to work only if one single clip is sent to this command
		vpEntry = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vMediaClipID));
		vpFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, vpEntry));
		MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(vpFileName));

		m_opTrackView -> GetView() -> UnlockWindow();
	}
	return NULL;
}

const char* CommandCutClips::GetShortDescription()
{
	return "Cut Clips";
}

bool CommandCutClips::IsUndoable()
{
	return true;
}

bool CommandCutClips::RequiresParameters()
{
	return true;
}

void CommandCutClips::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGClip*>::iterator oCutClipsIterator = m_oCutClips.begin();
		list<EMGClip*>::iterator oNewClipsIterator = m_oNewClips.begin();
		int64 vMarkOut;
		int32 vNewClipID;
		int32 vCutClipID;
		while(oCutClipsIterator != m_oCutClips.end())
		{
			vNewClipID = (*oNewClipsIterator) -> GetMediaClipID();
			vCutClipID = (*oCutClipsIterator) -> GetMediaClipID();
			vMarkOut = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vNewClipID, NULL, NULL)));
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_OUT, &vCutClipID, &vMarkOut, NULL);
			(*oCutClipsIterator) -> SetFrame((*oCutClipsIterator) -> Frame() | (*oNewClipsIterator) -> Frame());
			oCutClipsIterator++;
			oNewClipsIterator++;
		}

		EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_DELETE_CLIPS_FROM_LIST, &m_oNewClips, NULL, NULL);
		delete EMCommandRepository::Instance() -> GetUndoClone(COMMAND_DELETE_CLIPS_FROM_LIST);

		EMGClipRepository::Instance() -> Update();
		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

