#include "CommandChangeExplorerListViewControlStyle.h"


CommandChangeExplorerListViewControlStyle::CommandChangeExplorerListViewControlStyle(EMListViewControl* p_opExplorerListViewControl, EMListViewStyle p_eListViewStyle) :
m_opExplorerListViewControl(p_opExplorerListViewControl),
m_eListViewStyle(p_eListViewStyle)
{
}

void CommandChangeExplorerListViewControlStyle::ExecuteE()
{
	m_opExplorerListViewControl -> SwitchView(m_eListViewStyle);
}

bool CommandChangeExplorerListViewControlStyle::IsUndoable()
{
	return false;
}

bool CommandChangeExplorerListViewControlStyle::RequiresParameters()
{
	return false;
}


