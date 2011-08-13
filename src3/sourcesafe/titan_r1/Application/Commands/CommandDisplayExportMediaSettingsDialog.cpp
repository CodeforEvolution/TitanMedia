#include "CommandDisplayExportMediaSettingsDialog.h"

CommandDisplayExportMediaSettingsDialog::CommandDisplayExportMediaSettingsDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayExportMediaSettingsDialog::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	EMWindow* opParentWindow = static_cast<EMWindow*>(p_upParameterOne);
	int vRenderType = reinterpret_cast<int>(p_upParameterTwo);

	if(!m_vInitialized)
	{
		m_oAudioDialog.Init();
		m_oAudioDialog.SetMainWindow(m_opWindow);
		m_oAudioDialog.SetParent(opParentWindow);

		m_oMediaDialog.SetExportMediaDialog(p_upParameterThree);
		m_oMediaDialog.Init();
		m_oMediaDialog.SetMainWindow(m_opWindow);
		m_oMediaDialog.SetParent(opParentWindow);
		m_vInitialized = true;
	}
	switch(vRenderType)
	{
		case 0:
			m_oAudioDialog.MoveDialog();
			m_oAudioDialog.DisplayDialog();
			break;
		case 1:
			m_oMediaDialog.EnableAudioVideo(true, true);
			m_oMediaDialog.MoveDialog();
			m_oMediaDialog.DisplayDialog();
			break;
		case 2:
			m_oMediaDialog.EnableAudioVideo(false, true);
			m_oMediaDialog.MoveDialog();
			m_oMediaDialog.DisplayDialog();
			break;
		default:
			EMDebugger("CommandDisplayExportMediaSettingsDialog:\nThis rendertype does not exist");
	}

	return NULL;
}

bool CommandDisplayExportMediaSettingsDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayExportMediaSettingsDialog::RequiresParameters()
{
	return true;
}


