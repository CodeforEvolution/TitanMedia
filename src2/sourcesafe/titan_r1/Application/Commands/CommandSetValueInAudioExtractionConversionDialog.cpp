#include "CommandSetValueInAudioExtractionConversionDialog.h"

#include "AudioExtractionConversionDialog.h"

CommandSetValueInAudioExtractionConversionDialog::CommandSetValueInAudioExtractionConversionDialog()
{
}

CommandSetValueInAudioExtractionConversionDialog::~CommandSetValueInAudioExtractionConversionDialog()
{
}

void* CommandSetValueInAudioExtractionConversionDialog::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint16* vpValue = static_cast<uint16*>(p_upParameterOne);
	AudioExtractionConversionDialog::Instance() -> SetValue(*vpValue);
	return NULL;
}

bool CommandSetValueInAudioExtractionConversionDialog::IsUndoable()
{
	return false;
}

bool CommandSetValueInAudioExtractionConversionDialog::RequiresParameters()
{
	return true;
}
