#include "CommandDeleteClipsFromList.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGUIUtilities.h"
#include "EMImageBufferRepository.h"
#include "EMRect.h"
#include "EMView.h"
#include "MediaPoolView.h"
#include "TrackView.h"

CommandDeleteClipsFromList::CommandDeleteClipsFromList(TrackView* p_opTrackView, list<EMGClip*> p_oDeletedClips, list<EMCommand*> p_oMediaCommands) :
m_oDeletedClips(p_oDeletedClips),
m_oMediaCommands(p_oMediaCommands),
m_opTrackView(p_opTrackView)
{
}

CommandDeleteClipsFromList::CommandDeleteClipsFromList(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

EMCommand* CommandDeleteClipsFromList::CloneAndClear()
{
	CommandDeleteClipsFromList* opCommand = new CommandDeleteClipsFromList(m_opTrackView, m_oDeletedClips, m_oMediaCommands);
	m_oDeletedClips.clear();
	m_oMediaCommands.clear();
	return opCommand;
}

// Ownership of p_upParameterOne transferred to this object
void* CommandDeleteClipsFromList::ExecuteE(void* p_upParameterOne, void*  p_upParameterTwo, void* p_upParameterThree)
{
	int32 vMediaClipID;
	EMGClip* opClip;
	m_oDeletedClips = *(static_cast<list<EMGClip*>*>(p_upParameterOne));
	list<EMGClip*>::iterator oIterator = m_oDeletedClips.begin();
	list<EMGClip*> m_oClipToDelete;
	EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
	MediaPoolView* opMediaPoolView = MediaPoolView::Instance();
	bool vUpdateInstances(false);
	
	const char* vpFileName;
	int32* vpEntry;
	
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		while(oIterator != m_oDeletedClips.end())
		{
			EMImageBufferRepository::Instance() -> ClipRemoved(*oIterator);
			oIterator++;
		}

		m_opTrackView -> GetView() -> UnlockWindow();
	}

	oIterator = m_oDeletedClips.begin();
	// FIXME: Find a way to delete all clips at once instead of one at a time
	while(oIterator != m_oDeletedClips.end())
	{
		opClip = *oIterator;
		vMediaClipID = opClip -> GetMediaClipID();

		if(m_opTrackView -> GetView() -> LockWindow())
		{
			if(opClip -> GetType() == EM_TYPE_ANY_AUDIO || opClip -> GetType() == EM_TYPE_ANY_VIDEO)
			{
				vpEntry = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vMediaClipID));
				vpFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, vpEntry));
				vUpdateInstances = true;
			}

			m_oClipToDelete.push_back(opClip);
			opClipRepository -> DeleteClips(&m_oClipToDelete, false, true);
			m_oClipToDelete.clear();

			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DELETE_CLIP, &vMediaClipID);
			m_oMediaCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_DELETE_CLIP));

			if(vUpdateInstances)
				opMediaPoolView -> ChangeInstances(opMediaPoolView -> GetIndexID(vpFileName));

			m_opTrackView -> GetView() -> UnlockWindow();
		}
		oIterator++;
	}
	return NULL;
}

const char* CommandDeleteClipsFromList::GetShortDescription()
{
	return "Delete Clips";
}

bool CommandDeleteClipsFromList::IsUndoable()
{
	return true;
}

bool CommandDeleteClipsFromList::RequiresParameters()
{
	return true;
}

void CommandDeleteClipsFromList::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		EMCommand* opCommand;
		list<EMCommand*>::iterator oMediaCommandIterator = m_oMediaCommands.begin();
		while(oMediaCommandIterator != m_oMediaCommands.end())
		{
			opCommand = *oMediaCommandIterator;
			opCommand -> UndoE();
			oMediaCommandIterator++;
			delete opCommand;
		}

		EMGClipRepository::Instance() -> AddClips(&m_oDeletedClips);

		list<EMGClip*>::iterator oIterator = m_oDeletedClips.begin();
		int32 vMediaFileID;
		int32 vMediaClipID;
		while(oIterator != m_oDeletedClips.end())
		{
			(*oIterator) -> SetFrame((*oIterator) -> GetTrack() -> GetClipContainer() -> CalculateClipFrame(*oIterator));
			if((*oIterator) -> GetType() == EM_TYPE_ANY_AUDIO || (*oIterator) -> GetType() == EM_TYPE_ANY_VIDEO)
			{
				vMediaClipID = (*oIterator) -> GetMediaClipID();
				vMediaFileID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vMediaClipID)));
				EMImageBufferRepository::Instance() -> ClipAdded(*oIterator, vMediaFileID);
			}
			(*oIterator) -> UpdateContent();
			oIterator++;
		}

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

