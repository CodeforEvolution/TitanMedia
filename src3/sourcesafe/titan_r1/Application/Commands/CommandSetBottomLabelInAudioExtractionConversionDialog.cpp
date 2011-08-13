#include "CommandSetBottomLabelInAudioExtractionConversionDialog.h"

#include "AudioExtractionConversionDialog.h"

CommandSetBottomLabelInAudioExtractionConversionDialog::CommandSetBottomLabelInAudioExtractionConversionDialog()
{
}

CommandSetBottomLabelInAudioExtractionConversionDialog::~CommandSetBottomLabelInAudioExtractionConversionDialog()
{
}

void* CommandSetBottomLabelInAudioExtractionConversionDialog::ExecuteE(void* p_opBottomLabel, void* , void* )
{
	if(p_opBottomLabel != NULL)
		AudioExtractionConversionDialog::Instance() -> SetBottomLabel(static_cast<char*>(p_opBottomLabel));
	return NULL;
}

bool CommandSetBottomLabelInAudioExtractionConversionDialog::IsUndoable()
{
	return true;
}

bool CommandSetBottomLabelInAudioExtractionConversionDialog::RequiresParameters()
{
	return true;
}