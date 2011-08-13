#include "EMMessageBox.h"

#include "EMWindow.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinMessageBox.h"
#endif

EMMessageBox* EMMessageBox::m_opInstance = NULL;

EMMessageBox::EMMessageBox()
{
}

EMMessageBox* EMMessageBox::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;
#ifdef PLATFORM_WINDOWS
	m_opInstance = new EMWinMessageBox();
#endif
	return m_opInstance;
}
