#include "CommandDeleteEntryFromMediaPoolView.h"

#include "MediaPoolView.h"

CommandDeleteEntryFromMediaPoolView::CommandDeleteEntryFromMediaPoolView()
{
}

void* CommandDeleteEntryFromMediaPoolView::ExecuteE(void* p_opFullpathFilename, void* , void* )
{
	MediaPoolView::Instance() -> EraseEntry(static_cast<char*>(p_opFullpathFilename), true);
	return NULL;
}

bool CommandDeleteEntryFromMediaPoolView::IsUndoable()
{
	return false;
}

bool CommandDeleteEntryFromMediaPoolView::RequiresParameters()
{
	return true;
}