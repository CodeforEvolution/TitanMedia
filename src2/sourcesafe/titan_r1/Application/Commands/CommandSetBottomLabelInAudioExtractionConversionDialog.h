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

#ifndef __COMMAND_SET_BOTTOM_LABEL_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG
#define __COMMAND_SET_BOTTOM_LABEL_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG

#include "EMCommand.h"

class AudioExtractionConversionDialog;

class CommandSetBottomLabelInAudioExtractionConversionDialog : public EMCommand
{
public:
	CommandSetBottomLabelInAudioExtractionConversionDialog();
	~CommandSetBottomLabelInAudioExtractionConversionDialog();
	void ExecuteE(){};
	void* ExecuteE(void* p_opBottomLabel, void* , void* );
	bool IsUndoable();
	bool RequiresParameters();

private:
//	AudioExtractionConversionDialog* m_opDialog;
};

#endif
