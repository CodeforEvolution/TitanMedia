#include "CommandUndo.h"

#include "EMCommandRepository.h"

CommandUndo::CommandUndo()
{
}

void CommandUndo::ExecuteE()
{
	EMCommandRepository::Instance() -> Undo();
}

bool CommandUndo::IsUndoable()
{
	return false;
}

bool CommandUndo::RequiresParameters()
{
	return false;
}

