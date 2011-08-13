#include "EMFileManager.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinFileManager.h"
#endif

EMFileManager* EMFileManager::m_opInstance = NULL;

EMFileManager* EMFileManager::Instance()
{
	if(m_opInstance == NULL)
#ifdef PLATFORM_WINDOWS
		m_opInstance = new EMWinFileManager();
#endif
	return m_opInstance;
}
