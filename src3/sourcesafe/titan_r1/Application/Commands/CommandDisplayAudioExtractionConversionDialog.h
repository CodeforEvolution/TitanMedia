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

#ifndef __COMMAND_DISPLAY_AUDIO_EXTRACTION_CONVERSION_DIALOG
#define __COMMAND_DISPLAY_AUDIO_EXTRACTION_CONVERSION_DIALOG

#include "EMCommand.h"

class AudioExtractionConversionDialog;
class EMWindow;

class CommandDisplayAudioExtractionConversionDialog : public EMCommand
{
public:
	CommandDisplayAudioExtractionConversionDialog(EMWindow* p_opWindow);
	~CommandDisplayAudioExtractionConversionDialog();
	void ExecuteE(){};
	void* ExecuteE(void* p_opTopLabel, void* p_opBottomLabel, void* p_opCommandID);
	bool IsUndoable();
	bool RequiresParameters();

private:
	AudioExtractionConversionDialog* m_opDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
