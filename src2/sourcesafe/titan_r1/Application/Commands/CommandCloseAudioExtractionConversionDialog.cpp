#include "CommandCloseAudioExtractionConversionDialog.h"

#include "AudioExtractionConversionDialog.h"

CommandCloseAudioExtractionConversionDialog::CommandCloseAudioExtractionConversionDialog()
{
}

CommandCloseAudioExtractionConversionDialog::~CommandCloseAudioExtractionConversionDialog()
{
}

void CommandCloseAudioExtractionConversionDialog::ExecuteE()
{
	AudioExtractionConversionDialog::Instance() -> HideDialog();
}

bool CommandCloseAudioExtractionConversionDialog::IsUndoable()
{
	return false;
}

bool CommandCloseAudioExtractionConversionDialog::RequiresParameters()
{
	return false;
}
