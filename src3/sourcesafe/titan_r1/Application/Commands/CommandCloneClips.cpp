#include "CommandCloneClips.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGUIUtilities.h"
#include "EMRect.h"
#include "EMView.h"
#include "MediaPoolView.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

CommandCloneClips::CommandCloneClips(TrackView* p_opTrackView, list<EMGClip*> p_oClonedClips) :
m_oClonedClips(p_oClonedClips),
m_opTrackView(p_opTrackView)
{
}

CommandCloneClips::CommandCloneClips(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

EMCommand* CommandCloneClips::CloneAndClear()
{
	CommandCloneClips* opCommand = new CommandCloneClips(m_opTrackView, m_oClonedClips);
	return opCommand;
}

void* CommandCloneClips::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	m_vTrackOffset = *(static_cast<int32*>(p_opParameterOne));
	m_vPositionOffset = *(static_cast<int64*>(p_opParameterTwo));
	list<EMGClip*> oOriginalClips = *(static_cast<list<EMGClip*>*>(p_opParameterThree));
	EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
	m_oClonedClips.clear();
	opClipRepository -> CloneClips(&oOriginalClips, m_vTrackOffset, m_vPositionOffset, &m_oClonedClips);

	const char* vpFileName;
	int32* vpEntry;
	int32 vMediaClipID;
	list<EMGClip*>::iterator oIterator = oOriginalClips.begin();
	while(oIterator != oOriginalClips.end())
	{
		if((*oIterator) -> GetType() == EM_TYPE_ANY_AUDIO || (*oIterator) -> GetType() == EM_TYPE_ANY_VIDEO)
		{
			vMediaClipID = (*oIterator) -> GetMediaClipID();
			vpEntry = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vMediaClipID));
			vpFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, vpEntry));
			MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(vpFileName));
		}
		oIterator++;
	}

	char vOutput[80];
	if(oOriginalClips.size() == 1)
		sprintf(vOutput, "Clip cloned, Offset: %s, %ld", EMGUIUtilities::Instance() -> GetTimeString(m_vPositionOffset, EM_SMPTE), m_vTrackOffset);
	else
		sprintf(vOutput, "%ld clips cloned, Offset: %s, %ld", oOriginalClips.size(), EMGUIUtilities::Instance() -> GetTimeString(m_vPositionOffset, EM_SMPTE), m_vTrackOffset);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));

	return &m_oClonedClips;
}

const char* CommandCloneClips::GetShortDescription()
{
	return "Clone Clips";
}

bool CommandCloneClips::IsUndoable()
{
	return true;
}

bool CommandCloneClips::RequiresParameters()
{
	return true;
}

void CommandCloneClips::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_DELETE_CLIPS_FROM_LIST, &m_oClonedClips);
	 	m_opTrackView -> GetView() -> UnlockWindow();
	}
}


