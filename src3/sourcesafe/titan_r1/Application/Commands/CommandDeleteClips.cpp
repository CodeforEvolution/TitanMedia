#include "CommandDeleteClips.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMImageBufferRepository.h"
#include "EMMessages.h"
#include "EMRect.h"
#include "EMView.h"
#include "TrackView.h"

CommandDeleteClips::CommandDeleteClips()
{
}

EMCommand* CommandDeleteClips::CloneAndClear()
{
	CommandDeleteClips* opCommand = new CommandDeleteClips();
	return opCommand;
}

void CommandDeleteClips::ExecuteE()
{
	EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
	list<EMGClip*> oDeletedClips = *(opClipRepository -> GetSelectedClips());

	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_CLIPS_FROM_LIST, &oDeletedClips, NULL, NULL);
}

bool CommandDeleteClips::IsUndoable()
{
	return false;
}

bool CommandDeleteClips::RequiresParameters()
{
	return false;
}
