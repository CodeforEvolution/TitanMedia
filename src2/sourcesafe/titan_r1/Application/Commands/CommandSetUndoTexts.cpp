#include "CommandSetUndoTexts.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMMenu.h"
#include "EMMenuBar.h"
#include "EMMenuItem.h"
#include "EMResourceRepository.h"
#include "ResourceIDs.h"

const int MAX_UNDO_STRING_LENGTH = 255;

CommandSetUndoTexts::CommandSetUndoTexts(EMMenuBar* p_opMenuBar) :
m_opMenuBar(p_opMenuBar),
m_opMenuItem(NULL)
{
}

void* CommandSetUndoTexts::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	EMCommand* opNextCommandInUndoStack = static_cast<EMCommand*>(p_upParameterOne);
	EMCommand* opUndoedCommand = static_cast<EMCommand*>(p_upParameterTwo);

	if(opUndoedCommand != NULL)
	{
		if(opUndoedCommand -> GetShortDescription() == NULL)
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, "Command Undoed");
		else
		{
			char vUndoString[MAX_UNDO_STRING_LENGTH];
			sprintf(vUndoString, "%s Undoed", opUndoedCommand -> GetShortDescription());
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, vUndoString);
		}
	}

	if(m_opMenuItem == NULL)
	{
		string* opMenuName = EMResourceRepository::GetString(RES_TM_EDIT);
		string* opItemName = EMResourceRepository::GetString(RES_TM_UNDO);
		EMMenu* opMenu = m_opMenuBar -> GetMenu(opMenuName);
		m_opMenuItem = opMenu -> GetMenuItem(opItemName);
	}

	char vMenuLabel[MAX_UNDO_STRING_LENGTH];
	if(opNextCommandInUndoStack == NULL)
	{
		m_opMenuItem -> SetName(EMResourceRepository::GetString(RES_TM_UNDO) -> c_str());
		m_opMenuItem -> SetEnabled(false);
	}
	else if(opNextCommandInUndoStack -> GetShortDescription() == NULL)
	{
		m_opMenuItem -> SetName(EMResourceRepository::GetString(RES_TM_UNDO) -> c_str());
		m_opMenuItem -> SetEnabled(true);
	}
	else
	{
		sprintf(vMenuLabel, "%s - %s", EMResourceRepository::GetString(RES_TM_UNDO) -> c_str(), opNextCommandInUndoStack -> GetShortDescription());
		m_opMenuItem -> SetName(vMenuLabel);
		m_opMenuItem -> SetEnabled(true);
	}

	return NULL;
}

bool CommandSetUndoTexts::IsUndoable()
{
	return false;
}

bool CommandSetUndoTexts::RequiresParameters()
{
	return true;
}

