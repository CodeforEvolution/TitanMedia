#include "CommandLoadMedia.h"

#include "EMFactory.h"
#include "EMFileWindow.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "FileDialogTarget.h"
#include "TrackWindow.h"

#include <string>

#define FILE_WINDOW_TITLE "Load Media File"

CommandLoadMedia::CommandLoadMedia(FileDialogTarget* p_opFileDialogTarget) :
m_opFileDialogTarget(p_opFileDialogTarget),
m_opFileWindow(NULL)
{
}

void CommandLoadMedia::ExecuteE()
{
	char* vpStartDirectory = "/boot/home/Projects/Media data/";
	if(m_opFileWindow == NULL)
		m_opFileWindow = EMFactory::Instance() -> CreateFileWindow(FILE_PANEL_LOAD_FILE, FILE_WINDOW_TITLE, NULL, false, NULL, NULL, vpStartDirectory, EM_LOAD_MEDIA_REQUESTED);
	m_opFileWindow -> RemoveListeners();
	m_opFileWindow -> AddListener(m_opFileDialogTarget);
	if(m_opFileWindow -> IsHidden())
		m_opFileWindow -> Show();
}

bool CommandLoadMedia::IsUndoable()
{
	return false;
}

bool CommandLoadMedia::RequiresParameters()
{
	return false;
}
