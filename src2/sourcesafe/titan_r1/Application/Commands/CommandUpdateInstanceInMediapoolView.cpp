#include "CommandUpdateInstanceInMediaPoolView.h"

#include "MediaPoolView.h"

CommandUpdateInstanceInMediaPoolView::CommandUpdateInstanceInMediaPoolView()
{
}

void* CommandUpdateInstanceInMediaPoolView::ExecuteE(void* p_opFullpathFilename, void* , void* )
{
	MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(static_cast<char*>(p_opFullpathFilename)));
	return NULL;
}

bool CommandUpdateInstanceInMediaPoolView::IsUndoable()
{
	return false;
}

bool CommandUpdateInstanceInMediaPoolView::RequiresParameters()
{
	return true;
}