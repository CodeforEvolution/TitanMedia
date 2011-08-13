/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Command
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __COMMAND_SET_VALUE_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG
#define __COMMAND_SET_VALUE_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG

#include "EMCommand.h"

class AudioExtractionConversionDialog;

class CommandSetValueInAudioExtractionConversionDialog : public EMCommand
{
public:
	CommandSetValueInAudioExtractionConversionDialog();
	~CommandSetValueInAudioExtractionConversionDialog();
	void ExecuteE(){};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
//	AudioExtractionConversionDialog* m_opDialog;
};

#endif
