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

#ifndef __COMMAND_CLOSE_AUDIO_EXTRACTION_CONVERSION_DIALOG
#define __COMMAND_CLOSE_AUDIO_EXTRACTION_CONVERSION_DIALOG

#include "EMCommand.h"

class AudioExtractionConversionDialog;
class EMWindow;

class CommandCloseAudioExtractionConversionDialog : public EMCommand
{
public:
	CommandCloseAudioExtractionConversionDialog();
	~CommandCloseAudioExtractionConversionDialog();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ){return NULL;};
	bool IsUndoable();
	bool RequiresParameters();

private:
//	AudioExtractionConversionDialog* m_opDialog;
};

#endif
