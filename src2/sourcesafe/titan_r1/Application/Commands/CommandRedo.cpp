#include "CommandRedo.h"

#include "EMCommandRepository.h"

CommandRedo::CommandRedo()
{
}

void CommandRedo::ExecuteE()
{
	EMCommandRepository::Instance() -> Redo();
}

bool CommandRedo::IsUndoable()
{
	return false;
}

bool CommandRedo::RequiresParameters()
{
	return false;
}

