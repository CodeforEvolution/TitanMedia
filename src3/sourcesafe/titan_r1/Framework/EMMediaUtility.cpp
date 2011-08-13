#include "EMMediaUtility.h"

#include "EMGlobals.h"
#include "EMMediaFormat.h"

#ifdef PLATFORM_BEOS
#include "EMBeMediaUtility.h"
#endif
#ifdef PLATFORM_WINDOWS
#include "EMWinMediaUtility.h"
#endif

EMMediaUtility* EMMediaUtility::m_opInstance = NULL;

EMMediaUtility* EMMediaUtility::Instance()
{
	if(m_opInstance == NULL)
#ifdef PLATFORM_BEOS
		m_opInstance = new EMBeMediaUtility();
#endif

#ifdef PLATFORM_WINDOWS
		m_opInstance = new EMWinMediaUtility();
#endif
	return m_opInstance;
}

void EMMediaUtility::Delete()
{
	if(EMMediaUtility::m_opInstance != NULL)
		delete EMMediaUtility::m_opInstance;

	EMMediaUtility::m_opInstance = NULL;
}

EMMediaUtility::EMMediaUtility()
{
}

EMMediaUtility::~EMMediaUtility()
{
}