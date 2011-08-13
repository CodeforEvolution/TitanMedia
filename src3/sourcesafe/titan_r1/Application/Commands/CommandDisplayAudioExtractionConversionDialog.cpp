#include "CommandDisplayAudioExtractionConversionDialog.h"

#include "AudioExtractionConversionDialog.h"

CommandDisplayAudioExtractionConversionDialog::CommandDisplayAudioExtractionConversionDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

CommandDisplayAudioExtractionConversionDialog::~CommandDisplayAudioExtractionConversionDialog()
{
	AudioExtractionConversionDialog::Delete();
}

void* CommandDisplayAudioExtractionConversionDialog::ExecuteE(void* p_opTopLabel, void* p_opBottomLabel, void* p_opCommandID)
{
	if(!m_vInitialized)
	{
		m_opDialog = AudioExtractionConversionDialog::Instance();
		m_opDialog -> Init();
		m_opDialog -> SetParent(m_opWindow);
		m_vInitialized = true;
	}
	if(p_opTopLabel != NULL)
		m_opDialog -> SetTopLabel(static_cast<char*>(p_opTopLabel));
	if(p_opBottomLabel != NULL)
		m_opDialog -> SetBottomLabel(static_cast<char*>(p_opBottomLabel));
	m_opDialog -> SetCommand(reinterpret_cast<uint32>(p_opCommandID));
	m_opDialog -> MoveDialog();
	m_opDialog -> DisplayDialog();
	return NULL;
}

bool CommandDisplayAudioExtractionConversionDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayAudioExtractionConversionDialog::RequiresParameters()
{
	return true;
}
