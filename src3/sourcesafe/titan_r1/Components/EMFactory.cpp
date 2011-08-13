#include "EMFactory.h"

#include "EMWindow.h"

#ifdef PLATFORM_BEOS
#include "EMBeFactory.h"
#endif
#ifdef PLATFORM_WINDOWS
#include "EMWinFactory.h"
#endif

EMFactory* EMFactory::m_opInstance = NULL;

EMFactory::EMFactory()
{
}

EMFactory::~EMFactory()
{
}

EMFactory* EMFactory::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;
#ifdef PLATFORM_BEOS
	m_opInstance = new EMBeFactory();
#endif
#ifdef PLATFORM_WINDOWS
	m_opInstance = new EMWinFactory();
#endif
	return m_opInstance;
}
