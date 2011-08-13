#include "CommandDisplayPropertyPageDialog.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"

CommandDisplayPropertyPageDialog::CommandDisplayPropertyPageDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayPropertyPageDialog::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint32 vEffectID(*static_cast<uint32*>(p_upParameterOne));
	//EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_NATIVE_DIRECTX_EFFECT, &vEffectID);
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetParent(m_opWindow);
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog();
	
	return NULL;
}

bool CommandDisplayPropertyPageDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayPropertyPageDialog::RequiresParameters()
{
	return true;
}

