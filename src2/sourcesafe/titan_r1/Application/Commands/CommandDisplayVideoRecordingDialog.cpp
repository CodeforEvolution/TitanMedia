#include "CommandDisplayVideoRecordingDialog.h"

CommandDisplayVideoRecordingDialog::CommandDisplayVideoRecordingDialog() :
m_vInitialized(false)
{
}

void CommandDisplayVideoRecordingDialog::ExecuteE()
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog(200, 100);
	m_oDialog.DisplayDialog();
}

bool CommandDisplayVideoRecordingDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayVideoRecordingDialog::RequiresParameters()
{
	return false;
}


