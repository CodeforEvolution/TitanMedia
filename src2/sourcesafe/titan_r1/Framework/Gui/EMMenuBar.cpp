#include "EMMenuBar.h"

EMMenuBar* EMMenuBar::m_opInstance = NULL;
//uint32 EMMenuBar::m_vLastTriggeredMenuItemID = 0;

EMMenuBar::EMMenuBar()
{
}

EMMenuBar::~EMMenuBar()
{
}
/*
uint32 EMMenuBar::GetLastTriggeredMenuItemID()
{
	return m_vLastTriggeredMenuItemID;
}
*/
EMMenuBar* EMMenuBar::Instance()
{
	return m_opInstance;
}

void EMMenuBar::SetInstance(EMMenuBar* p_opMenuBar)
{
	m_opInstance = p_opMenuBar;
}