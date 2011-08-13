#include "CommandClearUndoStack.h"

#include "EMCommandRepository.h"

#include <string>

CommandClearUndoStack::CommandClearUndoStack()
{
}

void CommandClearUndoStack::ExecuteE()
{
	EMCommandRepository::Instance() -> ClearUndoStack();
}

bool CommandClearUndoStack::IsUndoable()
{
	return false;
}

bool CommandClearUndoStack::RequiresParameters()
{
	return false;
}




