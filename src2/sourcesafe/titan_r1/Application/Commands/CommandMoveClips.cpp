#include "CommandMoveClips.h"

#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "EMGUIUtilities.h"
#include "EMView.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

CommandMoveClips::CommandMoveClips(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

CommandMoveClips::CommandMoveClips(TrackView* p_opTrackView, list<EMGClip*> p_oMovedClips, int32 p_vTrackOffset, int64 p_vPositionOffset) :
m_oMovedClips(p_oMovedClips),
m_opTrackView(p_opTrackView),
m_vPositionOffset(p_vPositionOffset),
m_vTrackOffset(p_vTrackOffset)
{
}

CommandMoveClips::~CommandMoveClips()
{
}

EMCommand* CommandMoveClips::CloneAndClear()
{
	CommandMoveClips* opCommand = new CommandMoveClips(m_opTrackView, m_oMovedClips, m_vTrackOffset, m_vPositionOffset);
	m_oMovedClips.clear();
	return opCommand;
}

void CommandMoveClips::ExecuteBody(list<EMGClip*> p_oMovedClips, int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	/*
	// Let's first check if a video clip is present amongst the selected clips, if so then quantize the position to the nearest video frame
	// Note, clips will have their own quantizing of this position, but it gives them a hint on where to go from
	bool vVideoClipPresent(false);
	list<EMGClip*>::iterator oIterator = p_oMovedClips.begin();
	while(oIterator != p_oMovedClips.end())
	{
		if((*oIterator) -> GetType() == EM_TYPE_ANY_VIDEO)
		{
			vVideoClipPresent = true;
			break;
		}
		oIterator++;
	}
	if(vVideoClipPresent)
		p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateFrame(p_vPositionOffset);
	*/
	EMGClipRepository::Instance() -> MoveClips(&p_oMovedClips, p_vTrackOffset, p_vPositionOffset);

	char vOutput[80];
	if(m_oMovedClips.size() == 1)
		sprintf(vOutput, "Clip moved, Offset: %s, %ld", EMGUIUtilities::Instance() -> GetTimeString(m_vPositionOffset, EM_SMPTE), m_vTrackOffset);
	else
		sprintf(vOutput, "%ld clips moved, Offset: %s, %ld", m_oMovedClips.size(), EMGUIUtilities::Instance() -> GetTimeString(m_vPositionOffset, EM_SMPTE), m_vTrackOffset);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));
}

// Note: If the third parameter is NULL, the selected clips are moved, else the third parameter contains the clips to move
void* CommandMoveClips::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)

{
	m_vTrackOffset = *(static_cast<int32*>(p_opParameterOne));
	m_vPositionOffset = *(static_cast<int64*>(p_opParameterTwo));

	if(p_opParameterThree == NULL)
	{
		EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
		m_oMovedClips = *(opClipRepository -> GetSelectedClips());
	}
	else
		m_oMovedClips = *(static_cast<list<EMGClip*>*>(p_opParameterThree));
	
	ExecuteBody(m_oMovedClips, m_vTrackOffset, m_vPositionOffset);
	return NULL;
}

const char* CommandMoveClips::GetShortDescription()
{
	return "Move Clips";
}

bool CommandMoveClips::IsUndoable()
{
	return true;
}

void CommandMoveClips::RedoE()
{
	ExecuteBody(m_oMovedClips, m_vTrackOffset, m_vPositionOffset);
}

bool CommandMoveClips::RequiresParameters()
{
	return true;
}

void CommandMoveClips::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		ExecuteBody(m_oMovedClips, -1 * m_vTrackOffset, -1 * m_vPositionOffset);
		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
